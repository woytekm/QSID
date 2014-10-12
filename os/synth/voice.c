#include "common.h"
#include "defs.h"
#include "MIDI_to_SID.h"
#include "voice.h"
#include "patch.h"
#include "inventory.h"
#include "SID_control.h"

void SYNTH_note_on(uint16_t midi_note, uint16_t attack_velocity)
 {

   uint8_t free_voice;

   free_voice = SYNTH_get_free_voice();

   if(free_voice)
    {

     SYS_debug(DEBUG_HIGH,"SYNTH_note_on: allocating voice %d",free_voice);
     G_playing_voices++;
     G_voice_inventory[free_voice].state = G_playing_voices;  // set voice state to note age (this is currenlty most recent played note)
     G_voice_inventory[free_voice].note = midi_note;          // assign MIDI note to voice (needed for note off)

     // SID specific part 
     // TODO: check patch data: which oscillators to fire up, detune, and other stuff
     //if(G_current_patch.osc1_on)
      LIB_SID_OSC1_note_on(G_MIDI_to_SID_reg[midi_note+G_current_patch.octave_transposition],G_voice_inventory[free_voice].address);
     //if(G_current_patch.osc2_on)
      LIB_SID_OSC2_note_on(G_MIDI_to_SID_reg[midi_note+G_current_patch.octave_transposition],G_voice_inventory[free_voice].address);
     //if(G_current_patch.osc3_on)
      LIB_SID_OSC3_note_on(G_MIDI_to_SID_reg[midi_note+G_current_patch.octave_transposition],G_voice_inventory[free_voice].address);
     // SID specific part end

    }
   else 
    {
     SYS_debug(DEBUG_HIGH,"SYNTH_note_on: no free voices to play this note (%x)!",midi_note);
     return;
    }
 }

void SYNTH_note_on_fast(uint16_t midi_note, uint16_t attack_velocity)
 {

   uint8_t free_voice;

   free_voice = SYNTH_get_free_voice();

   if(free_voice)
    {

     SYS_debug(DEBUG_HIGH,"SYNTH_note_on_fast: allocating voice %d",free_voice);
     G_playing_voices++;
     G_voice_inventory[free_voice].state = G_playing_voices;  // set voice state to note age (this is currenlty most recent played note)
     G_voice_inventory[free_voice].note = midi_note;          // assign MIDI note to voice (needed for note off)

     LIB_SID_note_on(G_MIDI_to_SID_reg[midi_note+G_current_patch.octave_transposition],G_voice_inventory[free_voice].address);

    }
   else
    {
     SYS_debug(DEBUG_HIGH,"SYNTH_note_on_fast: no free voices to play this note (%x)!",midi_note);
     return;
    }

 }

void SYNTH_note_off(uint16_t midi_note)
 {

  uint8_t i;

  for(i=1; i <= G_inventory_voice_count; i++)
   {

    if(G_voice_inventory[i].note == midi_note)    //this is the voice that should be silenced
     {
      SYS_debug(DEBUG_HIGH,"SYNTH_note_off: freeing voice %d",i);
      G_playing_voices--;            // potential bug here! TODO
      G_voice_inventory[i].note = 0;
      G_voice_inventory[i].state = 0;

      // SID specific part
      LIB_SID_note_off(G_voice_inventory[i].address);
      // SID specific part end

      return;
     }
    
   }
   
   SYS_debug(DEBUG_HIGH,"SYNTH_note_off: %x: playing voice not found!", midi_note);

 }
