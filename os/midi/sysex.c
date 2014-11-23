#include "common.h"
#include "defs.h"
#include "sysex.h"
#include "sysex_parsers.h"


void MIDI_register_sysex_callbacks(void)
 {
  
  G_sysex_callbacks[SYSEX_OSC1_STATE]	= &MIDI_SYSEX_OSC1_STATE_CBK;
  G_sysex_callbacks[SYSEX_OSC1_FINE]	= &MIDI_SYSEX_OSC1_FINE_CBK;
  G_sysex_callbacks[SYSEX_OSC1_COARSE]	= &MIDI_SYSEX_OSC1_COARSE_CBK;
  G_sysex_callbacks[SYSEX_OSC1_PW]	= &MIDI_SYSEX_OSC1_PW_CBK;
  G_sysex_callbacks[SYSEX_OSC1_WAVE]	= &MIDI_SYSEX_OSC1_WAVE_CBK;
  G_sysex_callbacks[SYSEX_OSC1_RINGMOD]	= &MIDI_SYSEX_OSC1_RINGMOD_CBK;
  G_sysex_callbacks[SYSEX_OSC1_SYNC]	= &MIDI_SYSEX_OSC1_SYNC_CBK;
  G_sysex_callbacks[SYSEX_OSC1_ATTACK]	= &MIDI_SYSEX_OSC1_ATTACK_CBK;
  G_sysex_callbacks[SYSEX_OSC1_RELEASE]	= &MIDI_SYSEX_OSC1_RELEASE_CBK;
  G_sysex_callbacks[SYSEX_OSC1_SUSTAIN]	= &MIDI_SYSEX_OSC1_SUSTAIN_CBK;
  G_sysex_callbacks[SYSEX_OSC1_RELEASE]	= &MIDI_SYSEX_OSC1_RELEASE_CBK;
  G_sysex_callbacks[SYSEX_OSC1_FILTER]	= &MIDI_SYSEX_OSC1_FILTER_CBK;

  G_sysex_callbacks[SYSEX_OSC2_STATE]	= &MIDI_SYSEX_OSC2_STATE_CBK;
  G_sysex_callbacks[SYSEX_OSC2_FINE]	= &MIDI_SYSEX_OSC2_FINE_CBK;
  G_sysex_callbacks[SYSEX_OSC2_COARSE]	= &MIDI_SYSEX_OSC2_COARSE_CBK;
  G_sysex_callbacks[SYSEX_OSC2_PW]	= &MIDI_SYSEX_OSC2_PW_CBK;
  G_sysex_callbacks[SYSEX_OSC2_WAVE]	= &MIDI_SYSEX_OSC2_WAVE_CBK;
  G_sysex_callbacks[SYSEX_OSC2_RINGMOD]	= &MIDI_SYSEX_OSC2_RINGMOD_CBK;
  G_sysex_callbacks[SYSEX_OSC2_SYNC]	= &MIDI_SYSEX_OSC2_SYNC_CBK;
  G_sysex_callbacks[SYSEX_OSC2_ATTACK]	= &MIDI_SYSEX_OSC2_ATTACK_CBK;
  G_sysex_callbacks[SYSEX_OSC2_RELEASE]	= &MIDI_SYSEX_OSC2_RELEASE_CBK;
  G_sysex_callbacks[SYSEX_OSC2_SUSTAIN]	= &MIDI_SYSEX_OSC2_SUSTAIN_CBK;
  G_sysex_callbacks[SYSEX_OSC2_RELEASE]	= &MIDI_SYSEX_OSC2_RELEASE_CBK;
  G_sysex_callbacks[SYSEX_OSC2_FILTER]	= &MIDI_SYSEX_OSC2_FILTER_CBK;

  G_sysex_callbacks[SYSEX_OSC3_STATE]	= &MIDI_SYSEX_OSC3_STATE_CBK;
  G_sysex_callbacks[SYSEX_OSC3_FINE]	= &MIDI_SYSEX_OSC3_FINE_CBK;
  G_sysex_callbacks[SYSEX_OSC3_COARSE]	= &MIDI_SYSEX_OSC3_COARSE_CBK;
  G_sysex_callbacks[SYSEX_OSC3_PW]	= &MIDI_SYSEX_OSC3_PW_CBK;
  G_sysex_callbacks[SYSEX_OSC3_WAVE]	= &MIDI_SYSEX_OSC3_WAVE_CBK;
  G_sysex_callbacks[SYSEX_OSC3_RINGMOD]	= &MIDI_SYSEX_OSC3_RINGMOD_CBK;
  G_sysex_callbacks[SYSEX_OSC3_SYNC]	= &MIDI_SYSEX_OSC3_SYNC_CBK;
  G_sysex_callbacks[SYSEX_OSC3_ATTACK]	= &MIDI_SYSEX_OSC3_ATTACK_CBK;
  G_sysex_callbacks[SYSEX_OSC3_RELEASE]	= &MIDI_SYSEX_OSC3_RELEASE_CBK;
  G_sysex_callbacks[SYSEX_OSC3_SUSTAIN]	= &MIDI_SYSEX_OSC3_SUSTAIN_CBK;
  G_sysex_callbacks[SYSEX_OSC3_RELEASE]	= &MIDI_SYSEX_OSC3_RELEASE_CBK;
  G_sysex_callbacks[SYSEX_OSC3_FILTER]	= &MIDI_SYSEX_OSC3_FILTER_CBK;

  G_sysex_callbacks[SYSEX_FLT_MODE]	= &MIDI_SYSEX_FLT_MODE_CBK;
  G_sysex_callbacks[SYSEX_FLT_RESO]	= &MIDI_SYSEX_FLT_RESO_CBK;
  G_sysex_callbacks[SYSEX_FLT_CUTOFF]	= &MIDI_SYSEX_FLT_CUTOFF_CBK;
  G_sysex_callbacks[SYSEX_FLT_EXT]	= &MIDI_SYSEX_FLT_EXT_CBK;

  G_sysex_callbacks[SYSEX_VOLUME]	= &MIDI_SYSEX_VOLUME_CBK;
  G_sysex_callbacks[SYSEX_TRANSPOSE]	= &MIDI_SYSEX_TRANSPOSE_CBK;

  G_sysex_callbacks[SYSEX_LFO1_STATE]	= &MIDI_SYSEX_LFO1_STATE_CBK;
  G_sysex_callbacks[SYSEX_LFO1_SHAPE]	= &MIDI_SYSEX_LFO1_SHAPE_CBK;
  G_sysex_callbacks[SYSEX_LFO1_RATE]	= &MIDI_SYSEX_LFO1_RATE_CBK;
  G_sysex_callbacks[SYSEX_LFO1_DEPTH]	= &MIDI_SYSEX_LFO1_DEPTH_CBK;
  G_sysex_callbacks[SYSEX_LFO1_KEYSYNC]	= &MIDI_SYSEX_LFO1_KEYSYNC_CBK;
  G_sysex_callbacks[SYSEX_LFO1_ROUTE]	= &MIDI_SYSEX_LFO1_ROUTE_CBK;

  G_sysex_callbacks[SYSEX_LFO2_STATE]	= &MIDI_SYSEX_LFO2_STATE_CBK;
  G_sysex_callbacks[SYSEX_LFO2_SHAPE]	= &MIDI_SYSEX_LFO2_SHAPE_CBK;
  G_sysex_callbacks[SYSEX_LFO2_RATE]	= &MIDI_SYSEX_LFO2_RATE_CBK;
  G_sysex_callbacks[SYSEX_LFO2_DEPTH]	= &MIDI_SYSEX_LFO2_DEPTH_CBK;
  G_sysex_callbacks[SYSEX_LFO2_KEYSYNC]	= &MIDI_SYSEX_LFO2_KEYSYNC_CBK;
  G_sysex_callbacks[SYSEX_LFO2_ROUTE]	= &MIDI_SYSEX_LFO2_ROUTE_CBK;

  G_sysex_callbacks[SYSEX_TRANSMT_BANK]	= &MIDI_SYSEX_TRANSMT_BANK_CBK;
  G_sysex_callbacks[SYSEX_TRANSMT_BANK_DTA]	= &MIDI_SYSEX_TRANSMT_BANK_DTA_CBK;
  G_sysex_callbacks[SYSEX_TRANSMT_PATCH]	= &MIDI_SYSEX_TRANSMT_PATCH_CBK;
  G_sysex_callbacks[SYSEX_TRANSMT_PATCH_DTA]	= &MIDI_SYSEX_TRANSMT_PATCH_DTA_CBK;
  G_sysex_callbacks[SYSEX_RECV_BANK]	= &MIDI_SYSEX_RECV_BANK_CBK;
  G_sysex_callbacks[SYSEX_RECV_PATCH]	= &MIDI_SYSEX_RECV_PATCH_CBK; 
   

 }

uint8_t MIDI_parse_sysex(unsigned char *sysex_data_buffer, uint8_t sysex_buffer_len)
 {
  uint8_t parse_error;
  uint8_t (*sysex_callback)(unsigned char*, uint8_t);
  
  sysex_callback = G_sysex_callbacks[sysex_data_buffer[1]];   /* second byte should be sysex message type */
  parse_error = sysex_callback(sysex_data_buffer, sysex_buffer_len);

  return parse_error;

 }

uint16_t MIDI_get_sysex_len(unsigned char *buffer, uint16_t buflen)
 {

  uint16_t byte_counter = 0;

  if(buffer[0] != 0xF0) return 0;

  while((buffer[byte_counter] != 0xF7) && (byte_counter <= buflen) )
   byte_counter++;

  if(buffer[byte_counter] == 0xF7) 
   return byte_counter + 1; 
  else 
   return 0;

 }


