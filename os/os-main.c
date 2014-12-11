/*
 Q-SID OS main
*/

#include "common.h"
#include "defs.h"

#include "QSID_config.h"
#include "inventory.h"
#include "midi.h"
#include "SID_writer.h"
#include "SID_control.h"
#include "task.h"
#include "lfo.h"


void main(void)

 {
 
  int voice_counter;

  SYS_debug(DEBUG_LOW,"Q-SID OS version %d.%d",QSID_OS_VERSION_MAJOR, QSID_OS_VERSION_MINOR);
  
  SYS_init();

 /* detect voice boards  */

 if(!SYS_detect_voices())
  {
   SYS_error("No voice boards detected or detect error!");
   SYS_halt();
  } 
 else
  SYS_debug(DEBUG_LOW,"Detected %d voices in this synth",G_inventory_voice_count);

 /* start system tasks */

 SYS_debug(DEBUG_LOW,"Starting MIDI IN thread...");
 SYS_start_task(TASK_MIDI_IN, MIDI_IN_thread, SCHED_RR, PRIO_VERYHIGH95); 

 SYS_debug(DEBUG_LOW,"Starting MIDI UDP IN thread...");
 SYS_start_task(TASK_MIDI_IN, MIDI_UDP_input, SCHED_RR, PRIO_NORMAL50);

 SYS_debug(DEBUG_LOW,"Starting SID writer thread...");
 SYS_start_task(TASK_SID_WRITER, LIB_SID_tx_thread, SCHED_RR, PRIO_VERYHIGH94);

 SYS_debug(DEBUG_LOW,"Starting SID remote control thread...");
 SYS_start_task(TASK_UDP_MIDI_IN, LIB_SID_remote_control, SCHED_RR, PRIO_NORMAL51);

 /* init patch settings before starting LFO's */

 for(voice_counter = 1; voice_counter <= G_inventory_voice_count; voice_counter++)
  LIB_apply_demo_patch(G_voice_inventory[voice_counter].address);

 /* start LFO tasks  */

 SYS_debug(DEBUG_LOW,"Starting LFO1...");
 SYS_start_task(TASK_LFO1, SYNTH_LFO1, SCHED_IDLE, 0);

 /* do nothing */

 SYS_debug(DEBUG_LOW,"QSID ready.");

 while(1)
  { sleep(1); }

 }



