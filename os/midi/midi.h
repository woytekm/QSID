/*
 Q-SID OS

 MIDI definitions and variables
*/

#define MIDI_IN_BUFLEN 1024

int     G_MIDI_fd;
uint8_t G_MIDI_msg_lengths[255];

void MIDI_IN_thread(void);
void MIDI_UDP_input(void);

/* midi.h end  */
