#include "common.h"
#include "defs.h"

#ifdef QSID_RPI

/*
 MIDI initialization routine for Raspberry Pi UART 
*/

int8_t MIDI_RPi_MIDI_init(void)
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
    /* options.c_cflag |= (CLOCAL | CREAD); */
    /* options.c_cflag &= ~CRTSCTS;         */
    if (tcsetattr(UART_fd, TCSANOW, &options) != 0)
     return -1;

    return UART_fd;

 }

#endif


int8_t MIDI_init(void)
 {
  
  #ifdef QSID_RPI
   return MIDI_RPi_MIDI_init();
  #else
   SYS_error("no MIDI driver compiled in.");
   return -1;
  #endif

 }


