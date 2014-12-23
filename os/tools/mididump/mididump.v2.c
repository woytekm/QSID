// 
// Q-SID project
//
// mididump.c - decode and display MIDI messages (MIDI tcpdump)
//

#include "common.h"
#include "defs.h"

#include "midinote.h"
#include "midi.h"

#ifdef QSID_RPI

//
// MIDI initialization routine for Raspberry Pi UART 
//

int8_t RPi_MIDI_init(void)
 {

    int8_t UART_fd;
    struct termios options;
    struct serial_struct serinfo;

    UART_fd = open("/dev/ttyAMA0", O_RDWR);

    if( UART_fd == -1)
     return -1;

    fcntl(UART_fd, F_SETFL, 0);
    tcgetattr(UART_fd, &options);
    cfsetispeed(&options, B38400);
    cfsetospeed(&options, B38400);
    cfmakeraw(&options);
    //options.c_cflag |= (CLOCAL | CREAD);
    //options.c_cflag &= ~CRTSCTS;
    if (tcsetattr(UART_fd, TCSANOW, &options) != 0)
     return -1;

    return UART_fd;

 }

#endif


//
// hardware abstracted wrapper for MIDI initialization - returns descriptor of open MIDI interface, or -1 for error
//

int8_t MIDI_init(void)
 {
  
  #ifdef QSID_RPI
   return RPi_MIDI_init();
  #else
   printf("mididump: no MIDI driver compiled in!\n");
   return -1;
  #endif

 }


//
// print note name received in MIDI message
//

void print_MIDI_note(uint8_t note)
 {

   printf (", note %s ",G_midinotes[note]);

 }


//
// decode and print received MIDI message
//

 void MIDI_parse_msgbuffer(unsigned char *midi_in_buffer, uint16_t at_offset, uint16_t buflen)
  {

    uint8_t midi_channel, midi_msgtype;
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

      if(midi_in_buffer[at_offset+2] == 0)   /* attack velocity = 0, this is NOTE OFF  */
        printf("RCV: MIDI note on (off), CH%d, (%x, %x)\n", midi_channel, midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);
      else
        printf("RCV: MIDI note on, CH%d, (%x, %x)\n", midi_channel, midi_in_buffer[at_offset+1], midi_in_buffer[at_offset+2]);

      if(midi_in_buffer[at_offset+1] > 95 )
       printf("  invalid MIDI note value %d!\n",midi_in_buffer[at_offset+1]);
      break;

     case 0x80:  /* note off  */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0x80];
      printf("RCV: MIDI note off, CH%d, (%x)\n", midi_channel, midi_in_buffer[at_offset+1]);
      break;

     case 0xA0:  /* aftertouch */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xA0];
      printf("RCV: aftertouch message\n");
      break;

     case 0xB0:  /* control change */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xB0];
      printf("RCV: control change message\n");
      break;

     case 0xC0: /* program change */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xC0];
      printf("RCV: program change message\n");
      break;

     case 0xD0: /* aftertouch */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xD0];
      printf("RCV: MIDI aftertouch message\n");
      break;

     case 0xE0: /* pitchbend */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xE0];
      printf("RCV: MIDI pitchbend message\n");
      break;

     case 0xF0:  /* sysex */
      sysex_len = MIDI_get_sysex_len(&midi_in_buffer[at_offset], buflen - at_offset);

      printf("RCV: MIDI system exclusive message (%d bytes)\n", sysex_len);

      if(sysex_len == 0)   /* MIDI_get_sysex_len detected incomplete sysex message  - drop it */
       return;

      next_message_offset = at_offset + sysex_len;
      break;

     case 0xF8: /* MIDI clock */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xF8];
      /* do not log this as it would overwhelm the program - too many msgs */
      break;

     case 0xFF: /* MIDI reset */
      next_message_offset = at_offset + G_MIDI_msg_lengths[0xFF];
      printf("RCV: general MIDI reset message\n");
      break;

     default:
      if(midi_msgtype > 240)
       {
        next_message_offset = at_offset + 1;
        SYS_debug(DEBUG_HIGH,"RCV: unknown general system message\n");
       }
      else
       {
        printf("RCV: unsupported MIDI message or garbage - discarding message buffer\n");
        return;    
       }

   }

   MIDI_parse_msgbuffer(midi_in_buffer, next_message_offset, buflen);

 }



//
// mididump main
//

void main()
 {

   int8_t G_MIDI_fd,pollrc,end=0, i, last_message_incomplete;
   uint16_t rc,rcdelta;
   char midi_message_buffer[1024];
   struct pollfd fds;
   
   MIDI_init_MIDI_msg_lenghts();
 
   G_MIDI_fd = MIDI_init();

   if( G_MIDI_fd == -1 )
    {
      printf("mididump: cannot initialize MIDI interface!\n");
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
         printf("serial read: received %d bytes\n",rc);
         if (rc > 0)
          {


            while(last_message_incomplete)   /* partial midi message reassemblyi. TODO: add midi_message_buffer overflow check */
             {
              if(MIDI_is_partial_message(midi_message_buffer, rc))
               {
                printf("serial read: incomplete message - reading more...\n");
                rcdelta += read(G_MIDI_fd, midi_message_buffer+rc, sizeof(midi_message_buffer) - rc );
                if(rcdelta == 0) 
                  {
                   printf("serial read: seems like truncated message\n");
                   continue;  /* second read is empty - seems like truncated message - discard it */
                  }
                else if( (rc + rcdelta) >= MIDI_IN_BUFLEN) continue; /* next read will probably cause buffer overflow - discard */
                else rc += rcdelta;
               }
              else last_message_incomplete = 0;
             }
           }

           printf("serial read finished: received %d bytes from MIDI IN : [ ",rc);

           for(i=0; i<rc; i++)
            printf("%X ",midi_message_buffer[i]);
           printf("]\n");

           /* this will recursively parse entire message buffer: */
           MIDI_parse_msgbuffer(midi_message_buffer, 0, rc);
   
           printf("\n");

           bzero(midi_message_buffer, rc);
           last_message_incomplete = 1;


           bzero(midi_message_buffer, rc);
          }

        }
      }

 }



//
// mididump end
//
