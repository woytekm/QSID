#include "common.h"
#include "defs.h"
#include "inventory.h"

uint8_t SYNTH_get_free_voice(void)
 {

  uint8_t i, oldest_playing;

  for(i=1; i <= G_inventory_voice_count; i++)
   {

    if(G_voice_inventory[i].state == 0)    //free voice
     {
      return i;
      break;
     }

    if(G_voice_inventory[i].state == 1)   //oldest playing voice
     oldest_playing = i;
    
   }

  // if control reaches this point - this means that there are no free voices - for now do nothing
  // TODO - implement voice stealing here

  return 0;

 }


