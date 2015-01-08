/*
 synth patch management routines
*/

#include "common.h"
#include "defs.h"
#include "patch.h"
#include "lfo.h"
#include "inventory.h"

void SYNTH_setup_base_patch(patch_data_t *patch)
 {

  strncpy(patch->patch_name,"triangle 1\0", 11);

  patch->octave_transposition = -12;
  patch->volume = 10;

  patch->osc1_on = 1;
  patch->osc1_fine = 0;
  patch->osc1_transposition = 0;
  patch->osc1_pw = 2048;
  patch->osc1_wave = WAVEFORM_TRIANGLE;
  patch->osc1_ringmod_on = 0;
  patch->osc1_sync_on = 0;
  patch->osc1_adsr_attack = 0;
  patch->osc1_adsr_decay = 0;
  patch->osc1_adsr_sustain = 15;
  patch->osc1_adsr_release = 0;
  patch->osc1_filter_on = 0;

  patch->osc2_on = 0;
  patch->osc2_fine = 0;
  patch->osc2_transposition = 0;
  patch->osc2_pw = 2048;
  patch->osc2_wave = WAVEFORM_TRIANGLE;
  patch->osc2_ringmod_on = 0;
  patch->osc2_sync_on = 0;
  patch->osc2_adsr_attack = 0;
  patch->osc2_adsr_decay = 0;
  patch->osc2_adsr_sustain = 15;
  patch->osc2_adsr_release = 0;
  patch->osc2_filter_on = 0;

  patch->osc3_on = 0;
  patch->osc3_fine = 0;
  patch->osc3_transposition = 0;
  patch->osc3_pw = 2048;
  patch->osc3_wave = WAVEFORM_TRIANGLE;
  patch->osc3_ringmod_on = 0;
  patch->osc3_sync_on = 0;
  patch->osc3_adsr_attack = 0;
  patch->osc3_adsr_decay = 0;
  patch->osc3_adsr_sustain = 15;
  patch->osc3_adsr_release = 0;
  patch->osc3_filter_on = 0;

  patch->filter_cutoff = 0;
  patch->filter_reso = 0;
  patch->filter_mode = FILTER_OFF;
  patch->filter_ext = 0;

  patch->LFO1_state = 0;
  patch->LFO1_rate = 1;
  patch->LFO1_depth = 0;
  patch->LFO1_shape = LFO_SHAPE_TRIANGLE;

  patch->LFO2_state = 0;
  patch->LFO2_rate = 1;
  patch->LFO2_depth = 0;
  patch->LFO2_shape = LFO_SHAPE_TRIANGLE;

  patch->osc1_control_reg = patch->osc1_wave | patch->osc1_ringmod_on | patch->osc1_sync_on;
  patch->osc2_control_reg = patch->osc2_wave | patch->osc2_ringmod_on | patch->osc2_sync_on;
  patch->osc3_control_reg = patch->osc3_wave | patch->osc3_ringmod_on | patch->osc3_sync_on;

 }


void SYNTH_load_current_patch(patch_data_t *new_patch)
 {

  int i;

  memcpy(&G_current_patch, new_patch, sizeof(patch_data_t)); 

  G_current_patch.osc1_control_reg = G_current_patch.osc1_wave | G_current_patch.osc1_ringmod_on | G_current_patch.osc1_sync_on;
  G_current_patch.osc2_control_reg = G_current_patch.osc2_wave | G_current_patch.osc2_ringmod_on | G_current_patch.osc2_sync_on;
  G_current_patch.osc3_control_reg = G_current_patch.osc3_wave | G_current_patch.osc3_ringmod_on | G_current_patch.osc3_sync_on;

  for(i=1; i<=G_inventory_voice_count; i++)
   {
    LIB_SID_via_mcp23017_reset(G_i2c_voice_bus, G_voice_inventory[i].address);
    usleep(100);
    LIB_apply_patch_to_SID(G_voice_inventory[i].address, &G_current_patch);
   }

 }


