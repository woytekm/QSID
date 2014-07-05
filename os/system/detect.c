//
// Q-SID OS
//
// voice board autodetection routine
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "i2c_lib.h"
#include "MCP23017.h"
#include "rpi-sid.h"


#define I2C_FILE_NAME "/dev/i2c-1"

int main() {
    int counter = 0;
    i2c_debug = 0;
    SID_msg_t SID_msg;

    global_i2c_1_io = 0x20;
     
    // Open a connection to the I2C userspace control file.
    if ((global_i2c_1_descriptor = open(I2C_FILE_NAME, O_RDWR)) < 0) {
        perror("Unable to open i2c control file");
        exit(1);
    }

    //set all lines of MCP23017 to output  
    set_i2c_register(global_i2c_1_descriptor, global_i2c_1_io, MCP23017_IODIR_A, 0);
    set_i2c_register(global_i2c_1_descriptor, global_i2c_1_io, MCP23017_IODIR_B, 0);

    //reset SID
    SID_via_mcp23017_reset(global_i2c_1_descriptor, global_i2c_1_io);

    sleep(1);

    //start various synth threads
    SID_synth_threads_init(); 

    //init synth settings
    SID_init_synth();
  
    //set sound parameters
    SID_msg.addr = SID_OSC1_ATTACK; SID_msg.data = 7;
    SID_queue_one_msg(&global_SID_msg_queue, &SID_msg);
    SID_msg.addr = SID_OSC1_SUSTAIN; SID_msg.data = 0;
    SID_queue_one_msg(&global_SID_msg_queue, &SID_msg);
    SID_msg.addr = SID_FLT_MODE_VOL; SID_msg.data = 31;
    SID_queue_one_msg(&global_SID_msg_queue, &SID_msg);

    SID_msg.addr = SID_FLT_MODE_VOL; SID_msg.data = 31;
    SID_queue_one_msg(&global_SID_msg_queue, &SID_msg);

    //play some note pattern


    while(counter < 100)
     {
       SID_play_note(0x0449, NOTE_EIGHTH, BPM160);
       SID_play_note(0x0449, NOTE_EIGHTH, BPM160);
       SID_play_note(0x0893, NOTE_EIGHTH ,BPM160);
       SID_play_note(0x0893, NOTE_EIGHTH ,BPM160);
       SID_play_note(0x0893, NOTE_EIGHTH, BPM160);
       SID_play_note(0x0893, NOTE_EIGHTH, BPM160);
       counter++;
     }

    return 0;

  }
