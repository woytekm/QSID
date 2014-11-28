/*

*/

#include "common.h"
#include "defs.h"
#include "patch.h"
#include "sysex.h"
#include "SID.h"
#include "SID_writer.h"
#include "task.h"

/* message_buffer[0] = 0xF0 */
/* message_buffer[1] = sysex code */
/* message_buffer[2,3,...,buffer_len] = encoded value for sysex code */

#define SYSEX_CODE_POS 1
#define SYSEX_DATA_POS 2

uint8_t MIDI_SYSEX_OSC1_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one byte - 0 or 1 for oscillator off or on
*/
 { 
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_STATE) || (buffer_len != SYSEX_OSC1_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_OSC1_STATE_CBK: decoding %d bytes of sysex data",buffer_len-2);

  MIDI_sysex_to_data(&message_buffer[SYSEX_DATA_POS], &decoded_val, 2); 
  
  if((decoded_val == 0) || (decoded_val == 1))  
   {
    G_current_patch.osc1_on = decoded_val;
    return 1;
   }
  else 
   return 0;
 }

uint8_t MIDI_SYSEX_OSC1_FINE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one signed byte in FINE detune range
*/
 { 
  uint8_t decoded_val;
  int8_t  decoded_signed_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_FINE) || (buffer_len != SYSEX_OSC1_FINE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_OSC1_FINE_CBK: decoding %d bytes of sysex data",buffer_len-2);

  MIDI_sysex_to_data(&message_buffer[SYSEX_DATA_POS], &decoded_val, 2);

  decoded_signed_val = (int8_t)decoded_val;
  
  if((decoded_signed_val >= (DETUNE_RANGE * -1)) && (decoded_signed_val <= DETUNE_RANGE))
   {
    G_current_patch.osc1_fine = decoded_val;
    return 1;
   }
  else
   return 0;
 }

uint8_t MIDI_SYSEX_OSC1_COARSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  return 0;
 }

uint8_t MIDI_SYSEX_OSC1_PW_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint16_t from 0 to 4095
*/
 { 
  SID_msg_t SID_msg;
  uint8_t  decode_buf[2],pw_lo,pw_hi,pw_mask = 255;
  uint16_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_PW) || (buffer_len != SYSEX_OSC1_PW_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_OSC1_PW_CBK: decoding %d bytes of sysex data",buffer_len-2);

  MIDI_sysex_to_data(&message_buffer[SYSEX_DATA_POS], &decode_buf, 3);

  decoded_val = (uint16_t)decode_buf;

  if(decoded_val < PW_RANGE)
   {
    G_current_patch.osc1_pw = decoded_val;

    pw_lo = G_current_patch.osc1_pw & pw_mask;
    pw_hi = G_current_patch.osc1_pw >> 8;

    SID_msg.reg_addr = SID_OSC1_PW_LO;
    SID_msg.reg_data = pw_lo;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

    SID_msg.reg_addr = SID_OSC1_PW_HI;
    SID_msg.reg_data = pw_hi;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

    return 1;
   }
  else
   return 0;
 }

uint8_t MIDI_SYSEX_OSC1_WAVE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_RINGMOD_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_SYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_ATTACK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_DECAY_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_SUSTAIN_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_RELEASE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC1_FILTER_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_FINE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_COARSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_PW_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_WAVE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_RINGMOD_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_SYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_ATTACK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_DECAY_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_SUSTAIN_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_RELEASE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC2_FILTER_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_FINE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_COARSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_PW_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_WAVE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_RINGMOD_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_SYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_ATTACK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_DECAY_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_SUSTAIN_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_RELEASE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_OSC3_FILTER_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_FLT_MODE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_FLT_RESO_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_FLT_CUTOFF_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_FLT_EXT_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_VOLUME_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_TRANSPOSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO1_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO1_SHAPE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO1_RATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO1_DEPTH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO1_KEYSYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO1_ROUTE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO2_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO2_SHAPE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO2_RATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO2_DEPTH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO2_KEYSYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_LFO2_ROUTE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_TRANSMT_BANK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_TRANSMT_BANK_DTA_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_TRANSMT_PATCH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_TRANSMT_PATCH_DTA_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_RECV_BANK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

uint8_t MIDI_SYSEX_RECV_PATCH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t return_code = 0; 

  return return_code;
 }

