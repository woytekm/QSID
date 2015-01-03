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
#include "silencer.h"


void main(void)

 {
 
  uint8_t v_ctr, o_ctr;
  uint8_t silencer_task_id;

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
 SYS_start_task(TASK_MIDI_IN, MIDI_IN_thread, NULL, SCHED_RR, PRIO_VERYHIGH95); 

 SYS_debug(DEBUG_LOW,"Starting MIDI UDP IN thread...");
 SYS_start_task(TASK_MIDI_IN, MIDI_UDP_input, NULL, SCHED_RR, PRIO_NORMAL50);

 SYS_debug(DEBUG_LOW,"Starting SID writer thread...");
 SYS_start_task(TASK_SID_WRITER, LIB_SID_tx_thread, NULL, SCHED_RR, PRIO_VERYHIGH94);

 SYS_debug(DEBUG_LOW,"Starting SID remote control thread...");
 SYS_start_task(TASK_UDP_MIDI_IN, LIB_SID_remote_control, NULL, SCHED_RR, PRIO_NORMAL51);

#ifdef USE_SILENCERS

 SYS_debug(DEBUG_LOW,"Starting oscillator silencers...");

 for(v_ctr = 1; v_ctr <= (G_inventory_voice_count); v_ctr++)
  for(o_ctr = 1; o_ctr <= 3; o_ctr++)
   {
    silencer_task_id = (o_ctr << 4) | v_ctr;
    SYS_start_task(silencer_task_id, SYNTH_oscillator_silencer, (void *)&silencer_task_id, SCHED_RR, PRIO_VERYHIGH96);
   }

#endif

 /* init patch settings before starting LFO's */

 for(v_ctr = 1; v_ctr <= G_inventory_voice_count; v_ctr++)
  LIB_apply_demo_patch(G_voice_inventory[v_ctr].address);

 /* start LFO tasks  */

 SYS_debug(DEBUG_LOW,"Starting LFO1...");
 SYS_start_task(TASK_LFO1, SYNTH_LFO1, NULL, SCHED_IDLE, 0);

 /* do nothing */

 SYS_debug(DEBUG_LOW,"QSID ready.");

 while(1)
  { sleep(1); }

 }



