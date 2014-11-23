#include "common.h"
#include "defs.h"
#include "i2c_lib.h"
#include "QSID_live_settings.h"
#include "QSID_config.h"
#include "inventory.h"
#include "voice.h"
#include "midi.h"
#include "task.h"

 void SYS_init(void)
   {
 
     G_inventory_voice_count = 0;
     G_inventory_i2c_voice = 0;
     G_inventory_i2c_aux = 0;
     G_playing_voices = 0;
     G_QSID_task_count = 0;
 
     G_QSID_live_settings.MIDI_receive_channel = 1;

     SYS_debug(DEBUG_NORMAL,"SYS_init: registering sysex callbacks...");
     MIDI_register_sysex_callbacks();

     MIDI_init_MIDI_msg_lenghts();

     SYS_debug(DEBUG_NORMAL,"SYS_init: I2C init...");

#ifdef I2C_BCM2835
 
    LIB_i2c_bcm2835_init();
    G_i2c_voice_bus = 1;
    G_i2c_aux_bus = 0;
    G_inventory_i2c_voice = 1;
    G_inventory_i2c_aux = 1;
 
 #else

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
 
#endif
 
  }
