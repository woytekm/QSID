/*
 synth patch management routines
*/

void SYNTH_load_patch(patch_data_t *new_patch)
 {

  int i;

  memcpy(&G_current_patch, new_patch, sizeof(patch_data_t)); 

  for(i=1; i<=G_inventory_voice_count; i++)
    SYNTH_apply_patch(&G_current_patch, G_voice_iventory[i].address);

 }

void SYNTH_apply_patch(patch_data_t *patch)
 {

  

 }

