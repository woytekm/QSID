//
// Q-SID OS
//
// synth patch data structure
//

#include "common.h"

typedef struct {
  uint8_t  osc1_on;
  uint16_t osc1_freq;
  uint8_t  osc1_detune;
  uint16_t osc1_pw;
  uint8_t  osc1_wave;
  uint8_t  osc1_ringmod;
  uint8_t  osc1_sync;
  uint8_t  osc1_gate;
  uint8_t  osc1_adsr_attack;
  uint8_t  osc1_adsr_decay;
  uint8_t  osc1_adsr_sustain;
  uint8_t  osc1_adsr_release;

  uint8_t  osc2_on;
  uint16_t osc2_freq;
  uint8_t  osc2_detune;
  uint16_t osc2_pw;
  uint8_t  osc2_wave;
  uint8_t  osc2_ringmod;
  uint8_t  osc2_sync;
  uint8_t  osc2_gate;
  uint8_t  osc2_adsr_attack;
  uint8_t  osc2_adsr_decay;
  uint8_t  osc2_adsr_sustain;
  uint8_t  osc2_adsr_release;

  uint8_t  osc3_on;
  uint16_t osc3_freq;
  uint8_t  osc3_detune;
  uint16_t osc3_pw;
  uint8_t  osc3_wave;
  uint8_t  osc3_ringmod;
  uint8_t  osc3_sync;
  uint8_t  osc3_gate;
  uint8_t  osc3_adsr_attack;
  uint8_t  osc3_adsr_decay;
  uint8_t  osc3_adsr_sustain;
  uint8_t  osc3_adsr_release;


  uint16_t filter_cutoff;
  uint8_t  filter_reso;
  uint8_t  filter_map;
  uint8_t  filter_mode;
  uint8_t  volume;
  int8_t  octave_transposition;

} patch_data_t;


patch_data_t G_current_patch;             // polyphonic, monotimbral mode, all voices assigned to one patch
patch_data_t G_current_multi_program[8];  // multitimbral mode - multiple patches avaliable in one program

