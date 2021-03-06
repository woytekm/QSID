#include "QSID_config.h"
#include "common.h"
#include "defs.h"
#include "QSID_live_settings.h"
#include "midi.h"

void MIDI_IN_thread(void)
 {

   int pollrc,end=0;
   uint8_t free_SID, midi_channel, midi_msgtype, last_message_incomplete, i;
   size_t rc, rcdelta;
   unsigned char midi_message_buffer[MIDI_IN_BUFLEN];
   struct pollfd fds;
 
   last_message_incomplete = 1;

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
         SYS_debug(DEBUG_HIGH,"MIDI_in_thread: serial read received %d bytes",rc);
         if (rc > 0)
          {

          
            while(last_message_incomplete)   /* partial midi message reassemblyi. TODO: add midi_message_buffer overflow check */
             {
              if(MIDI_is_partial_message(midi_message_buffer, rc))
               {
                SYS_debug(DEBUG_HIGH,"MIDI_in_thread: partial message - reading more");
                rcdelta += read(G_MIDI_fd, midi_message_buffer+rc, sizeof(midi_message_buffer) - rc );  
                if(rcdelta == 0) continue;  /* second read is empty - seems like truncated message - discard it */
                else if( (rc + rcdelta) >= MIDI_IN_BUFLEN) continue; /* next read will probably cause buffer overflow - discard */
                else rc += rcdelta;
               } 
              else last_message_incomplete = 0;
             }

           
           SYS_debug(DEBUG_HIGH,"MIDI_in_thread: received %d bytes from MIDI IN",rc);
 
           /* this will recursively parse entire message buffer: */
           MIDI_parse_one_MIDI_msg(midi_message_buffer, 0, rc);

           bzero(midi_message_buffer, rc);
           last_message_incomplete = 1;
           rc = rcdelta = 0;

          } /* if(rc > 0)  */

        } /* if( fds.revents & POLLIN)  */
 
      } /* if(pollrc > 0) */

   } /* while(!end)  */

 }

