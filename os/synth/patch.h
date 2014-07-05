//
// Q-SID OS
//
// synth patch data structure
//


typedef struct {

  uint_16t osc1_freq;
  uint_8t  osc1_detune;
  uint_16t osc1_pw;
  uint_8t  osc1_wave;
  uint_8t  osc1_ringmod;
  uint_8t  osc1_sync;
  uint_8t  osc1_gate;
  uint_8t  osc1_adsr_attack;
  uint_8t  osc1_adsr_decay;
  uint_8t  osc1_adsr_sustain;
  uint_8t  osc1_adsr_release;

  uint_16t osc2_freq;
  uint_8t  osc2_detune;
  uint_16t osc2_pw;
  uint_8t  osc2_wave;
  uint_8t  osc2_ringmod;
  uint_8t  osc2_sync;
  uint_8t  osc2_gate;
  uint_8t  osc2_adsr_attack;
  uint_8t  osc2_adsr_decay;
  uint_8t  osc2_adsr_sustain;
  uint_8t  osc2_adsr_release;

  uint_16t osc3_freq;
  uint_8t  osc3_detune;
  uint_16t osc3_pw;
  uint_8t  osc3_wave;
  uint_8t  osc3_ringmod;
  uint_8t  osc3_sync;
  uint_8t  osc3_gate;
  uint_8t  osc3_adsr_attack;
  uint_8t  osc3_adsr_decay;
  uint_8t  osc3_adsr_sustain;
  uint_8t  osc3_adsr_release;


  uint_16t filter_cutoff;
  uint_8t  filter_reso;
  uint_8t  filter_map;
  uint_8t  filter_mode;
  uint_8t  volume;

} patch_data_t;


