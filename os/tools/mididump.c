// 
// Q-SID project
//
// mididump.c - decode and display MIDI messages (MIDI tcpdump)
//

#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include <termio.h>
#include <fcntl.h>
#include <err.h>
#include <linux/serial.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "midinote.h"


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

void MIDI_dump(char *midi_msg, ssize_t msglen)
 {
  uint8_t byte_idx, midi_channel, midi_msgtype;

  midi_channel = midi_msg[0] & 0b00001111;
  midi_msgtype = midi_msg[0] & 0b11110000;

  printf("MIDI IN (%d bytes): channel %d ", msglen, midi_channel);

  switch(midi_msgtype) {

   case 0x90: 
     printf(", note on ");
     print_MIDI_note(midi_msg[1]);
     printf(", attack velocity: %X ",midi_msg[2]);
     break;

   case 0x80:
     printf(", note off ");
     print_MIDI_note(midi_msg[1]);
     printf(", release velocity: %X", midi_msg[2]);
     break;

   case 0xA0:
     printf(", aftertouch ");
     break;

   case 0xB0:
     printf(", control change ");
     break;

   case 0xC0:
     printf(", patch change ");
     break;

   case 0xD0:
     printf(", channel-pressure ");
     break;

   case 0xE0:
     printf(", pitch-bend ");
     break;

   case 0xF0:
     printf(", sysex ");
     break;

   default:
     printf(", unknown msgtype! ");

  }

  printf ("\n");

}



//
// mididump main
//

void main()
 {

   int8_t MIDI_fd,pollrc,end=0;
   ssize_t rc;
   char buff[1024];
   struct pollfd fds[1];
 
   MIDI_fd = MIDI_init();

   if( MIDI_fd == -1 )
    {
      printf("mididump: cannot initialize MIDI interface!\n");
      exit(-1);
    }

   bzero(buff,1024);

   fds[0].fd = MIDI_fd;
   fds[0].events = POLLIN;

   while(!end)
    {

      pollrc = poll( fds, 1, -1);

     if (pollrc < 0)
     {
       perror("poll");
     }

    else if( pollrc > 0)
     {
      if( fds[0].revents & POLLIN )
       {

         rc = read(MIDI_fd, buff, sizeof(buff) );

         if (rc > 0)
          {
           MIDI_dump(buff,rc);
           bzero(buff, rc);
          }

        }
      }
    }

 }



//
// mididump end
//
