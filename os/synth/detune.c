#include "common.h"
#include "defs.h"
#include "MIDI_to_SID.h"

// detune is calculated in SID register values, and it's base unit translates roughly to 1/10 of adjacent note interval (10 cents)
// detune can be anything from -10 to 10, which means that maximal detune value should set oscillator to the adjacent note pitch (+/- 100 cents in 10 cent units)
 
int16_t SYNTH_SID_oscillator_detune(uint8_t MIDI_note, int8_t osc_detune)
 {

  float note_interval;

  if(!osc_detune) return;

  if(osc_detune > 0 )  
    {
       note_interval = G_MIDI_to_SID_reg[(MIDI_note + 1)] - G_MIDI_to_SID_reg[(MIDI_note)];
       return  (note_interval / 10) * osc_detune;
    }
  else
   {
      note_interval = G_MIDI_to_SID_reg[(MIDI_note)] - G_MIDI_to_SID_reg[(MIDI_note - 1)];
      return  (note_interval / 10) * osc_detune;  
   }

 }

