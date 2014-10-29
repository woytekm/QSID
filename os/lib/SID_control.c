#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "MIDI_to_SID.h"
#include "SID.h"
#include "patch.h"

uint16_t G_MIDI_to_SID_reg[95] = { 274, 291, 308, 325, 346, 366, 388, 411, 435, 461, 489, 518,
                         549, 581, 616, 652, 691, 732, 776, 822, 871, 923, 978, 1036,
                         1097, 1163, 1232, 1305, 1383, 1465, 1552, 1644, 1742, 1845, 1955,
                         2071, 2195, 2325, 2463, 2610, 2765, 2930, 3104, 3288, 3484, 3691,
                         3910, 4143, 4389, 4650, 4927, 5220, 5530, 5859, 6207, 6577, 6968,
                         7382, 7821, 8286, 8779, 9301, 9854, 10440, 11060, 11718, 12415,
                         13153, 13935, 14764, 15642, 16572, 17557, 18601, 19709, 20897,
                         22121, 23436, 24830, 26306, 27871, 29528, 31234, 33144, 35115,
                         37203, 39415, 41759, 44242, 46873, 49660, 52613, 55741, 59056,
                         62657 };



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


void LIB_SID_note_on(uint8_t MIDI_note, uint8_t board_address)
 {

   int8_t detune_osc1, detune_osc2, detune_osc3;

   SID_msg_t SID_OSC1_msg, SID_OSC2_msg, SID_OSC3_msg;
   
   // load pitch registers (+detune value from patch), and then trigger ADSR on all three oscillators at once

   SID_OSC1_msg.reg_data = G_MIDI_to_SID_reg[MIDI_note+G_current_patch.octave_transposition] + SYNTH_SID_oscillator_detune(MIDI_note, G_current_patch.osc1_detune);
   SID_OSC1_msg.reg_addr = SID_OSC1_FREQ_LO;
   SID_OSC1_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_OSC1_msg, sizeof(SID_msg_t));

   SID_OSC2_msg.reg_data = G_MIDI_to_SID_reg[MIDI_note+G_current_patch.octave_transposition] + SYNTH_SID_oscillator_detune(MIDI_note, G_current_patch.osc2_detune);
   SID_OSC2_msg.reg_addr = SID_OSC2_FREQ_LO;
   SID_OSC2_msg.SID_addr = board_address;

   write(G_SID_writer_rx_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));

   SID_OSC3_msg.reg_data = G_MIDI_to_SID_reg[MIDI_note+G_current_patch.octave_transposition] + SYNTH_SID_oscillator_detune(MIDI_note, G_current_patch.osc3_detune);
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

   if(G_current_patch.osc2_on)
    write(G_SID_writer_rx_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));

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
