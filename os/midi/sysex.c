#include "common.h"
#include "defs.h"
#include "sysex.h"
#include "sysex_handlers.h"
#include "QSID_config.h"


void MIDI_register_sysex_handlers(void)
 {
  
  memset(G_sysex_handlers, 0, sizeof(G_sysex_handlers));

  G_sysex_handlers[SYSEX_OSC1_STATE]	= &MIDI_SYSEX_OSC1_STATE_CBK;
  G_sysex_handlers[SYSEX_OSC1_FINE]	= &MIDI_SYSEX_OSC1_FINE_CBK;
  G_sysex_handlers[SYSEX_OSC1_COARSE]	= &MIDI_SYSEX_OSC1_COARSE_CBK;
  G_sysex_handlers[SYSEX_OSC1_PW]	= &MIDI_SYSEX_OSC1_PW_CBK;
  G_sysex_handlers[SYSEX_OSC1_WAVE]	= &MIDI_SYSEX_OSC1_WAVE_CBK;
  G_sysex_handlers[SYSEX_OSC1_RINGMOD]	= &MIDI_SYSEX_OSC1_RINGMOD_CBK;
  G_sysex_handlers[SYSEX_OSC1_SYNC]	= &MIDI_SYSEX_OSC1_SYNC_CBK;
  G_sysex_handlers[SYSEX_OSC1_ATTACK]	= &MIDI_SYSEX_OSC1_ATTACK_CBK;
  G_sysex_handlers[SYSEX_OSC1_DECAY]	= &MIDI_SYSEX_OSC1_DECAY_CBK;
  G_sysex_handlers[SYSEX_OSC1_SUSTAIN]	= &MIDI_SYSEX_OSC1_SUSTAIN_CBK;
  G_sysex_handlers[SYSEX_OSC1_RELEASE]	= &MIDI_SYSEX_OSC1_RELEASE_CBK;
  G_sysex_handlers[SYSEX_OSC1_FILTER]	= &MIDI_SYSEX_OSC1_FILTER_CBK;

  G_sysex_handlers[SYSEX_OSC2_STATE]	= &MIDI_SYSEX_OSC2_STATE_CBK;
  G_sysex_handlers[SYSEX_OSC2_FINE]	= &MIDI_SYSEX_OSC2_FINE_CBK;
  G_sysex_handlers[SYSEX_OSC2_COARSE]	= &MIDI_SYSEX_OSC2_COARSE_CBK;
  G_sysex_handlers[SYSEX_OSC2_PW]	= &MIDI_SYSEX_OSC2_PW_CBK;
  G_sysex_handlers[SYSEX_OSC2_WAVE]	= &MIDI_SYSEX_OSC2_WAVE_CBK;
  G_sysex_handlers[SYSEX_OSC2_RINGMOD]	= &MIDI_SYSEX_OSC2_RINGMOD_CBK;
  G_sysex_handlers[SYSEX_OSC2_SYNC]	= &MIDI_SYSEX_OSC2_SYNC_CBK;
  G_sysex_handlers[SYSEX_OSC2_ATTACK]	= &MIDI_SYSEX_OSC2_ATTACK_CBK;
  G_sysex_handlers[SYSEX_OSC2_DECAY]	= &MIDI_SYSEX_OSC2_DECAY_CBK;
  G_sysex_handlers[SYSEX_OSC2_SUSTAIN]	= &MIDI_SYSEX_OSC2_SUSTAIN_CBK;
  G_sysex_handlers[SYSEX_OSC2_RELEASE]	= &MIDI_SYSEX_OSC2_RELEASE_CBK;
  G_sysex_handlers[SYSEX_OSC2_FILTER]	= &MIDI_SYSEX_OSC2_FILTER_CBK;

  G_sysex_handlers[SYSEX_OSC3_STATE]	= &MIDI_SYSEX_OSC3_STATE_CBK;
  G_sysex_handlers[SYSEX_OSC3_FINE]	= &MIDI_SYSEX_OSC3_FINE_CBK;
  G_sysex_handlers[SYSEX_OSC3_COARSE]	= &MIDI_SYSEX_OSC3_COARSE_CBK;
  G_sysex_handlers[SYSEX_OSC3_PW]	= &MIDI_SYSEX_OSC3_PW_CBK;
  G_sysex_handlers[SYSEX_OSC3_WAVE]	= &MIDI_SYSEX_OSC3_WAVE_CBK;
  G_sysex_handlers[SYSEX_OSC3_RINGMOD]	= &MIDI_SYSEX_OSC3_RINGMOD_CBK;
  G_sysex_handlers[SYSEX_OSC3_SYNC]	= &MIDI_SYSEX_OSC3_SYNC_CBK;
  G_sysex_handlers[SYSEX_OSC3_ATTACK]	= &MIDI_SYSEX_OSC3_ATTACK_CBK;
  G_sysex_handlers[SYSEX_OSC3_DECAY]	= &MIDI_SYSEX_OSC3_DECAY_CBK;
  G_sysex_handlers[SYSEX_OSC3_SUSTAIN]	= &MIDI_SYSEX_OSC3_SUSTAIN_CBK;
  G_sysex_handlers[SYSEX_OSC3_RELEASE]	= &MIDI_SYSEX_OSC3_RELEASE_CBK;
  G_sysex_handlers[SYSEX_OSC3_FILTER]	= &MIDI_SYSEX_OSC3_FILTER_CBK;

  G_sysex_handlers[SYSEX_FLT_MODE]	= &MIDI_SYSEX_FLT_MODE_CBK;
  G_sysex_handlers[SYSEX_FLT_RESO]	= &MIDI_SYSEX_FLT_RESO_CBK;
  G_sysex_handlers[SYSEX_FLT_CUTOFF]	= &MIDI_SYSEX_FLT_CUTOFF_CBK;
  G_sysex_handlers[SYSEX_FLT_EXT]	= &MIDI_SYSEX_FLT_EXT_CBK;

  G_sysex_handlers[SYSEX_VOLUME]	= &MIDI_SYSEX_VOLUME_CBK;
  G_sysex_handlers[SYSEX_TRANSPOSE]	= &MIDI_SYSEX_TRANSPOSE_CBK;

  G_sysex_handlers[SYSEX_LFO1_STATE]	= &MIDI_SYSEX_LFO1_STATE_CBK;
  G_sysex_handlers[SYSEX_LFO1_SHAPE]	= &MIDI_SYSEX_LFO1_SHAPE_CBK;
  G_sysex_handlers[SYSEX_LFO1_RATE]	= &MIDI_SYSEX_LFO1_RATE_CBK;
  G_sysex_handlers[SYSEX_LFO1_DEPTH]	= &MIDI_SYSEX_LFO1_DEPTH_CBK;
  G_sysex_handlers[SYSEX_LFO1_KEYSYNC]	= &MIDI_SYSEX_LFO1_KEYSYNC_CBK;
  G_sysex_handlers[SYSEX_LFO1_ROUTE]	= &MIDI_SYSEX_LFO1_ROUTE_CBK;

  G_sysex_handlers[SYSEX_LFO2_STATE]	= &MIDI_SYSEX_LFO2_STATE_CBK;
  G_sysex_handlers[SYSEX_LFO2_SHAPE]	= &MIDI_SYSEX_LFO2_SHAPE_CBK;
  G_sysex_handlers[SYSEX_LFO2_RATE]	= &MIDI_SYSEX_LFO2_RATE_CBK;
  G_sysex_handlers[SYSEX_LFO2_DEPTH]	= &MIDI_SYSEX_LFO2_DEPTH_CBK;
  G_sysex_handlers[SYSEX_LFO2_KEYSYNC]	= &MIDI_SYSEX_LFO2_KEYSYNC_CBK;
  G_sysex_handlers[SYSEX_LFO2_ROUTE]	= &MIDI_SYSEX_LFO2_ROUTE_CBK;

  G_sysex_handlers[SYSEX_TRANSMT_BANK]	= &MIDI_SYSEX_TRANSMT_BANK_CBK;
  G_sysex_handlers[SYSEX_TRANSMT_BANK_DTA]	= &MIDI_SYSEX_TRANSMT_BANK_DTA_CBK;
  G_sysex_handlers[SYSEX_TRANSMT_PATCH]	= &MIDI_SYSEX_TRANSMT_PATCH_CBK;
  G_sysex_handlers[SYSEX_TRANSMT_PATCH_DTA]	= &MIDI_SYSEX_TRANSMT_PATCH_DTA_CBK;
  G_sysex_handlers[SYSEX_RECV_BANK]	= &MIDI_SYSEX_RECV_BANK_CBK;
  G_sysex_handlers[SYSEX_RECV_PATCH]	= &MIDI_SYSEX_RECV_PATCH_CBK; 

 }

void MIDI_debug_dump_sysex(uint8_t *sysex_buffer, uint16_t sysex_len, uint8_t buffer_type)
 {

  if(QSID_DEBUG_LEVEL >= 3)
   {
     uint16_t i;

     if(buffer_type == 0)
      printf("encoded ");
     else printf("decoded ");

     printf("sysex buffer dump: [ ");
     for(i = 0; i < sysex_len; i++)
      printf(" %X ",sysex_buffer[i]);
     printf(" ] \n");
   }

 }

uint8_t MIDI_parse_sysex(unsigned char *sysex_data_buffer, uint8_t sysex_buffer_len)
 {

  uint8_t parse_error = 0;
  uint8_t (*sysex_handler)(unsigned char*, uint16_t);
  
  MIDI_debug_dump_sysex(sysex_data_buffer, sysex_buffer_len, 0);

  SYS_debug(DEBUG_HIGH,"MIDI_parse_sysex: parsing sysex message code: 0x%X",sysex_data_buffer[SYSEX_CODE_POS]);

  sysex_handler = G_sysex_handlers[sysex_data_buffer[SYSEX_CODE_POS]];   /* third byte should be sysex message type */

  if(sysex_handler != NULL)
   parse_error = sysex_handler(sysex_data_buffer, sysex_buffer_len);
  else
   SYS_debug(DEBUG_HIGH,"MIDI_parse_sysex: error: no handler for code: 0x%X",sysex_data_buffer[SYSEX_CODE_POS]);

  return parse_error;

 }

uint16_t MIDI_get_sysex_len(unsigned char *buffer, uint16_t buflen)
 {

  uint16_t count = 0;

  if(buffer[0] != 0xF0) return 0;

  while((buffer[count] != 0xF7) && (count <= buflen) )
   count++;

  if(buffer[count] == 0xF7) 
   return count + 1; 
  else 
   return 0;

 }

uint8_t MIDI_7bit_shift_uint16_t(uint16_t value, uint8_t t[2])
 {

  if(value > 16383) return 0;

  t[0] = (uint8_t)(value & 127);
  t[1] = (uint8_t)(value >> 7);
  return 1;

 }


uint8_t MIDI_7bit_unshift_uint16_t(uint8_t t[2])
 {

  uint16_t *tmp, decoded;

  tmp = (uint16_t *)t;

  decoded = *tmp & 127;
  decoded |= ((*tmp & 16256) >> 1);

  if(decoded > 16383) return 0;

  t[0] = decoded & 255;
  t[1] = decoded >> 8;

  return 1;

 }

