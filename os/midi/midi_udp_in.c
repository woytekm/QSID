#include "common.h"
#include "defs.h"
#include "task.h"
#include "QSID_config.h"

/* MIDI over UDP receiver */

void MIDI_UDP_input(void)
 {

   /* UDP receiver code taken from: http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/udp-server.c  */
   uint8_t last_message_incomplete;
   uint16_t rc;
   int sockfd;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   unsigned char midi_message_buffer[1024];

   last_message_incomplete = 1; 

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(QSID_OS_MIDI_UDP_PORT);
   
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   len = sizeof(cliaddr);

   SYS_debug(DEBUG_HIGH,"MIDI_UDP_receiver: waiting for messages on port %d ...",QSID_OS_MIDI_UDP_PORT);

   while(1)
    {

      rc = recvfrom(sockfd,&midi_message_buffer,sizeof(midi_message_buffer),0,(struct sockaddr *)&cliaddr,&len);

      while(last_message_incomplete)   /* partial midi message reassembly. TODO: add midi_message_buffer overflow check */
       {
         if(MIDI_is_partial_message(midi_message_buffer, rc))    
           rc += recvfrom(sockfd,&midi_message_buffer + rc,sizeof(midi_message_buffer) - rc,0,(struct sockaddr *)&cliaddr,&len);
         else last_message_incomplete = 0;
       }

      MIDI_parse_one_MIDI_msg(midi_message_buffer, 0, rc);

      bzero(midi_message_buffer, rc);
      last_message_incomplete = 1;

    }

 }
