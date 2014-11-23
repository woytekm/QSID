#include "common.h"
#include "defs.h"
#include "SID.h"
#include "SID_writer.h"
#include "QSID_config.h"
#include "inventory.h"
#include "patch.h"
#include "task.h"

/* this task should accept midi sysex packets sent through UDP and set patch/SID registers accordingly  
 TODO:                                                                                                
 - define catalog of sysex messages                                                                   
 - rewrite this to accept sysex encapsulated in UDP instead of SID_msg_t                              
*/

void *LIB_SID_remote_control(void)
 {

   /* UDP receiver code taken from: http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/udp-server.c  */

   int sockfd,n,i,is_virtual;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   SID_msg_t SID_control_packet;

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(QSID_OS_SID_REMOTE_CONTROL_PORT);
   
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   len = sizeof(cliaddr);

   while(1)
    {
      SYS_debug(DEBUG_HIGH,"LIB_SID_remote_control: waiting for data...");
      n = recvfrom(sockfd,&SID_control_packet,sizeof(SID_msg_t),0,(struct sockaddr *)&cliaddr,&len);
      SYS_debug(DEBUG_HIGH,"LIB_SID_remote_control: received following data: 0x%X, 0x%X",SID_control_packet.reg_addr, SID_control_packet.reg_data); 


      if(SID_control_packet.reg_addr >= 32)
       SYS_debug(DEBUG_HIGH,"LIB_SID_remote_control: received QSID SID virtual register change");

      switch(SID_control_packet.reg_addr)
       {

         case SID_OSC1_CTRL:
           G_current_patch.osc1_control_reg = SID_control_packet.reg_data;
           break;

         case SID_OSC2_CTRL:
           G_current_patch.osc2_control_reg = SID_control_packet.reg_data;
           break;

         case SID_OSC3_CTRL:
           G_current_patch.osc3_control_reg = SID_control_packet.reg_data;
           break;

         case SID_OSC1_STATE:
          G_current_patch.osc1_on = SID_control_packet.reg_data;
          is_virtual = 1;
          break;
         
         case SID_OSC2_STATE:
          G_current_patch.osc2_on = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case SID_OSC3_STATE:
          G_current_patch.osc3_on = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case SID_OSC1_DETUNE:
          G_current_patch.osc1_detune = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case SID_OSC2_DETUNE:
          G_current_patch.osc2_detune = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case SID_OSC3_DETUNE:
          G_current_patch.osc3_detune = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case QSID_LFO1_RATE:
          G_current_patch.LFO1_rate = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case QSID_LFO1_SHAPE:
          G_current_patch.LFO1_shape = SID_control_packet.reg_data;
          write(G_QSID_tasks[TASK_LFO1].input_pipe[1], &SID_control_packet.reg_data, sizeof(uint8_t));
          is_virtual = 1;
          break;

         case QSID_LFO1_DEPTH:
          G_current_patch.LFO1_depth = SID_control_packet.reg_data;
          is_virtual = 1;
          break;

         case QSID_LFO1_ROUTE:
          G_current_patch.LFO1_routing = SID_control_packet.reg_data;
          write(G_QSID_tasks[TASK_LFO1].input_pipe[1], &SID_control_packet.reg_data, sizeof(uint8_t));
          is_virtual = 1;
          break;

       }
 
      if(!is_virtual)

        for(i = 1; i <= G_inventory_voice_count; i++)
         {
          SID_control_packet.SID_addr = G_voice_inventory[i].address;
          write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_control_packet, sizeof(SID_msg_t));
         }

      else is_virtual = 0;  /* clear the flag  */
     
    }

 }
