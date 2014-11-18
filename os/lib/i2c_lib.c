/*
 This software uses a BSD license.

Copyright (c) 2010, Sean Cross / chumby industries
All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.  
 * Neither the name of Sean Cross / chumby industries nor the names
   of its contributors may be used to endorse or promote products
   derived from this software without specific prior written
   permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 */

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include "i2c_lib.h"
#include "defs.h"

#ifdef I2C_BCM2835

#include "bcm2835.h"

#define RPI_I2C_BUS0 0
#define RPI_I2C_BUS1 1
#define RPI_QSID_I2C_CLOCK 1000000


int8_t LIB_i2c_bcm2835_init(void)
 {

  if(!bcm2835_init())
   {
    SYS_debug(DEBUG_NORMAL,"LIB_i2c_bcm2835_init: fatal: cannot initialize BCM2835 library!");
    return 0;
   }

  bcm2835_i2c_begin(RPI_I2C_BUS0);
  bcm2835_i2c_begin(RPI_I2C_BUS1);

  bcm2835_i2c_set_baudrate(RPI_I2C_BUS0, RPI_QSID_I2C_CLOCK);
  bcm2835_i2c_set_baudrate(RPI_I2C_BUS1, RPI_QSID_I2C_CLOCK);

  return 1;

 }


uint8_t LIB_set_i2c_register(int bus, uint8_t addr, uint8_t reg, uint8_t value)
 {

  uint8_t writebuf[2], reason;
  uint32_t wrote;

  writebuf[0] = reg;
  writebuf[1] = value;

  bcm2835_i2c_setSlaveAddress(bus, addr);

  reason = bcm2835_i2c_write(bus, &writebuf, 2, &wrote);

  if(reason == BCM2835_I2C_REASON_OK)
   return 0;
  else
   {
     SYS_debug(DEBUG_NORMAL,"LIB_set_i2c_register (BCM2835): register write failed (%d)",reason);
     return reason;
   }

 }


uint8_t LIB_get_i2c_register(int bus, unsigned char addr, unsigned char reg, uint8_t *val)
 {

  int32_t read;
  uint8_t reason;
  
  bcm2835_i2c_setSlaveAddress(bus, addr);

  reason = bcm2835_i2c_read_register_rs(bus, &reg, val, 1, &read);

  if(reason == BCM2835_I2C_REASON_OK)
   return 0;
  else 
   {
     SYS_debug(DEBUG_NORMAL,"LIB_get_i2c_register (BCM2835): register read failed (%d)",reason);
     return reason;
   }

 }


#else

uint8_t LIB_set_i2c_register(int file,
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

    //SYS_debug("set_i2c_register: addr: 0x%x, reg: 0x%x, val: 0x%x", addr, reg, value);

    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        SYS_debug(DEBUG_NORMAL,"I2C set failed: %d",errno);
        return 1;
    }

    return 0;
}


uint8_t LIB_get_i2c_register(int file,
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
        SYS_debug(DEBUG_NORMAL,"I2C get failed.");
        return 1;
    }

    *val = inbuf;

    return 0;
}

#endif

