/*

*/

#include "common.h"
#include "defs.h"
#include "patch.h"
#include "lfo.h"
#include "sysex.h"
#include "SID.h"
#include "SID_writer.h"
#include "task.h"
#include "inventory.h"

/* message_buffer[0] = 0xF0 */
/* message_buffer[1] = manufacturer's ID */
/* message_buffer[2] = manufacturer's ID */
/* message_buffer[3] = device ID (MIDI channel) */
/* message_buffer[4] = sysex code */
/*
/* message_buffer[5,6,...,buffer_len] = sysex subcode (optional) and encoded value for code/subcode */


/* oscillator 1 control: */

uint8_t MIDI_SYSEX_OSC1_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one byte - 0 or 1 for oscillator off or on
*/
 { 
  uint8_t decoded_buffer[SYSEX_OSC1_STATE_LEN];
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_STATE) || (buffer_len != SYSEX_OSC1_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (decoded_len = MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0)
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, decoded_len, 1);
  
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
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_FINE) || (buffer_len != SYSEX_OSC1_FINE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);
  
  if( (decoded_len = MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0)
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

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
  uint16_t *decoded_val, decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_PW) || (buffer_len != SYSEX_OSC1_PW_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (decoded_len = MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0)
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, decoded_len, 1);

  decoded_val = (uint16_t *) &decoded_buffer[SYSEX_DATA_POS]; /* two bytes */

  if(*decoded_val < PW_RANGE)
   {
    G_current_patch.osc1_pw = *decoded_val;

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
   {
    SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: invalid message value (%d)",*decoded_val);
    return 0;
   }
 }

uint8_t MIDI_SYSEX_OSC1_WAVE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t enum WAVEFORM_NOISE || WAVEFORM_PULSE || WAVEFORM_SAWTOOTH || WAVEFORM_TRIANGLE
*/
 { 
  uint8_t decoded_buffer[SYSEX_OSC1_WAVE_LEN];
  uint8_t decoded_val;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_WAVE) || (buffer_len != SYSEX_OSC1_WAVE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if((decoded_len = MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0);
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == WAVEFORM_NOISE) || (decoded_val == WAVEFORM_PULSE) || 
     (decoded_val == WAVEFORM_SAWTOOTH) || (decoded_val == WAVEFORM_TRIANGLE))
   {
    G_current_patch.osc1_wave = decoded_val;
    G_current_patch.osc1_control_reg &=  CLEAR_ALL_WAVEFORMS;
    G_current_patch.osc1_control_reg |= decoded_val;
    return 1;
   }
  else return 0;

 }

uint8_t MIDI_SYSEX_OSC1_RINGMOD_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t RINGMOD or RINGMOD_CLEAR for ring modulator on or off
*/
 { 
  uint8_t decoded_buffer[SYSEX_OSC1_RINGMOD_LEN];
  uint8_t decoded_val;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_RINGMOD) || (buffer_len != SYSEX_OSC1_RINGMOD_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (decoded_len = MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0)
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

 decoded_val = decoded_buffer[SYSEX_DATA_POS];

 if(decoded_val == RINGMOD)
   {
     G_current_patch.osc1_ringmod_on = decoded_val;
     G_current_patch.osc1_control_reg |= decoded_val;
     return 1;
   }
 else if(decoded_val == RINGMOD_CLEAR)
   {
     G_current_patch.osc1_ringmod_on = decoded_val;
     G_current_patch.osc1_control_reg &= decoded_val;
     return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC1_SYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t OSC_SYNC or OSC_SYNC_CLEAR for oscillator sync on or off
*/
 { 
  uint8_t decoded_buffer[SYSEX_OSC1_SYNC_LEN];
  uint8_t decoded_val;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_SYNC) || (buffer_len != SYSEX_OSC1_SYNC_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0 )
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val == OSC_SYNC)
   {
     G_current_patch.osc1_sync_on = decoded_val;
     G_current_patch.osc1_control_reg |= decoded_val;
    return 1;
   }
  else if(decoded_val == OSC_SYNC_CLEAR)
   {
     G_current_patch.osc1_sync_on = decoded_val;
     G_current_patch.osc1_control_reg &= decoded_val;
     return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC1_ATTACK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t 0 - 15 ADSR attack
*/
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC1_ATTACK_LEN];
  uint8_t decoded_val, voice_counter;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_ATTACK) || (buffer_len != SYSEX_OSC1_ATTACK_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);
 
  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0 )
   return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc1_adsr_attack = decoded_val;
    SID_msg.reg_addr = SID_OSC1_ATTACK;
    SID_msg.reg_data = (decoded_val << 4) | G_current_patch.osc1_adsr_decay;  
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
        SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
        write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC1_DECAY_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t 0 - 15 ADSR decay
*/
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC1_DECAY_LEN];
  uint8_t decoded_val, voice_counter;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_DECAY) || (buffer_len != SYSEX_OSC1_DECAY_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0 )
     return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc1_adsr_decay = decoded_val;
    SID_msg.reg_addr = SID_OSC1_ATTACK;  /* attack + decay are in the same register */
    SID_msg.reg_data = (G_current_patch.osc1_adsr_attack << 4) | decoded_val;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
        SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
        write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC1_SUSTAIN_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t 0 - 15 ADSR sustain
*/
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC1_SUSTAIN_LEN];
  uint8_t decoded_val, voice_counter;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_SUSTAIN) || (buffer_len != SYSEX_OSC1_SUSTAIN_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0 )
     return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc1_adsr_sustain = decoded_val;
    SID_msg.reg_addr = SID_OSC1_SUSTAIN;
    SID_msg.reg_data = (decoded_val << 4) | G_current_patch.osc1_adsr_release;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
     {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
     }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC1_RELEASE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/* 
 valid data: uint8_t 0 - 15 ADSR release
*/
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC1_RELEASE_LEN];
  uint8_t decoded_val, voice_counter;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_RELEASE) || (buffer_len != SYSEX_OSC1_RELEASE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0 )
     return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc1_adsr_release = decoded_val;
    SID_msg.reg_addr = SID_OSC1_SUSTAIN;  /* sustain + release are in the same register */
    SID_msg.reg_data = (G_current_patch.osc1_adsr_sustain << 4) | decoded_val;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC1_FILTER_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint8_t FILTER_ROUTING_OSC1 or 0 for filter on or off
*/
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC1_FILTER_LEN];
  uint8_t decoded_val, voice_counter;
  uint8_t apply_val = 0;
  uint16_t decoded_len;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC1_FILTER) || (buffer_len != SYSEX_OSC1_FILTER_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  MIDI_debug_dump_sysex(message_buffer, buffer_len, 0);

  if( (MIDI_decode_sysex(message_buffer, buffer_len, &decoded_buffer)) == 0 )
     return 0;

  MIDI_debug_dump_sysex(decoded_buffer, buffer_len, 1);

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val == 0)
    {
     G_current_patch.osc1_filter_on = decoded_val;
     apply_val = (G_current_patch.filter_reso << 4) | decoded_val | 
                 G_current_patch.osc2_filter_on | G_current_patch.osc3_filter_on | G_current_patch.filter_ext;
    }
  else if(decoded_val == FILTER_ROUTING_OSC1)
   {
    G_current_patch.osc1_filter_on = decoded_val;
    apply_val = (G_current_patch.filter_reso << 4) | decoded_val |
                G_current_patch.osc2_filter_on | G_current_patch.osc3_filter_on | G_current_patch.filter_ext;
   }
  else return 0;

  SID_msg.reg_addr = SID_FLT_RESO_ROUTE;
  SID_msg.reg_data = apply_val;
  for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
   {
    SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
   }
  return 1;
 }


/* oscillator 2 control */

uint8_t MIDI_SYSEX_OSC2_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one byte - 0 or 1 for oscillator off or on
*/
 {
  uint8_t decoded_buffer[SYSEX_OSC2_STATE_LEN];

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_STATE) || (buffer_len != SYSEX_OSC2_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  if((decoded_buffer[SYSEX_DATA_POS] == 0) || (decoded_buffer[SYSEX_DATA_POS] == 1))
   {
    G_current_patch.osc2_on = decoded_buffer[SYSEX_DATA_POS];
    return 1;
   }
  else
   return 0;
 }

uint8_t MIDI_SYSEX_OSC2_FINE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one signed byte in FINE detune range
*/
 {
  uint8_t decoded_buffer[SYSEX_OSC2_FINE_LEN];
  int8_t  decoded_signed_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_FINE) || (buffer_len != SYSEX_OSC2_FINE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_signed_val = (int8_t)decoded_buffer[SYSEX_DATA_POS];

  if((decoded_signed_val >= (DETUNE_RANGE * -1)) && (decoded_signed_val <= DETUNE_RANGE))
   {
    G_current_patch.osc2_fine = decoded_signed_val;
    return 1;
   }
  else
   return 0;
 }

uint8_t MIDI_SYSEX_OSC2_COARSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  return 0;
 }

uint8_t MIDI_SYSEX_OSC2_PW_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint16_t from 0 to 4095
*/
 {
  SID_msg_t SID_msg_LSB, SID_msg_MSB;
  uint8_t  decoded_buffer[SYSEX_OSC2_PW_LEN],pw_lo,pw_hi,pw_mask = 255, voice_counter;
  uint16_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_PW) || (buffer_len != SYSEX_OSC2_PW_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer);

  decoded_val = (uint16_t)decoded_buffer[SYSEX_DATA_POS]; /* two bytes */

  if(decoded_val < PW_RANGE)
   {
    G_current_patch.osc2_pw = decoded_val;

    pw_lo = G_current_patch.osc1_pw & pw_mask;
    pw_hi = G_current_patch.osc1_pw >> 8;

    SID_msg_LSB.reg_addr = SID_OSC2_PW_LO;
    SID_msg_LSB.reg_data = pw_lo;

    SID_msg_MSB.reg_addr = SID_OSC2_PW_HI;
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

uint8_t MIDI_SYSEX_OSC2_WAVE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  uint8_t decoded_buffer[SYSEX_OSC2_WAVE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_WAVE) || (buffer_len != SYSEX_OSC2_WAVE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == WAVEFORM_NOISE) || (decoded_val == WAVEFORM_PULSE) ||
     (decoded_val == WAVEFORM_SAWTOOTH) || (decoded_val == WAVEFORM_PULSE))
   {
    G_current_patch.osc2_wave = decoded_val;
    G_current_patch.osc2_control_reg &=  CLEAR_ALL_WAVEFORMS;
    G_current_patch.osc2_control_reg |= decoded_val;
    return 1;
   }
  else return 0;

 }

uint8_t MIDI_SYSEX_OSC2_RINGMOD_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  uint8_t decoded_buffer[SYSEX_OSC2_RINGMOD_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_RINGMOD) || (buffer_len != SYSEX_OSC2_RINGMOD_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

 decoded_val = decoded_buffer[SYSEX_DATA_POS];

 if(decoded_val == RINGMOD)
   {
     G_current_patch.osc2_ringmod_on = decoded_val;
     G_current_patch.osc2_control_reg |= decoded_val;
     return 1;
   }
 else if(decoded_val == RINGMOD_CLEAR)
   {
     G_current_patch.osc2_ringmod_on = decoded_val;
     G_current_patch.osc2_control_reg &= decoded_val;
     return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC2_SYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  uint8_t decoded_buffer[SYSEX_OSC2_SYNC_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_SYNC) || (buffer_len != SYSEX_OSC2_SYNC_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

 decoded_val = decoded_buffer[SYSEX_DATA_POS];

 if(decoded_val == OSC_SYNC)
   {
     G_current_patch.osc2_sync_on = decoded_val;
     G_current_patch.osc2_control_reg |= decoded_val;
    return 1;
   }
 else if(decoded_val == OSC_SYNC_CLEAR)
   {
     G_current_patch.osc2_sync_on = decoded_val;
     G_current_patch.osc2_control_reg &= decoded_val;
     return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC2_ATTACK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC2_ATTACK_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_ATTACK) || (buffer_len != SYSEX_OSC2_ATTACK_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc2_adsr_attack = decoded_val;
    SID_msg.reg_addr = SID_OSC2_ATTACK;
    SID_msg.reg_data = (decoded_val << 4) | G_current_patch.osc2_adsr_decay;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
        SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
        write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC2_DECAY_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC2_DECAY_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_DECAY) || (buffer_len != SYSEX_OSC2_DECAY_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc2_adsr_decay = decoded_val;
    SID_msg.reg_addr = SID_OSC2_ATTACK;  /* attack + decay are in the same register */
    SID_msg.reg_data = (G_current_patch.osc2_adsr_attack << 4) | decoded_val;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
        SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
        write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC2_SUSTAIN_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC2_SUSTAIN_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_SUSTAIN) || (buffer_len != SYSEX_OSC2_SUSTAIN_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc2_adsr_sustain = decoded_val;
    SID_msg.reg_addr = SID_OSC2_SUSTAIN;
    SID_msg.reg_data = (decoded_val << 4) | G_current_patch.osc2_adsr_release;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
     {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
     }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC2_RELEASE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC2_RELEASE_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_RELEASE) || (buffer_len != SYSEX_OSC2_RELEASE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc2_adsr_release = decoded_val;
    SID_msg.reg_addr = SID_OSC2_SUSTAIN;  /* sustain + release are in the same register */
    SID_msg.reg_data = (G_current_patch.osc2_adsr_sustain << 4) | decoded_val;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC2_FILTER_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC2_FILTER_LEN];
  uint8_t decoded_val, voice_counter;
  uint8_t apply_val = 0;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC2_FILTER) || (buffer_len != SYSEX_OSC2_FILTER_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val == 0)
    {
     G_current_patch.osc2_filter_on = decoded_val;
     apply_val = (G_current_patch.filter_reso << 4) | G_current_patch.osc1_filter_on |
                 decoded_val | G_current_patch.osc3_filter_on | G_current_patch.filter_ext;
    }
  else if(decoded_val == FILTER_ROUTING_OSC2)
   {
    G_current_patch.osc2_filter_on = decoded_val;
    apply_val = (G_current_patch.filter_reso << 4) | G_current_patch.osc1_filter_on |
                decoded_val | G_current_patch.osc3_filter_on | G_current_patch.filter_ext;
   }
  else return 0;

  SID_msg.reg_addr = SID_FLT_RESO_ROUTE;
  SID_msg.reg_data = apply_val;
  for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
   {
    SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
   }
  return 1;
 }


/* oscillator 3 control: */

uint8_t MIDI_SYSEX_OSC3_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one byte - 0 or 1 for oscillator off or on
*/
 {
  uint8_t decoded_buffer[SYSEX_OSC3_STATE_LEN];

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_STATE) || (buffer_len != SYSEX_OSC3_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  if((decoded_buffer[SYSEX_DATA_POS] == 0) || (decoded_buffer[SYSEX_DATA_POS] == 1))
   {
    G_current_patch.osc3_on = decoded_buffer[SYSEX_DATA_POS];
    return 1;
   }
  else
   return 0;
 }

uint8_t MIDI_SYSEX_OSC3_FINE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: one signed byte in FINE detune range
*/
 {
  uint8_t decoded_buffer[SYSEX_OSC3_FINE_LEN];
  int8_t  decoded_signed_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_FINE) || (buffer_len != SYSEX_OSC3_FINE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_signed_val = (int8_t)decoded_buffer[SYSEX_DATA_POS];

  if((decoded_signed_val >= (DETUNE_RANGE * -1)) && (decoded_signed_val <= DETUNE_RANGE))
   {
    G_current_patch.osc3_fine = decoded_signed_val;
    return 1;
   }
  else
   return 0;
 }

uint8_t MIDI_SYSEX_OSC3_COARSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  return 0;
 }

uint8_t MIDI_SYSEX_OSC3_PW_CBK(unsigned char *message_buffer, uint16_t buffer_len)
/*
 valid data: uint16_t from 0 to 4095
*/
 {
  SID_msg_t SID_msg_LSB, SID_msg_MSB;
  uint8_t  decoded_buffer[SYSEX_OSC3_PW_LEN],pw_lo,pw_hi,pw_mask = 255, voice_counter;
  uint16_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_PW) || (buffer_len != SYSEX_OSC3_PW_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer);

  decoded_val = (uint16_t)decoded_buffer[SYSEX_DATA_POS]; /* two bytes */

  if(decoded_val < PW_RANGE)
   {
    G_current_patch.osc3_pw = decoded_val;

    pw_lo = G_current_patch.osc3_pw & pw_mask;
    pw_hi = G_current_patch.osc3_pw >> 8;

    SID_msg_LSB.reg_addr = SID_OSC3_PW_LO;
    SID_msg_LSB.reg_data = pw_lo;

    SID_msg_MSB.reg_addr = SID_OSC3_PW_HI;
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

uint8_t MIDI_SYSEX_OSC3_WAVE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  uint8_t decoded_buffer[SYSEX_OSC3_WAVE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_WAVE) || (buffer_len != SYSEX_OSC3_WAVE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == WAVEFORM_NOISE) || (decoded_val == WAVEFORM_PULSE) ||
     (decoded_val == WAVEFORM_SAWTOOTH) || (decoded_val == WAVEFORM_PULSE))
   {
    G_current_patch.osc3_wave = decoded_val;
    G_current_patch.osc3_control_reg &=  CLEAR_ALL_WAVEFORMS;
    G_current_patch.osc3_control_reg |= decoded_val;
    return 1;
   }
  else return 0;

 }

uint8_t MIDI_SYSEX_OSC3_RINGMOD_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  uint8_t decoded_buffer[SYSEX_OSC3_RINGMOD_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_RINGMOD) || (buffer_len != SYSEX_OSC3_RINGMOD_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

 decoded_val = decoded_buffer[SYSEX_DATA_POS];

 if(decoded_val == RINGMOD)
   {
     G_current_patch.osc3_ringmod_on = decoded_val;
     G_current_patch.osc3_control_reg |= decoded_val;
     return 1;
   }
 else if(decoded_val == RINGMOD_CLEAR)
   {
     G_current_patch.osc3_ringmod_on = decoded_val;
     G_current_patch.osc3_control_reg &= decoded_val;
     return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC3_SYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  uint8_t decoded_buffer[SYSEX_OSC3_SYNC_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_SYNC) || (buffer_len != SYSEX_OSC3_SYNC_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

 decoded_val = decoded_buffer[SYSEX_DATA_POS];

 if(decoded_val == OSC_SYNC)
   {
     G_current_patch.osc3_sync_on = decoded_val;
     G_current_patch.osc3_control_reg |= decoded_val;
    return 1;
   }
 else if(decoded_val == OSC_SYNC_CLEAR)
   {
     G_current_patch.osc3_sync_on = decoded_val;
     G_current_patch.osc3_control_reg &= decoded_val;
     return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC3_ATTACK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC3_ATTACK_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_ATTACK) || (buffer_len != SYSEX_OSC3_ATTACK_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc3_adsr_attack = decoded_val;
    SID_msg.reg_addr = SID_OSC3_ATTACK;
    SID_msg.reg_data = (decoded_val << 4) | G_current_patch.osc3_adsr_decay;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
        SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
        write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC3_DECAY_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC3_DECAY_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_DECAY) || (buffer_len != SYSEX_OSC3_DECAY_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc3_adsr_decay = decoded_val;
    SID_msg.reg_addr = SID_OSC3_ATTACK;  /* attack + decay are in the same register */
    SID_msg.reg_data = (G_current_patch.osc3_adsr_attack << 4) | decoded_val;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
        SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
        write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC3_SUSTAIN_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC3_SUSTAIN_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_SUSTAIN) || (buffer_len != SYSEX_OSC3_SUSTAIN_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc3_adsr_sustain = decoded_val;
    SID_msg.reg_addr = SID_OSC3_SUSTAIN;
    SID_msg.reg_data = (decoded_val << 4) | G_current_patch.osc3_adsr_release;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
     {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
     }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC3_RELEASE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC3_RELEASE_LEN];
  uint8_t decoded_val, voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_RELEASE) || (buffer_len != SYSEX_OSC3_RELEASE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
   {
    G_current_patch.osc3_adsr_release = decoded_val;
    SID_msg.reg_addr = SID_OSC3_SUSTAIN;  /* sustain + release are in the same register */
    SID_msg.reg_data = (G_current_patch.osc3_adsr_sustain << 4) | decoded_val;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }
    return 1;
   }
  else return 0;
 }

uint8_t MIDI_SYSEX_OSC3_FILTER_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 {
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC3_FILTER_LEN];
  uint8_t decoded_val,voice_counter;
  uint8_t apply_val = 0;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_FILTER) || (buffer_len != SYSEX_OSC3_FILTER_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val == 0)
    {
     G_current_patch.osc3_filter_on = decoded_val;
     apply_val = (G_current_patch.filter_reso << 4) | G_current_patch.osc1_filter_on |
                 G_current_patch.osc2_filter_on | decoded_val | G_current_patch.filter_ext;
    }
  else if(decoded_val == FILTER_ROUTING_OSC3)
   {
    G_current_patch.osc3_filter_on = decoded_val;
    apply_val = (G_current_patch.filter_reso << 4) | G_current_patch.osc1_filter_on |
                G_current_patch.osc2_filter_on | decoded_val | G_current_patch.filter_ext;
   }
  else return 0;

  SID_msg.reg_addr = SID_FLT_RESO_ROUTE;
  SID_msg.reg_data = apply_val;
  for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
   {
    SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
   }
  return 1;
 }


/* filter control: */

uint8_t MIDI_SYSEX_FLT_MODE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_FLT_MODE_LEN];
  uint8_t decoded_val,voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_FLT_MODE) || (buffer_len != SYSEX_FLT_MODE_LEN))
    return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == FILTER_OFF) || (decoded_val == FILTER_HIGHPASS) || (decoded_val == FILTER_BANDPASS) ||
     (decoded_val == FILTER_LOWPASS))
   {
     G_current_patch.filter_mode = decoded_val;
     SID_msg.reg_addr = SID_FLT_MODE_VOL;
     SID_msg.reg_data = decoded_val | G_current_patch.volume;
     for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }   
    
   }
  else return 0;

 }

uint8_t MIDI_SYSEX_FLT_RESO_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_FLT_RESO_LEN];
  uint8_t decoded_val,voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_FLT_RESO) || (buffer_len != SYSEX_FLT_RESO_LEN))
    return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15)
   {
    G_current_patch.filter_reso = decoded_val;
    SID_msg.reg_addr = SID_FLT_RESO_ROUTE;
    SID_msg.reg_data = decoded_val | G_current_patch.osc1_filter_on | G_current_patch.osc2_filter_on |
                       G_current_patch.osc3_filter_on | G_current_patch.filter_ext;
    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
     {
      SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
      write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
     }

   }
  else return 0;

 }

uint8_t MIDI_SYSEX_FLT_CUTOFF_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  SID_msg_t SID_msg_LSB, SID_msg_MSB;
  uint8_t decoded_buffer[SYSEX_FLT_CUTOFF_LEN], cutoff_lo, cutoff_hi, c_mask=255;
  uint16_t decoded_val,voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_FLT_CUTOFF) || (buffer_len != SYSEX_FLT_CUTOFF_LEN))
    return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = (uint16_t)decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= CUTOFF_RANGE)
   {
    G_current_patch.filter_cutoff = decoded_val;
    cutoff_lo = decoded_val & c_mask;
    cutoff_hi = decoded_val >> 8;

    SID_msg_LSB.reg_addr = SID_FLT_CUTOFF_LO;
    SID_msg_LSB.reg_data = cutoff_lo;

    SID_msg_MSB.reg_addr = SID_FLT_CUTOFF_HI;
    SID_msg_MSB.reg_data = cutoff_hi;

    for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
     {
      SID_msg_LSB.SID_addr = G_voice_inventory[voice_counter].address;
      SID_msg_MSB.SID_addr = G_voice_inventory[voice_counter].address;
      write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg_LSB, sizeof(SID_msg_t));
      write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg_MSB, sizeof(SID_msg_t));
     }
    return 1;      

   }
  else return 0;

 }

uint8_t MIDI_SYSEX_FLT_EXT_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_OSC3_FILTER_LEN];
  uint8_t decoded_val,voice_counter;
  uint8_t apply_val = 0;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_OSC3_FILTER) || (buffer_len != SYSEX_OSC3_FILTER_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val == 0)
   {
     G_current_patch.filter_ext = decoded_val;
     apply_val = (G_current_patch.filter_reso << 4) | G_current_patch.osc1_filter_on |
                 G_current_patch.osc2_filter_on | G_current_patch.osc3_filter_on | decoded_val;
   }
  else if(decoded_val == FILTER_ROUTING_EXT)
   {
     G_current_patch.filter_ext = decoded_val;
     apply_val = (G_current_patch.filter_reso << 4) | G_current_patch.osc1_filter_on |
                 G_current_patch.osc2_filter_on | G_current_patch.osc3_filter_on | decoded_val;
   }
  else return 0;

  SID_msg.reg_addr = SID_FLT_RESO_ROUTE;
  SID_msg.reg_data = apply_val;
  for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
   {
    SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
   }
  return 1;
 }


/* miscellaneous parameters control: */

uint8_t MIDI_SYSEX_VOLUME_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  SID_msg_t SID_msg;
  uint8_t decoded_buffer[SYSEX_VOLUME_LEN];
  uint8_t decoded_val,voice_counter;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_VOLUME) || (buffer_len != SYSEX_VOLUME_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 15 )
    {
     G_current_patch.volume = decoded_val;
     SID_msg.reg_addr = SID_FLT_MODE_VOL;
     SID_msg.reg_data = G_current_patch.filter_mode | decoded_val;
     for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
      {
       SID_msg.SID_addr = G_voice_inventory[voice_counter].address;
       write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
      }

  }
 else return 0;

 }

uint8_t MIDI_SYSEX_TRANSPOSE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_TRANSPOSE_LEN];
  int8_t  decoded_signed_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_TRANSPOSE) || (buffer_len != SYSEX_TRANSPOSE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len - SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_signed_val = (int8_t)decoded_buffer[SYSEX_DATA_POS];

  if((decoded_signed_val >= -2 ) && (decoded_signed_val <= 2))
   {
    G_current_patch.octave_transposition = decoded_signed_val;
    return 1;
   }
  else
   return 0;
 }


/* LFO1 control: */

uint8_t MIDI_SYSEX_LFO1_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO1_STATE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO1_STATE) || (buffer_len != SYSEX_LFO1_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == 0 ) || (decoded_val == 1 ))
    {
     G_current_patch.LFO1_state = decoded_val;
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO1_SHAPE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO1_SHAPE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO1_SHAPE) || (buffer_len != SYSEX_LFO1_SHAPE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == LFO_SHAPE_TRIANGLE ) || (decoded_val == LFO_SHAPE_SAWTOOTH ) || (decoded_val == LFO_SHAPE_SQUARE ) ||
     (decoded_val == LFO_SHAPE_SH ))
    {
      G_current_patch.LFO1_shape = decoded_val;
      write(G_QSID_tasks[TASK_LFO1].input_pipe[1], &decoded_val, sizeof(uint8_t));
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO1_RATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO1_RATE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO1_RATE) || (buffer_len != SYSEX_LFO1_RATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= LFO_RATE_MAX)
    {
      G_current_patch.LFO1_rate = decoded_val;
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO1_DEPTH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO1_DEPTH_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO1_DEPTH) || (buffer_len != SYSEX_LFO1_DEPTH_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= LFO_DEPTH_MAX)
    {
      G_current_patch.LFO1_depth = decoded_val;
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO1_KEYSYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  /* TODO */
 }

uint8_t MIDI_SYSEX_LFO1_ROUTE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO1_ROUTE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO1_ROUTE) || (buffer_len != SYSEX_LFO1_ROUTE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 10) /* LFO route values are in 1 - 10 range */
    {
      G_current_patch.LFO1_routing = decoded_val;
      write(G_QSID_tasks[TASK_LFO1].input_pipe[1], &decoded_val, sizeof(uint8_t));
    }
 else return 0;
 }


/* LFO2 control: */


uint8_t MIDI_SYSEX_LFO2_STATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO2_STATE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO2_STATE) || (buffer_len != SYSEX_LFO2_STATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == 0 ) || (decoded_val == 1 ))
    {
     G_current_patch.LFO2_state = decoded_val;
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO2_SHAPE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO2_SHAPE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO2_SHAPE) || (buffer_len != SYSEX_LFO2_SHAPE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if((decoded_val == LFO_SHAPE_TRIANGLE ) || (decoded_val == LFO_SHAPE_SAWTOOTH ) || (decoded_val == LFO_SHAPE_SQUARE ) ||
     (decoded_val == LFO_SHAPE_SH ))
    {
      G_current_patch.LFO2_shape = decoded_val;
      write(G_QSID_tasks[TASK_LFO2].input_pipe[1], &decoded_val, sizeof(uint8_t));
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO2_RATE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO2_RATE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO2_RATE) || (buffer_len != SYSEX_LFO2_RATE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= LFO_RATE_MAX)
    {
      G_current_patch.LFO2_rate = decoded_val;
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO2_DEPTH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO2_DEPTH_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO2_DEPTH) || (buffer_len != SYSEX_LFO2_DEPTH_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= LFO_DEPTH_MAX)
    {
      G_current_patch.LFO2_depth = decoded_val;
    }
 else return 0;
 }

uint8_t MIDI_SYSEX_LFO2_KEYSYNC_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  /* TODO */
 }

uint8_t MIDI_SYSEX_LFO2_ROUTE_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
  uint8_t decoded_buffer[SYSEX_LFO2_ROUTE_LEN];
  uint8_t decoded_val;

  if((message_buffer[SYSEX_CODE_POS] != SYSEX_LFO2_ROUTE) || (buffer_len != SYSEX_LFO2_ROUTE_LEN))
   return 0;

  SYS_debug(DEBUG_HIGH,"MIDI_SYSEX_CBK: decoding %d bytes of sysex data",buffer_len-SYSEX_CONTROL_BYTES);

  if(!MIDI_decode_sysex(&message_buffer, buffer_len, &decoded_buffer))
   return 0;

  decoded_val = decoded_buffer[SYSEX_DATA_POS];

  if(decoded_val <= 10) /* LFO route values are in 1 - 10 range */
    {
      G_current_patch.LFO2_routing = decoded_val;
      write(G_QSID_tasks[TASK_LFO2].input_pipe[1], &decoded_val, sizeof(uint8_t));
    }
 else return 0;
 }



/* patch management control: */

uint8_t MIDI_SYSEX_TRANSMT_BANK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
 }

uint8_t MIDI_SYSEX_TRANSMT_BANK_DTA_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
 }

uint8_t MIDI_SYSEX_TRANSMT_PATCH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
 }

uint8_t MIDI_SYSEX_TRANSMT_PATCH_DTA_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
 }

uint8_t MIDI_SYSEX_RECV_BANK_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
 }

uint8_t MIDI_SYSEX_RECV_PATCH_CBK(unsigned char *message_buffer, uint16_t buffer_len)
 { 
 }



