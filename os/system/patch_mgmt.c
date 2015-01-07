#include "common.h"
#include "defs.h"
#include "patch.h"
#include "patch_mgmt.h"

uint8_t SYS_load_patch_bank(uint8_t bank_id)
 {
  /* stat patch bank file, check length which should be 128*sizeof(patch_data_t) */
  /* read() 128 * sizeof(patch_data_t) into patch bank array */
  /* validate patch bank */
  /* return appropriate error codes at subsequent stages */

 }

uint8_t SYS_save_patch_bank(uint8_t bank_id)
 {
  /* write patch bank to disk */

 }

uint8_t SYS_save_patch_to_bank(patch_data_t *patch, uint8_t bank_id, uint8_t bank_pos)
 {
  /* validate patch */
  /* memcpy patch to bank at bank_pos */

 }

uint8_t SYS_load_patch_from_bank(uint8_t bank_id, uint8_t bank_pos)
 {
 /* use existing routine SYNTH_load_current_patch to load and apply patch at bank_pos to G_current_patch */

 }

uint8_t SYS_init_patch_bank(uint8_t bank_id)
 {
  /* write base patch "triangle 1" to 128 cells of selected bank */

 }

uint8_t SYS_validate_patch_bank(uint8_t bank_id)
 {
  /* use existing routine SYS_validate_patch to validate 128 cells of patch_bank */

 }

uint8_t SYS_validate_patch(patch_data_t *patch)
 {

  if( (patch->osc1_pw > PW_RANGE) || ((patch->osc1_adsr_attack || patch->osc1_adsr_decay ||
                                   patch->osc1_adsr_sustain || patch->osc1_adsr_release) > 15) )
   return 0;

  if( (patch->osc2_pw > PW_RANGE) || ((patch->osc2_adsr_attack || patch->osc2_adsr_decay ||
                                   patch->osc2_adsr_sustain || patch->osc2_adsr_release) > 15) )
   return 0;

  if( (patch->osc3_pw > PW_RANGE) || ((patch->osc3_adsr_attack || patch->osc3_adsr_decay ||
                                   patch->osc3_adsr_sustain || patch->osc3_adsr_release) > 15) )
   return 0;

  if( (patch->filter_cutoff > CUTOFF_RANGE) || (patch->filter_reso > 15))
   return 0;

  if((patch->filter_mode != FILTER_OFF) && (patch->filter_mode != FILTER_HIGHPASS) && (patch->filter_mode != FILTER_BANDPASS) &&
     (patch->filter_mode != FILTER_LOWPASS))
   return 0;

  /* only basic checks - there should be a lot more here */

  return 1;

 }

