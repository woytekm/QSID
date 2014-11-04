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
#include "SID_control.h"
#include "task.h"
#include "lfo.h"


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

 // detect voice boards 

 if(!SYS_detect_voices())
  {
   SYS_error("No voice boards detected or detect error!");
   SYS_halt();
  } 
 else
  SYS_debug(DEBUG_LOW,"Detected %d voices in this synth",G_inventory_voice_count);

 // start system tasks

 SYS_debug(DEBUG_LOW,"Starting MIDI IN thread...");
 SYS_start_task(TASK_MIDI_IN, MIDI_IN_thread, SCHED_RR, PRIO_VERYHIGH95); 

 SYS_debug(DEBUG_LOW,"Starting SID writer thread...");
 SYS_start_task(TASK_SID_WRITER, LIB_SID_tx_thread, SCHED_RR, PRIO_VERYHIGH94);

 SYS_debug(DEBUG_LOW,"Starting SID remote control thread...");
 SYS_start_task(TASK_UDP_MIDI_IN, LIB_SID_remote_control, SCHED_RR, PRIO_NORMAL50);

 SYS_debug(DEBUG_LOW,"Starting LFO1...");
 SYS_start_task(TASK_LFO1, SYNTH_lfo1, SCHED_IDLE, 0);

 // do nothing

 int i;

 for(i = 1; i <= G_inventory_voice_count; i++)
  LIB_apply_demo_patch(G_voice_inventory[i].address);

 while(1)
  { sleep(1); }

 }



