#include "common.h"
#include "defs.h"
#include "QSID_settings.h"
#include "midi.h"



void MIDI_IN_thread(void)
 {

   int pollrc,end=0;
   uint8_t free_SID, midi_channel, midi_msgtype;
   size_t rc;
   unsigned char midi_message_buffer[1024];
   struct pollfd fds;
 
   G_MIDI_fd = MIDI_init();

   if( G_MIDI_fd == -1 )
    {
      SYS_error("cannot initialize MIDI interface");
      exit(-1);
    }

   bzero(midi_message_buffer,1024);

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
         rc = read(G_MIDI_fd, midi_message_buffer, sizeof(midi_message_buffer));
         if (rc > 0)
          {

           SYS_debug(DEBUG_HIGH,"MIDI_in_thread: received %d bytes from MIDI IN",rc);
           MIDI_parse_one_MIDI_msg(midi_message_buffer, 0, rc);

           bzero(midi_message_buffer, rc);

          } // if(rc > 0)

        } // if( fds.revents & POLLIN)
 
      } // if(pollrc > 0)

   } // while(!end)

 }

