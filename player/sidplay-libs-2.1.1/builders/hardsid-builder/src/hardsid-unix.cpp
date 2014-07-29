/***************************************************************************
             hardsid.cpp  -  Hardsid support interface.
                             -------------------
    begin                : Fri Dec 15 2000
    copyright            : (C) 2001-2001 by Jarno Paananen
    email                : jpaana@s2.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *  $Log: hardsid-unix.cpp,v $
 *  Revision 1.14  2004/05/27 21:18:28  jpaana
 *  The filter ioctl was reversed
 *
 *  Revision 1.13  2004/05/05 23:48:01  s_a_white
 *  Detect available sid devices on Unix system.
 *
 *  Revision 1.12  2004/04/29 23:20:01  s_a_white
 *  Optimisation to polling hardsid delay write to only access the hardsid
 *  if really necessary.
 *
 *  Revision 1.11  2003/10/28 00:15:16  s_a_white
 *  Get time with respect to correct clock phase.
 *
 *  Revision 1.10  2003/01/20 16:25:25  s_a_white
 *  Updated for new event scheduler interface.
 *
 *  Revision 1.9  2002/10/17 18:36:43  s_a_white
 *  Prevent multiple unlocks causing a NULL pointer access.
 *
 *  Revision 1.8  2002/08/14 16:03:54  jpaana
 *  Fixed to compile with new HardSID::lock method
 *
 *  Revision 1.7  2002/07/20 08:36:24  s_a_white
 *  Remove unnecessary and pointless conts.
 *
 *  Revision 1.6  2002/02/17 17:24:51  s_a_white
 *  Updated for new reset interface.
 *
 *  Revision 1.5  2002/01/30 01:47:47  jpaana
 *  Read ioctl used wrong parameter type and delay ioctl takes uint, not uint*
 *
 *  Revision 1.4  2002/01/30 00:43:50  s_a_white
 *  Added realtime delays even when there is no accesses to
 *  the sid.  Prevents excessive CPU usage.
 *
 *  Revision 1.3  2002/01/29 21:47:35  s_a_white
 *  Constant fixed interval delay added to prevent emulation going fast when
 *  there are no writes to the sid.
 *
 *  Revision 1.2  2002/01/29 00:32:56  jpaana
 *  Use the new read and delay IOCTLs
 *
 *  Revision 1.1  2002/01/28 22:35:20  s_a_white
 *  Initial Release.
 *
 *
 ***************************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>
#include "config.h"
#include "hardsid-emu.h"

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "i2c_lib.h"
#include "MCP23017.h"
#include "SID.h"
#include "i2c.h"

// Move these to common header file
#define HSID_IOCTL_RESET     _IOW('S', 0, int)
#define HSID_IOCTL_FIFOSIZE  _IOR('S', 1, int)
#define HSID_IOCTL_FIFOFREE  _IOR('S', 2, int)
#define HSID_IOCTL_SIDTYPE   _IOR('S', 3, int)
#define HSID_IOCTL_CARDTYPE  _IOR('S', 4, int)
#define HSID_IOCTL_MUTE      _IOW('S', 5, int)
#define HSID_IOCTL_NOFILTER  _IOW('S', 6, int)
#define HSID_IOCTL_FLUSH     _IO ('S', 7)
#define HSID_IOCTL_DELAY     _IOW('S', 8, int)
#define HSID_IOCTL_READ      _IOWR('S', 9, int*)

bool       HardSID::m_sidFree[16] = {0};
const uint HardSID::voices = HARDSID_VOICES;
uint       HardSID::sid = 0;
char       HardSID::credit[];

HardSID::HardSID (sidbuilder *builder)
:sidemu(builder),
 Event("HardSID Delay"),
 m_handle(0),
 m_eventContext(NULL),
 m_phase(EVENT_CLOCK_PHI1),
 m_instance(sid++),
 m_status(false),
 m_locked(false)
{
    uint num = 16;
    for ( uint i = 0; i < 16; i++ )
    {
        if(m_sidFree[i] == 0)
        {
            m_sidFree[i] = 1;
            num = i;
            break;
        }
    }

    // All sids in use?!?
    if ( num == 16 )
        return;

    m_instance = num;

    {

        m_handle = i2c_open("/dev/i2c-1");

        if (m_handle < 0)
        {
          sprintf (m_errorBuffer, "HARDSID ERROR: Cannot access /dev/i2c-1");
          return;
        }
    }

    m_status = true;
    ioaddr = 0x26;
    reset ();
}

HardSID::~HardSID()
{
    sid--;
    m_sidFree[m_instance] = 0;
    if (m_handle)
        close (m_handle);
}

void HardSID::reset (uint8_t volume)
{
  set_i2c_register(G_i2c_voice_bus, ioaddr, MCP23017_IODIR_A, MCP23017_GPIO_OUT);
  set_i2c_register(G_i2c_voice_bus, ioaddr, MCP23017_IODIR_B, MCP23017_GPIO_OUT);
  SID_via_mcp23017_reset(m_handle,ioaddr);

  m_accessClk = 0;

  if (m_eventContext != NULL)
    m_eventContext->schedule (this, HARDSID_DELAY_CYCLES, m_phase);

}

uint8_t HardSID::read (uint_least8_t addr)
{
    if (!m_handle)
        return 0;

    event_clock_t cycles = m_eventContext->getTime (m_accessClk, m_phase);
    m_accessClk += cycles;

    while ( cycles > 0xffff )
    {
        /* delay */
        //ioctl(m_handle, HSID_IOCTL_DELAY, 0xffff);
        usleep(50);
        cycles -= 0xffff;
    }

    uint8_t packet = SID_via_mcp23017_read(m_handle, ioaddr, addr);

    cycles = 0;
    return (uint8_t) (packet & 0xff);
}

void HardSID::write (uint_least8_t addr, uint8_t data)
{
    if (!m_handle)
        return;

    event_clock_t cycles = m_eventContext->getTime (m_accessClk, m_phase);
    m_accessClk += cycles;


    while ( cycles > 0xffff )
    {
        /* delay */
        //ioctl(m_handle, HSID_IOCTL_DELAY, 0xffff);
        usleep(500);
        cycles -= 0xffff;
    }

    usleep(500);

    //printf("addr: %x, data %x\n",addr, data);
    //uint packet = (( cycles & 0xffff ) << 16 ) | (( addr & 0x1f ) << 8 )
    //    | (data & 0xff);
    cycles = 0;

    SID_via_mcp23017_write(m_handle,ioaddr,addr,data);

    //::write (m_handle, &packet, sizeof (packet));
    
   
}

void HardSID::voice (uint_least8_t num, uint_least8_t volume,
                     bool mute)
{
    // Only have 3 voices!
    if (num >= voices)
        return;
    muted[num] = mute;
    
    int cmute = 0;
    for ( uint i = 0; i < voices; i++ )
        cmute |= (muted[i] << i);
    //ioctl (m_handle, HSID_IOCTL_MUTE, cmute);
}

void HardSID::event (void)
{
    event_clock_t cycles = m_eventContext->getTime (m_accessClk, m_phase);
    if (cycles < HARDSID_DELAY_CYCLES)
    {
        m_eventContext->schedule (this, HARDSID_DELAY_CYCLES - cycles,
                                EVENT_CLOCK_PHI1);
    }
    else
    {
        uint delay = (uint) cycles;
        m_accessClk += cycles;
        //ioctl(m_handle, HSID_IOCTL_DELAY, delay);
        m_eventContext->schedule (this, HARDSID_DELAY_CYCLES, m_phase);
    }
}

void HardSID::filter(bool enable)
{
    //ioctl (m_handle, HSID_IOCTL_NOFILTER, !enable);
    return;
}

void HardSID::flush(void)
{
    //ioctl(m_handle, HSID_IOCTL_FLUSH);
    return;
}

bool HardSID::lock(c64env* env)
{
    if( env == NULL )
    {
	if (!m_locked)
	    return false;
        m_eventContext->cancel (this);
        m_locked = false;
        m_eventContext = NULL;
    }
    else
    {
	if (m_locked)
	    return false;
        m_locked = true;
        m_eventContext = &env->context();
        m_eventContext->schedule (this, HARDSID_DELAY_CYCLES, m_phase);
    }
    return true;
}




uint8_t HardSID::set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value) {

    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = 2;
    messages[0].buf   = outbuf;

    /* The first byte indicates which register we'll write */
    outbuf[0] = reg;

    /* 
     * The second byte indicates the value to write.  Note that for many
     * devices, we can write multiple, sequential registers at once by
     * simply making outbuf bigger.
     */

    outbuf[1] = value;

    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 1;
    }

    return 0;
}


uint8_t HardSID::get_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            uint8_t *val) {

    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * In order to read a register, we first do a "dummy write" by writing
     * 0 bytes to the register we want to read from.  This is similar to
     * the packet in set_i2c_register, except it's 1 byte rather than 2.
     */

    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = 1;
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = 1;
    messages[1].buf   = &inbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) { 
        return 1;
    }

    *val = inbuf;

    return 0;
}

int8_t HardSID::i2c_open(char *i2c_file_path)
 {
    int8_t i2c_descriptor;

    if (( i2c_descriptor = open(i2c_file_path, O_RDWR)) < 0) {
       return -1;
    }
   
   return i2c_descriptor;

 }

int HardSID::SID_via_mcp23017_reset(uint8_t i2c_dev, uint8_t ic_addr)
 {

    //write reset bit pattern to SID...
    set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b10100000);
    set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b00000000);
    usleep(10); // eight cycles should be enough, but...
    set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b10100000);

    return 0;

 }


int HardSID::SID_via_mcp23017_write(uint8_t i2c_dev, uint8_t ic_addr, uint8_t addr, uint8_t data)
 {

  uint8_t cshi_rsthi_mask=160; // RST = 1, CS = 1 [101xxxxx] where x = address bits
  uint8_t cslo_rsthi_mask=128; // RST = 1, CS = 0 [100xxxxx] where x = address bits
 
  uint8_t out_addr = 0; 

  out_addr = cshi_rsthi_mask | 0x0;  // clear write pattern (CS=1)
  set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);
 
  out_addr = cslo_rsthi_mask | addr; // merge five bits of address with control lines pattern (bitwise OR)  
 
  set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_A, data);
  set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);
 
  usleep(2); // sleep two cycles of the clock - this should be enough to get CS=0, CLOCK=1 pattern

  out_addr = cshi_rsthi_mask | 0x0;  // clear write pattern (CS=1)
  set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);

 }


uint8_t HardSID::SID_via_mcp23017_read(uint8_t i2c_dev, uint8_t ic_addr, uint8_t addr)
 {

  uint8_t cshi_rsthi_mask=224; // RST = 1, CS = 1 [111xxxxx] where x = address bits
  uint8_t cslo_rsthi_mask=192; // RST = 1, CS = 0 [110xxxxx] where x = address bits
 
  uint8_t out_addr = 0; 
  uint8_t data = 0;

  out_addr = cslo_rsthi_mask | addr; // merge five bits of address with control lines pattern (bitwise OR)

  set_i2c_register(i2c_dev, ic_addr, MCP23017_GPPU_A, 255);
  set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);

  usleep(2); // sleep two cycles of the clock - this should be enough to get CS=0, CLOCK=1 pattern

  get_i2c_register(i2c_dev, ic_addr, MCP23017_GPIO_A, &data);
  out_addr = cshi_rsthi_mask | out_addr;  // clear write pattern (CS=1)
  set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);

  return data;

 }

