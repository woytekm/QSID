#include "common.h"
#include "defs.h"
#include "inventory.h"
#include "task.h"
#include "SID.h"
#include "SID_writer.h"
#include "patch.h"
#include "silencer.h"

void SYNTH_SIGALRM_empty(int sig)
 {}


void SYNTH_oscillator_silencer(void *input_arg)
/*
  Oscillator silencer is aimed at fixing oscillator leak bug present in SID chips
   - QSID OS will start as many silencers as there are actual oscillators in the synth (voice count * 3)
   - silencer thread will do it's setup, and then enter endless loop which blocks on reading input pipe
   - silencer will be notified via pipe that release phase for it's oscillator has just started
   - silencer will usleep through release time period, then will zeroize oscillator pitch registers and return to read() on pipe
   - if silencer will receive SIGALRM - it will cease current action and return to blocking read() on input pipe 

*/
 {

  uint8_t *my_task_id, input_pipe, readed, v_id, o_id, *rel_time, reg_lo, reg_hi, *o_state;
  uint8_t input_msg;
  SID_msg_t SID_msg_lo, SID_msg_hi;

  int16_t SID_release_to_msec[] = {6, 24, 48, 72, 114, 168, 204, 240, 300, 750, 1500, 2400, 3000, 9000, 15000, 24000};

  /* silencer setup : */

  my_task_id = (uint8_t *)input_arg;  /* get our oscillator id */

  signal(SIGALRM,SYNTH_SIGALRM_empty);

  input_pipe = G_QSID_tasks[*my_task_id].input_pipe[0];

  v_id = *my_task_id & 15;
  o_id = *my_task_id >> 4;

  SYS_debug(DEBUG_HIGH,"SYNTH_oscillator_silencer: starting silencer V%dO%d, (board %X)", v_id, o_id, G_voice_inventory[v_id].address);

  SID_msg_lo.reg_data = SID_msg_hi.reg_data = 0; /* silencer job is to  write zeros to pitch registers at the right moment */
  SID_msg_lo.SID_addr = SID_msg_hi.SID_addr = G_voice_inventory[v_id].address;

  switch (o_id)
   {
    case 1:
     rel_time = &G_current_patch.osc1_adsr_release;
     o_state = &G_current_patch.osc1_on;
     SID_msg_lo.reg_addr = SID_OSC1_FREQ_LO;
     SID_msg_hi.reg_addr = SID_OSC1_FREQ_HI;
     break;
    case 2:
     rel_time = &G_current_patch.osc2_adsr_release;
     o_state = &G_current_patch.osc2_on;
     SID_msg_lo.reg_addr = SID_OSC2_FREQ_LO;
     SID_msg_hi.reg_addr = SID_OSC2_FREQ_HI;
     break;
    case 3:
     rel_time = &G_current_patch.osc3_adsr_release;
     o_state = &G_current_patch.osc3_on;
     SID_msg_lo.reg_addr = SID_OSC3_FREQ_LO;
     SID_msg_hi.reg_addr = SID_OSC3_FREQ_HI;
     break;
   }

  /* silencer main loop: */

  while(1)
    {
     readed = read(input_pipe, &input_msg, sizeof(input_msg));      /* block thread on input pipe */

     if(readed == 1) /* it does not matter what is in the pipe - just check if it's one byte long */
      {

       if(*o_state)   /* is this oscillator active? */
        {
         SYS_debug(DEBUG_HIGH,"SYNTH_oscillator_silencer: release trigger for V%dO%d",v_id, o_id);
         if(usleep( (SID_release_to_msec[*rel_time]*1000) ) == -1) /* if usleep returned -1 - it was interrupted by SIGALRM */
           continue;
         else
          {
           SYS_debug(DEBUG_HIGH,"SYNTH_oscillator_silencer: silencing V%dO%d",v_id, o_id);
           write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg_lo, sizeof(SID_msg_t));
           write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg_hi, sizeof(SID_msg_t));
          }
        }

      }

    }
 }
