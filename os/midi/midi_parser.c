#include "QSID_config.h"
#include "common.h"
#include "defs.h"
#include "inventory.h"
#include "QSID_live_settings.h"
#include "patch.h"
#include "patch_mgmt.h"
#include "midi.h"

/*

 this function should receive pointer to midi message buffer, and offset at which it should parse
 (initially zero), and then recursively parse entire message buffer
 
 this function expects that message buffer is already reassembled (no incomplete, truncated messages in buffer)

*/

 void MIDI_parse_one_MIDI_msg(unsigned char *midi_in_buffer, uint16_t at_offset, uint16_t buflen)
  {

    uint8_t midi_channel, midi_msgtype, i, dup = 0;
    uint16_t next_message_offset, sysex_len;

    if(at_offset >= buflen)   /* end of buffer  */
     return;
    
    if(midi_in_buffer[at_offset] < 240)  /* channel related message - let's split channel number and message type */
     {
      midi_channel = (midi_in_buffer[at_offset] & 0b00001111) + 1;  /* channel ID in MIDI messages starts from 0  */
      midi_msgtype = midi_in_buffer[at_offset] & 0b11110000;
     }
    else   /* message = 0b1111nnnn  - system common message or realtime message */
     {
      midi_channel = 0;
      midi_msgtype = midi_in_buffer[at_offset];
     }

    switch(midi_msgtype) {

     case 0x90:  /* note on */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0x90];

      if(midi_channel != G_QSID_live_settings.MIDI_receive_channel)
        break;

      if(midi_in_buffer[at_offset+2] == 0)   /* attack velocity = 0, this is NOTE OFF  */
        {
         SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: MIDI note on (off), CH%d, (%X, %X)", midi_channel, midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);
         SYNTH_note_off(midi_in_buffer[at_offset+1], 0);
         break;
        }

      SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: MIDI note on, CH%d, (%X, %X)", midi_channel, midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);

#ifdef DISCARD_DUPLICATE_NOTES

      for(i = 1; i <= G_inventory_voice_count; i++)
       {
        if(G_voice_inventory[i].note == midi_in_buffer[at_offset+1])
         {
          SYS_debug(DEBUG_NORMAL,"MIDI_parse_one_MIDI_msg: duplicate MIDI note %X - discarding", midi_in_buffer[at_offset+1]);
          dup = 1;
          break;
         }
       }

      if(dup) break;

#endif

      if(midi_in_buffer[at_offset+1] < 95 )
       SYNTH_note_on_fast(midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);   /* (note, attack_velocity)  */
      else
       SYS_debug(DEBUG_NORMAL,"MIDI_parse_one_MIDI_msg: invalid MIDI note value %X!",midi_in_buffer[at_offset+1]);
      break;

     case 0x80:  /* note off  */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0x80];

      if(midi_channel != G_QSID_live_settings.MIDI_receive_channel)
       break;

      SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: MIDI note off, CH%d, (%X)", midi_channel, midi_in_buffer[at_offset+1]);
      SYNTH_note_off(midi_in_buffer[at_offset+1], 0);
      break;

     case 0xA0:  /* aftertouch */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xA0];
      SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: aftertouch message (not supported)");
      break;

     case 0xB0:  /* control change */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xB0];
      if(midi_channel != G_QSID_live_settings.MIDI_receive_channel)
       break;
      else
       SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: control change message");
       MIDI_handle_CC(midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);
      break;

     case 0xC0: /* program change */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xC0];
      if(midi_channel != G_QSID_live_settings.MIDI_receive_channel)
       break;
      else
       SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: program change message (%d)",midi_in_buffer[at_offset+1]);
       if(midi_in_buffer[at_offset+1] < QSID_PATCH_BANK_SIZE)
        {
         G_QSID_live_settings.current_patch_idx = midi_in_buffer[at_offset+1];
         SYS_load_patch_from_bank(G_QSID_live_settings.current_bank_idx, G_QSID_live_settings.current_patch_idx);
        }
       else
        SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: invalid program number in program change (%d)",midi_in_buffer[at_offset+1]);
      break;

     case 0xD0: /* aftertouch */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xD0];
      SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: aftertouch message (not supported)");
      break;

     case 0xE0: /* pitchbend */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xE0];
      if(midi_channel != G_QSID_live_settings.MIDI_receive_channel)
       break;
      else
       SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: pitchbend message (not supported yet)");
      break;

     case 0xF0:  /* sysex */
      sysex_len = MIDI_get_sysex_len(&midi_in_buffer[at_offset], buflen - at_offset);

      SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: received system exclusive message (%d bytes)", sysex_len);

      if(sysex_len == 0)   /* MIDI_get_sysex_len detected incomplete sysex message  - drop it */
       return;

      next_message_offset = at_offset + sysex_len;

      MIDI_parse_sysex(&midi_in_buffer[at_offset], sysex_len);
      break;

     case 0xF8: /* MIDI clock */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xF8];
      /* do not log this as it would overwhelm the program - too many msgs */
      break;

     case 0xFF: /* MIDI reset */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xFF];
      SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: general MIDI reset message (not supported yet)");
      break;

     default:
      if(midi_msgtype > 240)
       {
        next_message_offset = at_offset + 1;
        SYS_debug(DEBUG_HIGH,"MIDI_parse_one_MIDI_msg: received unknown general system message");
       }
      else
       {
        SYS_debug(DEBUG_NORMAL,"MIDI_parse_one_MIDI_msg: unsupported MIDI message or garbage - discarding message buffer.");
        return;    /* we have to discard entire buffer since don't know what could be the offset of the next message */
       }

   }

   MIDI_parse_one_MIDI_msg(midi_in_buffer, next_message_offset, buflen);

 }



