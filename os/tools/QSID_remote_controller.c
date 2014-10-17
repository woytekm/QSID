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

#include <ncurses.h>

#define PANEL_UPPER_LEFT_CORNER_X 1
#define PANEL_UPPER_LEFT_CORNER_Y 1

#define COLUMN1 2
#define COLUMN2 33
#define COLUMN3 64

#define ROW1 3
#define ROW2 4
#define ROW3 5
#define ROW4 6
#define ROW5 7

#define ROW6 10
#define ROW7 11
#define ROW8 12
#define ROW9 13

#define PANEL_WIDTH 95
#define PANEL_HEIGHT 20


void refresh_panel_data(patch_data_t *patch)
 {

   mvprintw(ROW1,COLUMN1,"1 toggle on       (%d)  ",patch->osc1_on);
   mvprintw(ROW2,COLUMN1,"! toggle waveform (%d)  ",patch->osc1_wave);
   mvprintw(ROW3,COLUMN1,"q - PW + Q        (%d)  ",patch->osc1_pw);
   mvprintw(ROW4,COLUMN1,"t - detune + T    (%d)  ",patch->osc1_detune);
   mvprintw(ROW5,COLUMN1,"4 toggle filter   (%d)  ",patch->osc1_filter_on);

   mvprintw(ROW1,COLUMN2,"2 toggle on       (%d)  ",patch->osc2_on);
   mvprintw(ROW2,COLUMN2,"@ toggle waveform (%d)  ",patch->osc2_wave);
   mvprintw(ROW3,COLUMN2,"w - PW + W        (%d)  ",patch->osc2_pw);
   mvprintw(ROW4,COLUMN2,"y - detune + Y    (%d)  ",patch->osc2_detune);
   mvprintw(ROW5,COLUMN2,"5 toggle filter   (%d)  ",patch->osc2_filter_on);

   mvprintw(ROW1,COLUMN3,"3 toggle on       (%d)  ",patch->osc3_on);
   mvprintw(ROW2,COLUMN3,"# toggle waveform (%d)  ",patch->osc3_wave);
   mvprintw(ROW3,COLUMN3,"e - PW + E        (%d)  ",patch->osc3_pw);
   mvprintw(ROW4,COLUMN3,"u - detune + U    (%d)  ",patch->osc3_detune);
   mvprintw(ROW5,COLUMN3,"6 toggle filter   (%d)  ",patch->osc3_filter_on);


   mvprintw(ROW6,COLUMN1,"c -  cutoff   + C (%d)  ",patch->filter_cutoff);
   mvprintw(ROW7,COLUMN1,"v - resonance + V (%d)  ",patch->filter_reso);
   mvprintw(ROW8,COLUMN1,"m toggle mode     (%d)  ",patch->filter_mode);

   mvprintw(ROW6,COLUMN2,"a -  attack  + A (%d)  ",patch->osc1_adsr_attack);
   mvprintw(ROW7,COLUMN2,"d -  decay   + D (%d)  ",patch->osc1_adsr_decay);
   mvprintw(ROW8,COLUMN2,"s -  sustain + S (%d)  ",patch->osc1_adsr_sustain);
   mvprintw(ROW9,COLUMN2,"r -  release + R (%d)  ",patch->osc1_adsr_release);

   mvprintw(ROW6,COLUMN3,"< - main volume + > (%d)  ",patch->volume);

   refresh();

 }


void draw_panel()
 {
  mvprintw(PANEL_UPPER_LEFT_CORNER_Y, PANEL_UPPER_LEFT_CORNER_X,
           "==== QSID remote controller =================================================================");
  mvprintw(PANEL_UPPER_LEFT_CORNER_Y+1, PANEL_UPPER_LEFT_CORNER_X,
           "====== OSC1 ============================ OSC2 ======================== OSC3 =================");
  mvprintw(PANEL_UPPER_LEFT_CORNER_Y+7, PANEL_UPPER_LEFT_CORNER_X,
           "=============================================================================================");
  mvprintw(PANEL_UPPER_LEFT_CORNER_Y+8, PANEL_UPPER_LEFT_CORNER_X,
           "===== Filter ======================== AMP ADSR ====================== Volume ================");
  mvprintw(PANEL_UPPER_LEFT_CORNER_Y+14, PANEL_UPPER_LEFT_CORNER_X,
           "=============================================================================================");
  mvprintw(PANEL_UPPER_LEFT_CORNER_Y+15, PANEL_UPPER_LEFT_CORNER_X,
           "========================================= d - load patch === z - save patch === x - quit ====");
  refresh();
 }




void send_to_QSID(int sock, SID_msg_t *packet, struct sockaddr_in *srv_addr)
 {

      if(sendto(sock,packet,sizeof(SID_msg_t),0,
             (struct sockaddr *)srv_addr,sizeof(struct sockaddr_in)) == -1)
         printf("sendto error: %d\n\r", errno);

 }


void init_patch(patch_data_t *blank_patch)
 {

   blank_patch->osc1_adsr_attack = 0; //(upper 4 bits)
   blank_patch->osc1_adsr_decay = 0; //(lower 4 bits)
   blank_patch->osc1_adsr_sustain = 0; //(upper 4 bits)
   blank_patch->osc1_adsr_release = 0; //(lower 4 bits) 

   blank_patch->osc2_adsr_attack = 0; //(upper 4 bits)
   blank_patch->osc2_adsr_decay = 0; //(lower 4 bits)
   blank_patch->osc2_adsr_sustain = 0; //(upper 4 bits)
   blank_patch->osc2_adsr_release = 0; //(lower 4 bits)

   blank_patch->osc3_adsr_attack = 0; //(upper 4 bits)
   blank_patch->osc3_adsr_decay = 0; //(lower 4 bits)
   blank_patch->osc3_adsr_sustain = 0; //(upper 4 bits)
   blank_patch->osc3_adsr_release = 0; //(lower 4 bits)

   blank_patch->osc1_wave = WAVEFORM_TRIANGLE;
   blank_patch->osc2_wave = WAVEFORM_TRIANGLE;
   blank_patch->osc3_wave = WAVEFORM_TRIANGLE;

   blank_patch->osc1_pw = 2048;
   blank_patch->osc2_pw = 2048;
   blank_patch->osc3_pw = 2048;

   blank_patch->osc1_filter_on = 1;
   blank_patch->osc2_filter_on = 1;
   blank_patch->osc3_filter_on = 1;
 
   blank_patch->osc1_on = 1;
   blank_patch->osc2_on = 1;
   blank_patch->osc3_on = 1; 
  
   blank_patch->filter_mode = FILTER_LOWPASS;

   blank_patch->volume = 14;

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



void update_control_indexes(patch_data_t *patch, uint8_t *osc1_wave_idx, uint8_t *osc2_wave_idx, uint8_t *osc3_wave_idx, uint8_t *filter_idx)
 {

  switch (patch->osc1_wave)
   {
    case WAVEFORM_NOISE:
     *osc1_wave_idx = 0;
     break;

    case WAVEFORM_PULSE:
     *osc1_wave_idx = 1;
     break;

    case WAVEFORM_SAWTOOTH:
     *osc1_wave_idx = 2;
     break;

    case WAVEFORM_TRIANGLE:
     *osc1_wave_idx = 3;
     break;

   }

  switch (patch->osc2_wave)
   {
    case WAVEFORM_NOISE:
     *osc2_wave_idx = 0;
     break;

    case WAVEFORM_PULSE:
     *osc2_wave_idx = 1;
     break;

    case WAVEFORM_SAWTOOTH:
     *osc2_wave_idx = 2;
     break;

    case WAVEFORM_TRIANGLE:
     *osc2_wave_idx = 3;
     break;
   }

  switch (patch->osc3_wave)
   {
    case WAVEFORM_NOISE:
     *osc3_wave_idx = 0;
     break;

    case WAVEFORM_PULSE:
     *osc3_wave_idx = 1;
     break;

    case WAVEFORM_SAWTOOTH:
     *osc3_wave_idx = 2;
     break;

    case WAVEFORM_TRIANGLE:
     *osc3_wave_idx = 3;
     break;
   }

  switch (patch->filter_mode)
   {
    case FILTER_OFF:
     *filter_idx = 0;
     break;

    case FILTER_HIGHPASS:
     *filter_idx = 1;
     break;

    case FILTER_BANDPASS:
     *filter_idx = 2;
     break;

    case FILTER_LOWPASS:
     *filter_idx = 3;
     break;
   }

 }


int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   SID_msg_t SID_control_packet;
   uint8_t i, input, cutoff_lo, cutoff_hi, res_filt_reg, mode_vol_reg;
   uint8_t waveforms[4] = { WAVEFORM_NOISE, WAVEFORM_PULSE, WAVEFORM_SAWTOOTH, WAVEFORM_TRIANGLE };
   uint8_t filter_modes[4] = { FILTER_OFF, FILTER_HIGHPASS, FILTER_BANDPASS, FILTER_LOWPASS };
   patch_data_t current_patch;
   uint8_t osc1_waveform_idx,  osc2_waveform_idx, osc3_waveform_idx, filter_mode_idx;
   uint16_t row, col;

   initscr();

   getmaxyx(stdscr,row,col);

   if((col < PANEL_WIDTH) || (row < PANEL_HEIGHT))
    {
     endwin();
     printf("error - terminal to small\n");
     exit(-1);
    }


   filter_mode_idx = 0;

   if (argc != 2)
   {
      endwin();
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

   init_patch(&current_patch);

   update_control_indexes(&current_patch, &osc1_waveform_idx, &osc2_waveform_idx, &osc3_waveform_idx, &filter_mode_idx);

   draw_panel();

   refresh_panel_data(&current_patch);

   while (1)
   {
      input = getch();
      
      if(input == 'x')
        {
         endwin();
         exit(0);
         break;
        }

      if(input == '1')
       {
         
        
       }

      if(input == 'c')
       {
        if(current_patch.filter_cutoff > 100)
         {
          current_patch.filter_cutoff -= 100;
          cutoff_lo = current_patch.filter_cutoff & 7; // lowest three bits
          cutoff_hi = current_patch.filter_cutoff  >> 3;  // shift out three bits

          SID_control_packet.reg_addr = SID_FLT_CUTOFF_LO;
          SID_control_packet.reg_data = cutoff_lo;
          send_to_QSID(sockfd, &SID_control_packet, &servaddr);

          SID_control_packet.reg_addr = SID_FLT_CUTOFF_HI;
          SID_control_packet.reg_data = cutoff_hi;
          send_to_QSID(sockfd, &SID_control_packet, &servaddr);
         } 
       }

      if(input == 'C')
       {
        if(current_patch.filter_cutoff < 3900)
         {
          current_patch.filter_cutoff += 100;
          cutoff_lo = current_patch.filter_cutoff & 7; // lowest three bits
          cutoff_hi = current_patch.filter_cutoff  >> 3;  // shift out three bits

          SID_control_packet.reg_addr = SID_FLT_CUTOFF_LO;
          SID_control_packet.reg_data = cutoff_lo;
          send_to_QSID(sockfd, &SID_control_packet, &servaddr);

          SID_control_packet.reg_addr = SID_FLT_CUTOFF_HI;
          SID_control_packet.reg_data = cutoff_hi;
          send_to_QSID(sockfd, &SID_control_packet, &servaddr);
         }
       }


    if(input == 'v')
     {
      if(current_patch.filter_reso > 0)
       {
        current_patch.filter_reso--;
        res_filt_reg = current_patch.filter_reso << 4;

        if(current_patch.osc1_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC1;

        if(current_patch.osc2_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC2;

        if(current_patch.osc3_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC3;

        if(current_patch.filter_ext)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_EXT;

        SID_control_packet.reg_addr = SID_FLT_RESO_ROUTE;
        SID_control_packet.reg_data = res_filt_reg;
        send_to_QSID(sockfd, &SID_control_packet, &servaddr);
       }
     }

    if(input == 'V')
     {
      if(current_patch.filter_reso < 14)
       {
        current_patch.filter_reso--;
        res_filt_reg = current_patch.filter_reso << 4;

        if(current_patch.osc1_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC1;

        if(current_patch.osc2_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC2;

        if(current_patch.osc3_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC3;

        if(current_patch.filter_ext)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_EXT;

        SID_control_packet.reg_addr = SID_FLT_RESO_ROUTE;
        SID_control_packet.reg_data = res_filt_reg;
        send_to_QSID(sockfd, &SID_control_packet, &servaddr);
       }
     }

   if(input == '4')
    {
        if(current_patch.osc1_filter_on)
          current_patch.osc1_filter_on = 0;
        else current_patch.osc1_filter_on = 1;

        res_filt_reg = current_patch.filter_reso << 4;

        if(current_patch.osc1_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC1;

        if(current_patch.osc2_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC2;

        if(current_patch.osc3_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC3;

        if(current_patch.filter_ext)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_EXT;

        SID_control_packet.reg_addr = SID_FLT_RESO_ROUTE;
        SID_control_packet.reg_data = res_filt_reg;
        send_to_QSID(sockfd, &SID_control_packet, &servaddr);
    }

   if(input == '5')
    {
        if(current_patch.osc2_filter_on)
          current_patch.osc2_filter_on = 0;
        else current_patch.osc2_filter_on = 1;
    
        res_filt_reg = current_patch.filter_reso << 4;

        if(current_patch.osc1_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC1;

        if(current_patch.osc2_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC2;

        if(current_patch.osc3_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC3;

        if(current_patch.filter_ext)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_EXT;

        SID_control_packet.reg_addr = SID_FLT_RESO_ROUTE;
        SID_control_packet.reg_data = res_filt_reg;
        send_to_QSID(sockfd, &SID_control_packet, &servaddr);
    }

   if(input == '6')
    {
        if(current_patch.osc3_filter_on)
          current_patch.osc3_filter_on = 0;
        else current_patch.osc3_filter_on = 1;
    
        res_filt_reg = current_patch.filter_reso << 4;

        if(current_patch.osc1_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC1;

        if(current_patch.osc2_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC2;

        if(current_patch.osc3_filter_on)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_OSC3;

        if(current_patch.filter_ext)
          res_filt_reg = res_filt_reg | FILTER_ROUTING_EXT;

        SID_control_packet.reg_addr = SID_FLT_RESO_ROUTE;
        SID_control_packet.reg_data = res_filt_reg;
        send_to_QSID(sockfd, &SID_control_packet, &servaddr);
    }

  if(input == 'm')
   {

    if(filter_mode_idx == 3)
     filter_mode_idx = 0;
    else filter_mode_idx++;

    current_patch.filter_mode = filter_modes[filter_mode_idx];

    mode_vol_reg = current_patch.volume; //<< 4;

    switch(current_patch.filter_mode)
     {

      case FILTER_OFF:
        mode_vol_reg = mode_vol_reg | FILTER_OFF;
        break;

      case FILTER_HIGHPASS:
        mode_vol_reg = mode_vol_reg | FILTER_HIGHPASS;
        break;

      case FILTER_BANDPASS:
        mode_vol_reg = mode_vol_reg | FILTER_BANDPASS;
        break;

      case FILTER_LOWPASS:
        mode_vol_reg = mode_vol_reg | FILTER_LOWPASS;
        break;

     }

    SID_control_packet.reg_addr = SID_FLT_MODE_VOL;
    SID_control_packet.reg_data = mode_vol_reg;
    send_to_QSID(sockfd, &SID_control_packet, &servaddr);

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

     if(input == 'l')
      {
       open_patch(&current_patch); 
       remote_apply_patch(&current_patch, sockfd, &servaddr);
       update_control_indexes(&current_patch, &osc1_waveform_idx, &osc2_waveform_idx, &osc3_waveform_idx, &filter_mode_idx);
      }

     refresh_panel_data(&current_patch);

   }
}

