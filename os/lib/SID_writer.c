#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "i2c.h"
#include "task.h"

void LIB_SID_tx_thread(void) // SID writer thread
 {
 
  SID_msg_t SID_msg_in;
  int SID_message_pipe, pollrc, rc;
  struct pollfd fds;
  uint16_t bit_mask = 255;
  uint8_t msg_byte_lo, msg_byte_hi;

  usleep(100); // put the thread to sleep for some time - pipe descriptor is getting ready...

  SID_message_pipe = G_QSID_tasks[TASK_SID_WRITER].input_pipe[0];

  fds.fd = SID_message_pipe;
  fds.events = POLLIN;
  
 
  while(1)
   {

    pollrc = poll(&fds, 1, -1);

    if (pollrc < 0)
     {
       perror("poll");
     }

    else if( pollrc > 0)
     {

      if( fds.revents & POLLIN )   
        rc = read(SID_message_pipe, &SID_msg_in, sizeof(SID_msg_t) );
         if (rc > 0) 
          {
            SYS_debug(DEBUG_HIGH,"LIB_SID_tx_thread: received %d bytes from pipe",rc);

            msg_byte_lo = bit_mask & SID_msg_in.reg_data;
            msg_byte_hi = SID_msg_in.reg_data >> 8;

           SYS_debug(DEBUG_HIGH,"SID_write_msg: about to write %x, %x into SID at 0x%x...",SID_msg_in.reg_data, SID_msg_in.reg_addr, SID_msg_in.SID_addr);

           if(SID_msg_in.reg_data > 255)
            {
             LIB_SID_via_mcp23017_write(G_i2c_voice_bus, SID_msg_in.SID_addr, SID_msg_in.reg_addr, msg_byte_lo);
             LIB_SID_via_mcp23017_write(G_i2c_voice_bus, SID_msg_in.SID_addr, SID_msg_in.reg_addr+1, msg_byte_hi);
            }
           else
            LIB_SID_via_mcp23017_write(G_i2c_voice_bus, SID_msg_in.SID_addr, SID_msg_in.reg_addr, msg_byte_lo);

          }
     }


   }   

 }
