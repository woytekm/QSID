#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "inventory.h"

void *LIB_SID_remote_control(void)
 {

   // UDP receiver code taken from: http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/udp-server.c

   int sockfd,n,i;
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
      SYS_debug(DEBUG_HIGH,"LIB_SID_remote_control: received following data: %x, %x",SID_control_packet.reg_addr, SID_control_packet.reg_data); 
 
      for(i = 1; i <= G_inventory_voice_count; i++)
       {
         SID_control_packet.SID_addr = G_voice_inventory[i].address;
         write(G_SID_writer_rx_pipe[1], &SID_control_packet, sizeof(SID_msg_t));
       }
     
    }

 }
