#include "common.h"
#include "defs.h"
#include "lfo.h"
#include "task.h"

void SYNTH_lfo1(void)
 {

  int lfo1_input_pipe;
  char input_msg;
 
  lfo1_input_pipe = G_QSID_tasks[TASK_LFO1].input_pipe[0];
  

  if (fcntl(lfo1_input_pipe, F_SETFL, O_NONBLOCK) == -1) {
   SYS_debug(DEBUG_NORMAL,"SYNTH_lfo1: fcntl on input pipe failed: %d", errno);
  }
  
  while(1)
   {

    if(read(lfo1_input_pipe, input_msg, 1))
     SYS_debug(DEBUG_VERYHIGH,"SYNTH_lfo1: input pipe data: %c",input_msg);
     
    usleep(BASE_LFO_TICK);
   }

 }
 
