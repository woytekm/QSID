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


void send_to_QSID(int sock, SID_msg_t *packet, struct sockaddr_in *srv_addr)
 {

      if(sendto(sock,packet,sizeof(SID_msg_t),0,
             (struct sockaddr *)srv_addr,sizeof(struct sockaddr_in)) == -1)
         printf("sendto error: %d\n\r", errno);

 }


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

   current_patch.osc1_wave = WAVEFORM_TRIANGLE;
   current_patch.osc2_wave = WAVEFORM_TRIANGLE;
   current_patch.osc3_wave = WAVEFORM_TRIANGLE;

   current_patch.osc1_pw = 2048;
   current_patch.osc2_pw = 2048;
   current_patch.osc3_pw = 2048;

   current_patch.osc1_filter_on = 1;
   current_patch.osc2_filter_on = 1;
   current_patch.osc3_filter_on = 1;
 
   current_patch.osc1_on = 1;
   current_patch.osc2_on = 1;
   current_patch.osc3_on = 1; 
  
   current_patch.filter_mode = FILTER_LOWPASS;

 }

void remote_apply_patch(patch_data_t *new_patch, int sock, struct sockaddr_in *srv_addr)
 {
    SID_msg_t SID_control_packet;
    uint8_t pw_lo, pw_hi, cutoff_lo, cutoff_hi, res_filt_reg, control_reg, mode_vol_reg;
    
    control_reg = 0;

    pw_lo = new_patch->osc1_pw & 255;
    pw_hi = new_patch->osc1_pw >> 8; 
    
    SID_control_packet.reg_addr = SID_OSC1_PW_LO;
    SID_control_packet.reg_data = pw_lo;
    send_to_QSID(sock, &SID_control_packet, srv_addr);  

    SID_control_packet.reg_addr = SID_OSC1_PW_HI;
    SID_control_packet.reg_data = pw_hi;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC1_ATTACK;
    SID_control_packet.reg_data = new_patch->osc1_adsr_attack | new_patch->osc1_adsr_decay;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC1_SUSTAIN;
    SID_control_packet.reg_data = new_patch->osc1_adsr_sustain | new_patch->osc1_adsr_release;
    send_to_QSID(sock, &SID_control_packet, srv_addr);
   
    switch(new_patch->osc1_wave)
     {
      case WAVEFORM_NOISE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_NOISE;

      case WAVEFORM_PULSE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_PULSE;

      case WAVEFORM_SAWTOOTH:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_SAWTOOTH;

      case WAVEFORM_TRIANGLE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_TRIANGLE;
     }

    SID_control_packet.reg_addr = SID_OSC1_CTRL;
    SID_control_packet.reg_data = control_reg;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    pw_lo = new_patch->osc2_pw & 255; // lowest 8 bits
    pw_hi = new_patch->osc2_pw >> 8;  // shift the rest 

    SID_control_packet.reg_addr = SID_OSC2_PW_LO;
    SID_control_packet.reg_data = pw_lo;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC2_PW_HI;
    SID_control_packet.reg_data = pw_hi;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC2_ATTACK;
    SID_control_packet.reg_data = new_patch->osc1_adsr_attack | new_patch->osc1_adsr_decay;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC2_SUSTAIN;
    SID_control_packet.reg_data = new_patch->osc1_adsr_sustain | new_patch->osc1_adsr_release;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    switch(new_patch->osc2_wave)
     {
      case WAVEFORM_NOISE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_NOISE;

      case WAVEFORM_PULSE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_PULSE;

      case WAVEFORM_SAWTOOTH:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_SAWTOOTH;

      case WAVEFORM_TRIANGLE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_TRIANGLE;
     }

    SID_control_packet.reg_addr = SID_OSC2_CTRL;
    SID_control_packet.reg_data = control_reg;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    pw_lo = new_patch->osc3_pw & 255;
    pw_hi = new_patch->osc3_pw >> 8;

    SID_control_packet.reg_addr = SID_OSC3_PW_LO;
    SID_control_packet.reg_data = pw_lo;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC3_PW_HI;
    SID_control_packet.reg_data = pw_hi;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC3_ATTACK;
    SID_control_packet.reg_data = new_patch->osc1_adsr_attack | new_patch->osc1_adsr_decay;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_OSC3_SUSTAIN;
    SID_control_packet.reg_data = new_patch->osc1_adsr_sustain | new_patch->osc1_adsr_release;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    switch(new_patch->osc3_wave)
     {
      case WAVEFORM_NOISE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_NOISE;
        break;

      case WAVEFORM_PULSE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_PULSE;
        break;

      case WAVEFORM_SAWTOOTH:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_SAWTOOTH;
        break;

     case WAVEFORM_TRIANGLE:
        control_reg = control_reg & CLEAR_ALL_WAVEFORMS;
        control_reg = control_reg | WAVEFORM_TRIANGLE;
        break;
     }

    SID_control_packet.reg_addr = SID_OSC3_CTRL;
    SID_control_packet.reg_data = control_reg;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    cutoff_lo = new_patch->filter_cutoff & 7; // lowest three bits
    cutoff_hi = new_patch->filter_cutoff  >> 3;  // shift out three bits 

    SID_control_packet.reg_addr = SID_FLT_CUTOFF_LO;
    SID_control_packet.reg_data = cutoff_lo;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    SID_control_packet.reg_addr = SID_FLT_CUTOFF_HI;
    SID_control_packet.reg_data = cutoff_hi;
    send_to_QSID(sock, &SID_control_packet, srv_addr);    

    res_filt_reg = new_patch->filter_reso << 4;

    if(new_patch->osc1_filter_on)
     res_filt_reg = res_filt_reg & FILTER_ROUTING_OSC1;

    if(new_patch->osc2_filter_on)
     res_filt_reg = res_filt_reg & FILTER_ROUTING_OSC2;

    if(new_patch->osc3_filter_on)
     res_filt_reg = res_filt_reg & FILTER_ROUTING_OSC3;

    if(new_patch->filter_ext)
     res_filt_reg = res_filt_reg & FILTER_ROUTING_EXT;

    SID_control_packet.reg_addr = SID_FLT_RESO_ROUTE;
    SID_control_packet.reg_data = res_filt_reg;
    send_to_QSID(sock, &SID_control_packet, srv_addr);

    mode_vol_reg = new_patch->volume << 4;
    
    switch(new_patch->filter_mode)
     {
     
      case FILTER_OFF:
        mode_vol_reg = mode_vol_reg & FILTER_OFF;
        break;

      case FILTER_HIGHPASS:
        mode_vol_reg = mode_vol_reg & FILTER_HIGHPASS;
        break;

      case FILTER_BANDPASS:
        mode_vol_reg = mode_vol_reg & FILTER_BANDPASS;
        break;

      case FILTER_LOWPASS:
        mode_vol_reg = mode_vol_reg & FILTER_LOWPASS;
        break;
    
     }
  
    SID_control_packet.reg_addr = SID_FLT_MODE_VOL;
    SID_control_packet.reg_data = mode_vol_reg;
    send_to_QSID(sock, &SID_control_packet, srv_addr);   
 
 }


uint8_t save_patch(patch_data_t *new_patch)
 {
  
  FILE *patch_data;

  patch_data=fopen("qsid_patch.dat","w");
   if (!patch_data)
     {
       printf("Unable to open patch file!");
       return -1;
     }
   fwrite(new_patch, sizeof(patch_data_t), 1, patch_data);
   fclose(patch_data);

 }

uint8_t open_patch(patch_data_t *new_patch)
 {
 
  FILE *patch_data;

  patch_data=fopen("qsid_patch.dat","r");
   if (!patch_data)
     {
       printf("Unable to open patch file!");
       return -1;
     }
   fread(new_patch, sizeof(patch_data_t), 1, patch_data);
   fclose(patch_data);

 }



void draw_panel(void)
 {

  system("/usr/bin/clear");
  printf("==== QSID remote controller ========================================================== \n\r");
  printf("====== OSC1 ================ OSC2 ================ OSC3 ================ AMP ADSR ==== \n\r");
  printf("   1 toggle on           2 toggle on           3 toggle on           a -  attack  + A   \n\r");
  printf("   ! toggle waveform     @ toggle waveform     # toggle waveform     d -  decay   + D   \n\r");
  printf("   q - pulse width + Q   w - pulse width + W   e - pulse width + E   s - sustain  + S   \n\r");
  printf("   4 toggle filter       5 toggle filter       6 toggle filter       r - release  + R   \n\r");
  printf("====== Filter ================================================ Volume ================= \n\r");
  printf("   c -  cutoff   + C                                     < - main volume + >            \n\r");
  printf("   v - resonance + V                                                                    \n\r");
  printf("   m toggle mode                                                                        \n\r");
  printf("======================================================================================  \n\r");
  printf("=================================== d - load patch === z - save patch === x - quit ===  \n\r");

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
      
      if(input == 'x')
        {
         system ("/bin/stty cooked echo");
         break;
        }

      if(input == '1')
       {
         
        
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

     if(input == 'z')
       save_patch(&current_patch);

     if(input == 'd')
      {
       open_patch(&current_patch); 
       remote_apply_patch(&current_patch, sockfd, &servaddr);
      }

   }
}

