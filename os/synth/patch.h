/*
 Q-SID OS

 synth patch data structure
*/

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


#define CUTOFF_RANGE 2047
#define PW_RANGE 4095
#define DETUNE_RANGE 10
#define VOLUME_RANGE 15

typedef struct _patch_data_t {

  char patch_name[12];

  uint8_t  osc1_on;
  int8_t   osc1_fine;
  int8_t   osc1_transposition;
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
  int8_t   osc2_fine;
  int8_t   osc2_transposition;
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
  int8_t   osc3_fine;
  int8_t   osc3_transposition;
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

  uint8_t filter_adsr_attack;   /* 0 - 15 */
  uint8_t filter_adsr_decay;    /* 0 - 15 */
  uint8_t filter_adsr_sustain;  /* 0 - 15 */
  uint8_t filter_adsr_release;  /* 0 - 15 */

  uint8_t filter_adsr_amount;

  uint8_t  LFO1_state;       /* 0 - off, 1 - on */
  uint8_t  LFO1_rate;        /* 0 - 100 */
  uint8_t  LFO1_shape;       /* waveform constants defined in lfo.h */
  uint8_t  LFO1_depth;       /* 2,4,6,8,10,12 */
  uint8_t  LFO1_key_sync;    /* 0 - off, 1 - on  */
  uint8_t  LFO1_routing;     /* LFO destination constants defined in lfo.h  */

  uint8_t  LFO2_state;
  uint8_t  LFO2_rate;
  uint8_t  LFO2_shape;
  uint8_t  LFO2_depth;
  uint8_t  LFO2_key_sync;
  uint8_t  LFO2_routing;

  uint8_t  volume;
  int8_t   octave_transposition;

} patch_data_t;


patch_data_t G_current_patch;             /* polyphonic, monotimbral mode, all voices assigned to one patch */
patch_data_t G_current_multi_program[8];  /* multitimbral mode - multiple patches avaliable in one program  */

/* patch.h end */
