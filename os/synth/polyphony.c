#include "common.h"
#include "defs.h"
#include "inventory.h"

uint8_t SYNTH_get_free_voice(void)
 {

  uint8_t i, oldest_playing_voice = 0, oldest_playing_note = 0;
  uint64_t oldest_timestamp = 0;

  oldest_timestamp = G_voice_inventory[1].playstart;
  oldest_playing_voice = 1;
  oldest_playing_note = G_voice_inventory[1].note;

  for(i=1; i <= G_inventory_voice_count; i++)
   {
    
    if(G_voice_inventory[i].note == 0)    // found free voice
      return i;

    if(G_voice_inventory[i].note > 0)   
     if(G_voice_inventory[i].playstart < oldest_timestamp)   // find oldest playing voice
      {
        oldest_timestamp = G_voice_inventory[i].playstart;
        oldest_playing_voice = i;
        oldest_playing_note = G_voice_inventory[i].note;
      }
   }

  // kill oldest playing note and return it's voice as free

  SYS_debug(DEBUG_HIGH,"SYNTH_get_free_voice: no free voices: killing oldest note (%d, %d)\n",oldest_playing_voice, oldest_playing_note);
  SYNTH_note_off(oldest_playing_note);

  return oldest_playing_voice;

 }


