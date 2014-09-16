//
// Q-SID OS main
//

#include "common.h"
#include "defs.h"

#include "qsid-config.h"
#include "inventory.h"
#include "i2c.h"
#include "midi.h"
#include "SID_writer.h"


//
// OS main
//

void main(void)

 {

  SYS_debug(DEBUG_LOW,"Q-SID OS version %d.%d",QSID_OS_VERSION_MAJOR, QSID_OS_VERSION_MINOR);
  // open i2c control files for voice and aux bus

  SYS_init();

  if( (G_i2c_voice_bus = LIB_i2c_open(I2C_VOICE_BUS)) < 0 )  
   {
    SYS_error("I2C (voice) open error!");
    SYS_halt();
   }

  else G_inventory_i2c_voice = 1;

  if( (G_i2c_aux_bus = LIB_i2c_open(I2C_AUX_BUS)) < 0 )  
   {
    SYS_error("I2C (aux) open error!");
    G_inventory_i2c_aux = 0;
   }
  else G_inventory_i2c_aux = 1;


 // detect voice boards - return address to voice inventory linked list

 if(!SYS_detect_voices())
  {
   SYS_error("No voice boards detected or detect error!");
   SYS_halt();
  } 
 else
  SYS_debug(DEBUG_LOW,"Detected %d voices in this synth",G_inventory_voice_count);

 SYS_debug(DEBUG_LOW,"Starting MIDI IN thread...");
 
 pthread_t MIDI_thread;
 pthread_attr_t MIDI_thread_attr;
 pthread_attr_init(&MIDI_thread_attr);
 pthread_attr_setdetachstate(&MIDI_thread_attr, PTHREAD_CREATE_DETACHED);
     
 if(pthread_create(&MIDI_thread, &MIDI_thread_attr, MIDI_IN_thread, NULL));

 SYS_debug(DEBUG_LOW,"Starting SID writer thread...");

 pthread_t SID_thread;
 pthread_attr_t SID_thread_attr;
 pthread_attr_init(&SID_thread_attr);
 pthread_attr_setdetachstate(&SID_thread_attr, PTHREAD_CREATE_DETACHED);

 if(pthread_create(&SID_thread, &SID_thread_attr, LIB_SID_tx_thread, NULL));


 while(1)
  { sleep(1); }

 }



