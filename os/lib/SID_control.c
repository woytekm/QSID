#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "SID.h"


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

  SID_msg.reg_data = 16;
  SID_msg.SID_addr = board_address;

  SID_msg.reg_addr = SID_OSC1_CTRL;
  write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC2_CTRL;
  write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC3_CTRL;
  write(G_SID_writer_rx_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }
