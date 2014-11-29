/*

*/

#include "common.h"
#include "defs.h"
#include "midi.h"

uint8_t MIDI_is_partial_message(unsigned char *buffer, uint8_t len)
 {

  uint16_t buffer_position, sysex_len;
  uint8_t finished, midi_msgtype;

  buffer_position = 0;
  finished = 0;

  while(!finished)
   {

     midi_msgtype = buffer[buffer_position] & 0b11110000;

     if(midi_msgtype == 0xF0)   /* sysex - variable lenghth */
      {   
       sysex_len = MIDI_get_sysex_len(buffer, len);
       if(sysex_len == 0) 
        return 1; /* incomplete sysex message */
       else
        buffer_position += sysex_len;
      }
     else
      buffer_position += G_MIDI_msg_lengths[midi_msgtype];

     if(buffer_position > len) return 1;
     if(buffer_position == len) return 0;

   }

 }

void MIDI_init_MIDI_msg_lenghts(void)
 {

  G_MIDI_msg_lengths[0b10000000] = 3;   /* note off    */
  G_MIDI_msg_lengths[0b10010000] = 3;   /* note on     */
  G_MIDI_msg_lengths[0b10100000] = 3;   /* aftertouch  */
  G_MIDI_msg_lengths[0b10110000] = 3;   /* control change */
  G_MIDI_msg_lengths[0b11000000] = 2;   /* pgm change  */
  G_MIDI_msg_lengths[0b11010000] = 2;   /* aftertouch  */
  G_MIDI_msg_lengths[0b11100000] = 3;   /* pitch bend  */
  G_MIDI_msg_lengths[0b11110000] = 0;   /* sysex - undefined */
  G_MIDI_msg_lengths[0b11111000] = 1;   /* MIDI clock */
  G_MIDI_msg_lengths[0b11110001] = 1;   /* various common system/realtime messages: */
  G_MIDI_msg_lengths[0b11110010] = 1;
  G_MIDI_msg_lengths[0b11110011] = 1;
  G_MIDI_msg_lengths[0b11110110] = 1;
  G_MIDI_msg_lengths[0b11111010] = 1;
  G_MIDI_msg_lengths[0b11111011] = 1;
  G_MIDI_msg_lengths[0b11111110] = 1;
  G_MIDI_msg_lengths[0b11111111] = 1;   /* MIDI reset */

 }
