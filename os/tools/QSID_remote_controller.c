//
//
// QSID remote network controller - test tool before building hardware front control panel for QSID
//
//

#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "SID.h"
#include "patch.h"

patch_data_t current_patch;

void init_patch(void)
 {

   current_patch.osc1_adsr_attack = 0; //(upper 4 bits)
   current_patch.osc1_adsr_decay = 0; //(lower 4 bits)
   current_patch.osc1_adsr_sustain = 0; //(upper 4 bits)
   current_patch.osc1_adsr_release = 0; //(lower 4 bits) 

   current_patch.osc2_adsr_attack = 0; //(upper 4 bits)
   current_patch.osc2_adsr_decay = 0; //(lower 4 bits)
   current_patch.osc2_adsr_sustain = 0; //(upper 4 bits)
   current_patch.osc2_adsr_release = 0; //(lower 4 bits)

   current_patch.osc3_adsr_attack = 0; //(upper 4 bits)
   current_patch.osc3_adsr_decay = 0; //(lower 4 bits)
   current_patch.osc3_adsr_sustain = 0; //(upper 4 bits)
   current_patch.osc3_adsr_release = 0; //(lower 4 bits)

 }

void send_to_QSID(int sock, SID_msg_t *packet, struct sockaddr_in *srv_addr)
 { 

      if(sendto(sock,packet,sizeof(SID_msg_t),0,
             (struct sockaddr *)srv_addr,sizeof(struct sockaddr_in)) == -1)
         printf("sendto error: %d\n\r", errno);

 }

void draw_panel(void)
 {

  system("/usr/bin/clear");
  printf("==== QSID remote controller ===============\n\r");
  printf("===== AMP ADSR ============================\n\r");
  printf(" a -  attack  + A \n\r");
  printf(" d -  decay   + D \n\r");
  printf(" s - sustain  + S \n\r");
  printf(" r - release  + R \n\r");
  printf("===========================================\n\r");
  printf("============================== q - quit ===\n\r");

 }

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   SID_msg_t SID_control_packet;
   uint8_t i, input;

   if (argc != 2)
   {
      printf("usage:  %s <IP address>\n",argv[0]);
      exit(1);
   }

   printf("listening to SID control commands [q to quit]:\n");

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   
   servaddr.sin_port=htons(QSID_OS_SID_REMOTE_CONTROL_PORT);

   system ("/bin/stty raw -echo");

   init_patch();

   draw_panel();

   while (1)
   {
      input = getchar();
      
      if(input == 'q')
        {
         system ("/bin/stty cooked echo");
         break;
        }

      if(input == 'a')
       {
         if(current_patch.osc1_adsr_attack > 0 )
           current_patch.osc1_adsr_attack = current_patch.osc2_adsr_attack = current_patch.osc3_adsr_attack -= 16;

         SID_control_packet.reg_addr = SID_OSC1_ATTACK;
         SID_control_packet.reg_data = current_patch.osc1_adsr_attack | current_patch.osc1_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_ATTACK;
         SID_control_packet.reg_data = current_patch.osc2_adsr_attack | current_patch.osc2_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_ATTACK;
         SID_control_packet.reg_data = current_patch.osc3_adsr_attack | current_patch.osc3_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

       }

      if(input == 'A')
       {
         if(current_patch.osc1_adsr_attack < 240 )
           current_patch.osc1_adsr_attack = current_patch.osc2_adsr_attack = current_patch.osc3_adsr_attack += 16;

         SID_control_packet.reg_addr = SID_OSC1_ATTACK;
         SID_control_packet.reg_data = current_patch.osc1_adsr_attack | current_patch.osc1_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_ATTACK;
         SID_control_packet.reg_data = current_patch.osc2_adsr_attack | current_patch.osc2_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_ATTACK;
         SID_control_packet.reg_data = current_patch.osc3_adsr_attack | current_patch.osc3_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

       }

      if(input == 'd')
       {
         if(current_patch.osc1_adsr_decay > 0 )
           current_patch.osc1_adsr_decay = current_patch.osc2_adsr_decay = current_patch.osc3_adsr_decay -= 1;

         SID_control_packet.reg_addr = SID_OSC1_ATTACK;
         SID_control_packet.reg_data = current_patch.osc1_adsr_attack | current_patch.osc1_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_ATTACK;
         SID_control_packet.reg_data = current_patch.osc2_adsr_attack | current_patch.osc2_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_ATTACK;
         SID_control_packet.reg_data = current_patch.osc3_adsr_attack | current_patch.osc3_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);
       }

      if(input == 'D')
       {
         if(current_patch.osc1_adsr_decay < 15 )
           current_patch.osc1_adsr_decay = current_patch.osc2_adsr_decay = current_patch.osc3_adsr_decay += 1;

         SID_control_packet.reg_addr = SID_OSC1_ATTACK;
         SID_control_packet.reg_data = current_patch.osc1_adsr_attack | current_patch.osc1_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_ATTACK;
         SID_control_packet.reg_data = current_patch.osc2_adsr_attack | current_patch.osc2_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_ATTACK;
         SID_control_packet.reg_data = current_patch.osc3_adsr_attack | current_patch.osc3_adsr_decay;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);
       }
    
      if(input == 's')
       {
         if(current_patch.osc1_adsr_sustain > 0 )
           current_patch.osc1_adsr_sustain = current_patch.osc2_adsr_sustain = current_patch.osc3_adsr_sustain -= 16;
   
         SID_control_packet.reg_addr = SID_OSC1_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc1_adsr_sustain | current_patch.osc1_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc2_adsr_sustain | current_patch.osc2_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc3_adsr_sustain | current_patch.osc3_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);
       }

      if(input == 'S')
       {
         if(current_patch.osc1_adsr_sustain < 240 )
           current_patch.osc1_adsr_sustain = current_patch.osc2_adsr_sustain = current_patch.osc3_adsr_sustain += 16;

         SID_control_packet.reg_addr = SID_OSC1_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc1_adsr_sustain | current_patch.osc1_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc2_adsr_sustain | current_patch.osc2_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc3_adsr_sustain | current_patch.osc3_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);
       }

      if(input == 'r')
       {
         if(current_patch.osc1_adsr_release > 0 )
           current_patch.osc1_adsr_release = current_patch.osc2_adsr_release = current_patch.osc3_adsr_release -= 1;

         SID_control_packet.reg_addr = SID_OSC1_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc1_adsr_sustain | current_patch.osc1_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc2_adsr_sustain | current_patch.osc2_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc3_adsr_sustain | current_patch.osc3_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);
 
       }

      if(input == 'R')
       {
         if(current_patch.osc1_adsr_release < 15 )
           current_patch.osc1_adsr_release = current_patch.osc2_adsr_release = current_patch.osc3_adsr_release += 1;

         SID_control_packet.reg_addr = SID_OSC1_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc1_adsr_sustain | current_patch.osc1_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC2_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc2_adsr_sustain | current_patch.osc2_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

         SID_control_packet.reg_addr = SID_OSC3_SUSTAIN;
         SID_control_packet.reg_data = current_patch.osc3_adsr_sustain | current_patch.osc3_adsr_release;
         send_to_QSID(sockfd, &SID_control_packet, &servaddr);

       }
 

       
   }
}

