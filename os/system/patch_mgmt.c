#include "common.h"
#include "defs.h"
#include "patch.h"
#include "patch_mgmt.h"


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

  if( ((patch->filter_adsr_attack || patch->filter_adsr_decay ||
        patch->filter_adsr_sustain || patch->filter_adsr_release) > 15) )
   return 0;

  /* only basic checks - there should be a lot more here */

  return 1;

 }


int8_t SYS_allocate_patch_banks()
 {

  uint8_t i, j;

  for(i = 0; i < QSID_PATCH_BANK_COUNT; i++)
   for(j = 0; j < QSID_PATCH_BANK_SIZE; j++)
    G_patch_banks[i][j] = malloc(sizeof(patch_data_t));

 }


int8_t SYS_load_patch_bank(uint8_t bank_id)
 {
  /* stat patch bank file, check length which should be 128*sizeof(patch_data_t) */
  /* read() 128 * sizeof(patch_data_t) into patch bank array */
  /* validate patch bank */
  /* return appropriate error codes at subsequent stages */

  uint8_t j;
  int bank_fd;
  struct stat bank_stat;
  char bank_fname[PATCH_BANK_NAMELEN];

  snprintf(bank_fname, PATCH_BANK_NAMELEN,"%s%s%d", 
           PATCH_BANK_PATH,PATCH_BANK_FILENAME, bank_id);
  
  if(stat(bank_fname, &bank_stat) == -1)
   {
    SYS_debug(DEBUG_LOW,"SYS_load_patch_bank: failed to load patch bank %d",bank_id);
    return errno;
   }

  if(bank_stat.st_size != (QSID_PATCH_BANK_SIZE*sizeof(patch_data_t)))
   {
    SYS_debug(DEBUG_LOW,"SYS_load_patch_bank: patch bank %d corrupted or differen version",bank_id);
    return EPBCORRUPT;
   }

  if((bank_fd = open(bank_fname, O_RDONLY)) == -1)
   {
    SYS_debug(DEBUG_LOW,"SYS_load_patch_bank: patch bank %d open error",bank_id);
    return errno;
   }

 for(j = 0; j < QSID_PATCH_BANK_SIZE; j++)
  if((read(bank_fd, (void *)G_patch_banks[bank_id][j], (sizeof(patch_data_t)))) == -1)
   {
    SYS_debug(DEBUG_LOW,"SYS_load_patch_bank: patch bank %d read error",bank_id);
    return errno;
   }
  
  return 0;

 }

int8_t SYS_save_patch_bank(uint8_t bank_id)
 {
 
   uint8_t j;  
   int bank_fd;
   char bank_fname[PATCH_BANK_NAMELEN];
   void *bank_buffer;

   snprintf(bank_fname, PATCH_BANK_NAMELEN,"%s%s%d",
            PATCH_BANK_PATH,PATCH_BANK_FILENAME, bank_id);

   if( (bank_fd = open(bank_fname, O_WRONLY)) == -1 )
    {
     SYS_debug(DEBUG_LOW,"SYS_save_patch_bank: patch bank %d open error",bank_id);
     return errno;
    }
 
  for(j = 0; j < QSID_PATCH_BANK_SIZE; j++)
   if((write(bank_fd, (void *)G_patch_banks[bank_id][j], (sizeof(patch_data_t)))) == -1)
    {
     SYS_debug(DEBUG_LOW,"SYS_save_patch_bank: patch bank %d write error",bank_id);
     return errno;
    }

   close(bank_fd);

   return 0;

 }

int8_t SYS_save_patch_to_bank(patch_data_t *patch, uint8_t bank_id, uint8_t bank_pos)
 {
  /* validate patch */
  /* memcpy patch to bank at bank_pos */

  if(bank_pos > QSID_PATCH_BANK_SIZE)
   return -1;

  if(SYS_validate_patch(patch))
   {
    memcpy((void *)G_patch_banks[bank_id][bank_pos], (void *)patch, sizeof(patch_data_t));
   }
  else return -1;

 }

uint8_t SYS_load_patch_from_bank(uint8_t bank_id, uint8_t bank_pos)
 {
  /* use existing routine SYNTH_load_current_patch to load and apply patch at bank_pos to G_current_patch */

   SYNTH_load_current_patch(G_patch_banks[bank_id][bank_pos]);
   
   return 1;

 }

uint8_t SYS_init_patch_bank(uint8_t bank_id)
 {

  uint8_t j;
  patch_data_t blank_patch;

  SYNTH_setup_base_patch(&blank_patch);

  for(j = 0; j < QSID_PATCH_BANK_SIZE; j++)
   memcpy((void *)G_patch_banks[bank_id][j],(void *)&blank_patch,sizeof(patch_data_t));

  return 1;
  

 }

uint8_t SYS_validate_patch_bank(uint8_t bank_id)
 {

  uint8_t j;

  for(j = 0; j < QSID_PATCH_BANK_SIZE; j++)
   if(!SYS_validate_patch(G_patch_banks[bank_id][j]))
    return 0;

  return 1; 

 }


int8_t SYS_load_all_patch_banks(void)
 {

  uint8_t i,j;

  for(i = 0; i < QSID_PATCH_BANK_COUNT; i++)
 
   if(SYS_load_patch_bank(i) != 0)
    {
     SYS_debug(DEBUG_LOW,"SYS_load_all_patch_banks: cannot load bank %d, initializing empty bank",i);
     SYS_init_patch_bank(i);
    }

  return 1;

 }


/* patch_mgmt.c end */
