#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "SID.h"
#include "patch.h"


void LIB_apply_demo_patch(uint8_t board_address)
 {

   SID_msg_t SID_msg;

   SID_msg.SID_addr = board_address;

   SID_msg.reg_addr = SID_OSC1_ATTACK; SID_msg.reg_data = 7;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC1_SUSTAIN; SID_msg.reg_data = 16;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC2_ATTACK; SID_msg.reg_data = 7;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC2_SUSTAIN; SID_msg.reg_data = 16;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));
 
   SID_msg.reg_addr = SID_OSC3_ATTACK; SID_msg.reg_data = 7;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC3_SUSTAIN; SID_msg.reg_data = 16;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_FLT_MODE_VOL; SID_msg.reg_data = 31;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));
  
   G_current_patch.octave_transposition = -12;

   G_current_patch.osc1_detune = 0;
   G_current_patch.osc2_detune = 0;
   G_current_patch.osc3_detune = 0;

   G_current_patch.osc1_on = 1;
   G_current_patch.osc2_on = 1;
   G_current_patch.osc3_on = 1;
   

 }


void LIB_SID_note_on(uint16_t SID_osc_pitch, uint8_t board_address)
 {

   // load pitch registers (+detune value from patch), and then trigger ADSR on all three oscillators at once

   SID_msg_t SID_OSC1_msg, SID_OSC2_msg, SID_OSC3_msg;

   SID_OSC1_msg.reg_data = SID_osc_pitch + G_current_patch.osc1_detune;
   SID_OSC1_msg.reg_addr = SID_OSC1_FREQ_LO;
   SID_OSC1_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_OSC1_msg, sizeof(SID_msg_t));

   SID_OSC2_msg.reg_data = SID_osc_pitch + G_current_patch.osc2_detune;
   SID_OSC2_msg.reg_addr = SID_OSC2_FREQ_LO;
   SID_OSC2_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));

   SID_OSC3_msg.reg_data = SID_osc_pitch + G_current_patch.osc3_detune;
   SID_OSC3_msg.reg_addr = SID_OSC3_FREQ_LO;
   SID_OSC3_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_OSC3_msg, sizeof(SID_msg_t));

   SID_OSC1_msg.reg_data = G_current_patch.osc1_control_reg | 1;  // set GATE bit
   SID_OSC2_msg.reg_data = G_current_patch.osc2_control_reg | 1;  
   SID_OSC3_msg.reg_data = G_current_patch.osc3_control_reg | 1;

   SID_OSC1_msg.reg_addr = SID_OSC1_CTRL;
   SID_OSC2_msg.reg_addr = SID_OSC2_CTRL;
   SID_OSC3_msg.reg_addr = SID_OSC3_CTRL;

   
   if(G_current_patch.osc1_on) 
    write(G_SID_writer_rx_pipe[1], &SID_OSC1_msg, sizeof(SID_msg_t));
   usleep(10);

   if(G_current_patch.osc2_on)
    write(G_SID_writer_rx_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));
   usleep(10);

   if(G_current_patch.osc3_on)
    write(G_SID_writer_rx_pipe[1], &SID_OSC3_msg, sizeof(SID_msg_t));

 }


void LIB_SID_OSC1_note_on(uint16_t SID_osc_pitch, uint8_t board_address)
 {
 
   // set oscillator pitch and set GATE bit on oscillator 1

   SID_msg_t SID_msg;

   SID_msg.reg_data = SID_osc_pitch;
   SID_msg.reg_addr = SID_OSC1_FREQ_LO;
   SID_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_data = 17;
   SID_msg.reg_addr = SID_OSC1_CTRL;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }

void LIB_SID_OSC2_note_on(uint16_t SID_osc_pitch, uint8_t board_address)
 {

   // set oscillator pitch and set GATE bit on oscillator 2
 
   SID_msg_t SID_msg;

   SID_msg.reg_data = SID_osc_pitch;
   SID_msg.reg_addr = SID_OSC2_FREQ_LO;
   SID_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_data = 17;
   SID_msg.reg_addr = SID_OSC2_CTRL;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }

void LIB_SID_OSC3_note_on(uint16_t SID_osc_pitch, uint8_t board_address)
 {

   // set oscillator pitch and set GATE bit on oscillator 3

   SID_msg_t SID_msg;

   SID_msg.reg_data = SID_osc_pitch;
   SID_msg.reg_addr = SID_OSC3_FREQ_LO;
   SID_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_data = 17;
   SID_msg.reg_addr = SID_OSC3_CTRL;
   write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }


void LIB_SID_note_off(uint8_t board_address)
 {

  // clear GATE bit on all three oscillators

  SID_msg_t SID_msg;

  SID_msg.SID_addr = board_address;

  SID_msg.reg_data = G_current_patch.osc1_control_reg;
  SID_msg.reg_addr = SID_OSC1_CTRL;
  write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_data = G_current_patch.osc2_control_reg;
  SID_msg.reg_addr = SID_OSC2_CTRL;
  write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));
 
  SID_msg.reg_data = G_current_patch.osc3_control_reg;
  SID_msg.reg_addr = SID_OSC3_CTRL;
  write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }
