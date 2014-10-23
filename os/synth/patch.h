//
// Q-SID OS
//
// synth patch data structure
//

#include "common.h"

#define WAVEFORM_NOISE 128
#define WAVEFORM_PULSE 64
#define WAVEFORM_SAWTOOTH 32
#define WAVEFORM_TRIANGLE 16

#define RINGMOD 4
#define RINGMOD_CLEAR 251

#define OSC_SYNC 2
#define OSC_SYNC_CLEAR 253

#define CLEAR_ALL_WAVEFORMS 14

#define FILTER_OFF 128
#define FILTER_HIGHPASS 64
#define FILTER_BANDPASS 32
#define FILTER_LOWPASS 16
#define FILTER_CLEAR 15

#define FILTER_ROUTING_NONE 240
#define FILTER_ROUTING_OSC1 1
#define FILTER_ROUTING_OSC2 2
#define FILTER_ROUTING_OSC3 4
#define FILTER_ROUTING_EXT 8

typedef struct _patch_data_t {
  uint8_t  osc1_on;
  uint8_t  osc1_detune;
  uint16_t osc1_pw;
  uint8_t  osc1_wave;
  uint8_t  osc1_ringmod_on;
  uint8_t  osc1_sync_on;
  uint8_t  osc1_adsr_attack;
  uint8_t  osc1_adsr_decay;
  uint8_t  osc1_adsr_sustain;
  uint8_t  osc1_adsr_release;
  uint8_t  osc1_filter_on;

  uint8_t  osc1_control_reg;


  uint8_t  osc2_on;
  uint8_t  osc2_detune;
  uint16_t osc2_pw;
  uint8_t  osc2_wave;
  uint8_t  osc2_ringmod_on;
  uint8_t  osc2_sync_on;
  uint8_t  osc2_adsr_attack;
  uint8_t  osc2_adsr_decay;
  uint8_t  osc2_adsr_sustain;
  uint8_t  osc2_adsr_release;
  uint8_t  osc2_filter_on;

  uint8_t  osc2_control_reg;


  uint8_t  osc3_on;
  uint8_t  osc3_detune;
  uint16_t osc3_pw;
  uint8_t  osc3_wave;
  uint8_t  osc3_ringmod_on;
  uint8_t  osc3_sync_on;
  uint8_t  osc3_adsr_attack;
  uint8_t  osc3_adsr_decay;
  uint8_t  osc3_adsr_sustain;
  uint8_t  osc3_adsr_release;
  uint8_t  osc3_filter_on;

  uint8_t  osc3_control_reg;


  uint16_t filter_cutoff;
  uint8_t  filter_reso;
  uint8_t  filter_mode;
  uint8_t  filter_ext;
  uint8_t  volume;
  int8_t  octave_transposition;

} patch_data_t;


patch_data_t G_current_patch;             // polyphonic, monotimbral mode, all voices assigned to one patch
patch_data_t G_current_multi_program[8];  // multitimbral mode - multiple patches avaliable in one program

