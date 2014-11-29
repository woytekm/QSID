/*

*/

#include "common.h"
#include "defs.h"
#include "patch.h"
#include "sysex.h"
#include "SID.h"
#include "SID_writer.h"
#include "task.h"
#include "inventory.h"

/* message_buffer[0] = 0xF0 */
/* message_buffer[1] = manufacturer's ID */
/* message_buffer[2] = sysex code */
/* message_buffer[3] = sysex subcode */
/*
/* message_buffer[4,5,...,buffer_len] = encoded value for sysex subcode */

#define SYSEX_CODE_POS 2
#define SYSEX_DATA_POS 3
#define SYSEX_CONTROL_BYTES 5

uint8_t MIDI_SYSEX_OSC1_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one byte - 0 or 1 for oscillator off or on
*/
 { 
  uint8_t decoded_buffer[SYSEX_OSC1_STATE_LEN];

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_STATE) || (buffer_len != SYSEX_OSC1_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_OSC1_STATE_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0; 
  
  if((decoded_buffer[SYSEX_DATA_POS] == 0) || (decoded_buffer[SYSEX_DATA_POS] == 1))  
   {
    G_current_patch.osc1_on = decoded_buffer[SYSEX_DATA_POS];
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
  uint8_t decoded_buffer[SYSEX_OSC1_FINE_LEN];
  int8_t  decoded_signed_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_FINE) || (buffer_len != SYSEX_OSC1_FINE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_OSC1_FINE_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  if(!MIDI_decode(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_signed_val = (int8_t)decoded_buffer[SYSEX_DATA_POS];
  
  if((decoded_signed_val >= (DETUNE_RANGE * -1)) && (decoded_signed_val <= DETUNE_RANGE))
   {
    G_current_patch.osc1_fine = decoded_signed_val;
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
  SID_msg_t SID_msg_LSB, SID_msg_MSB;
  uint8_t  decoded_buffer[SYSEX_OSC1_PW_LEN],pw_lo,pw_hi,pw_mask = 255, voice_counter;
  uint16_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_PW) || (buffer_len != SYSEX_OSC1_PW_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_OSC1_PW_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer);

  decoded_val = (uint16_t)decoded_buffer[SYSEX_DATA_POS]; /* two bytes */

  if(decoded_val < PW_RANGE)
   {
    G_current_patch.osc1_pw = decoded_val;

    pw_lo = G_current_patch.osc1_pw & pw_mask;
    pw_hi = G_current_patch.osc1_pw >> 8;

    SID_msg_LSB.reg_addr = SID_OSC1_PW_LO;
    SID_msg_LSB.reg_data = pw_lo;

    SID_msg_MSB.reg_addr = SID_OSC1_PW_HI;
    SID_msg_MSB.reg_data = pw_hi;

    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
     {
       SID_msg_LSB.SID_addr = G_voice_inventory[voice_counter].address;
       SID_msg_MSB.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg_LSB, sizeof(SID_msg_t));
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg_MSB, sizeof(SID_msg_t));
     }
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

