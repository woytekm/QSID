#include "common.h"
#include "defs.h"
#include "QSID_settings.h"
#include "midi.h"



void MIDI_IN_thread(void)
 {

   int pollrc,end=0;
   uint8_t free_SID, midi_channel, midi_msgtype;
   size_t rc;
   char midi_msg[256];
   struct pollfd fds;
 
   G_MIDI_fd = MIDI_init();

   if( G_MIDI_fd == -1 )
    {
      SYS_error("cannot initialize MIDI interface");
      exit(-1);
    }

   bzero(midi_msg,256);

   fds.fd = G_MIDI_fd;
   fds.events = POLLIN;

   while(!end)
    {

      pollrc = poll(&fds, 1, -1);

    if (pollrc < 0)
     {
       perror("poll");
     }

   else if( pollrc > 0)
     {

      if( fds.revents & POLLIN )
       {
         rc = read(G_MIDI_fd, midi_msg, sizeof(midi_msg) );
         if (rc > 0)
          {

           midi_channel = midi_msg[0] & 0b00001111;
           midi_msgtype = midi_msg[0] & 0b11110000;
 
           SYS_debug(DEBUG_HIGH,"MIDI IN (%d bytes): channel %d ", rc, midi_channel); 

           switch(midi_msgtype) {

            case 0x90: 
             if(midi_msg[2] == 0) // attack velocity = 0, this is NOTE OFF
              {
                SYS_debug(DEBUG_HIGH,"MIDI note on (off) (%x, %x)", midi_msg[1], midi_msg[2]);
                SYNTH_note_off(midi_msg[1]);
                break;
              }

             SYS_debug(DEBUG_HIGH,"MIDI note on (%x, %x)", midi_msg[1], midi_msg[2]);
             SYNTH_note_on(midi_msg[1], midi_msg[2]);  // (note, attack_velocity)
             break;

            case 0x80:
             SYS_debug(DEBUG_HIGH,"MIDI note off (%x)", midi_msg[1]);
             SYNTH_note_off(midi_msg[1]);
             break;

            }

           bzero(midi_msg, rc);

          } // if(rc > 0)

        } // if( fds.revents & POLLIN)
 
      } // if(pollrc > 0)

   } // while(!end)

 }

