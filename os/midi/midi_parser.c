#include "common.h"
#include "defs.h"

//
// this function should receive pointer to midi message buffer, and offset at which it should parse
// (initially zero), and then recursively parse entire message buffer
//

 void MIDI_parse_one_MIDI_msg(unsigned char *midi_in_buffer, uint16_t at_offset, uint16_t buflen)
  {

    uint8_t midi_channel, midi_msgtype;
    uint16_t next_message_offset;

    if(at_offset >= buflen)  // end of buffer
     return;

    midi_channel = (midi_in_buffer[at_offset] & 0b00001111) + 1; // channel ID in MIDI messages starts from 0
    midi_msgtype = midi_in_buffer[at_offset] & 0b11110000;

    switch(midi_msgtype) {

     case 0x90:  // MIDI note on (3 bytes)
      next_message_offset = at_offset + 3;
      if(midi_in_buffer[at_offset+2] == 0) // attack velocity = 0, this is NOTE OFF
        {
         SYS_debug(DEBUG_HIGH,"MIDI note on (off), CH%d, (%x, %x)", midi_channel, midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);
         SYNTH_note_off(midi_in_buffer[at_offset+1]);
         break;
        }

      SYS_debug(DEBUG_HIGH,"MIDI note on, CH%d, (%x, %x)", midi_channel, midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);
      SYNTH_note_on(midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);  // (note, attack_velocity)
      break;

    case 0x80:  // MIDI note off (3 bytes)
     next_message_offset = at_offset + 3;
     SYS_debug(DEBUG_HIGH,"MIDI note off, CH%d, (%x)", midi_channel, midi_in_buffer[at_offset+1]);
     SYNTH_note_off(midi_in_buffer[at_offset+1]);
     break;

   }

   MIDI_parse_one_MIDI_msg(midi_in_buffer, at_offset + next_message_offset, buflen);

 }



