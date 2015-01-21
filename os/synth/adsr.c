#include "common.h"
#include "defs.h"
#include "task.h"
#include "adsr.h"
#include "patch.h"
#include "inventory.h"


#ifndef ADSR_SIGNAL_HANDLERS

#define ADSR_SIGNAL HANDLERS 1

void V1_SH_release(int sig) {G_V_ADSR_release_start[1] = 1;}
void V2_SH_release(int sig) {G_V_ADSR_release_start[2] = 1;}
void V3_SH_release(int sig) {G_V_ADSR_release_start[3] = 1;}
void V4_SH_release(int sig) {G_V_ADSR_release_start[4] = 1;}
void V5_SH_release(int sig) {G_V_ADSR_release_start[5] = 1;}
void V6_SH_release(int sig) {G_V_ADSR_release_start[6] = 1;}
void V7_SH_release(int sig) {G_V_ADSR_release_start[7] = 1;}
void V8_SH_release(int sig) {G_V_ADSR_release_start[8] = 1;}

void V1_SH_ADSR_stop(int sig) {G_V_ADSR_stop[1] = 1;}
void V2_SH_ADSR_stop(int sig) {G_V_ADSR_stop[2] = 1;}
void V3_SH_ADSR_stop(int sig) {G_V_ADSR_stop[3] = 1;}
void V4_SH_ADSR_stop(int sig) {G_V_ADSR_stop[4] = 1;}
void V5_SH_ADSR_stop(int sig) {G_V_ADSR_stop[5] = 1;}
void V6_SH_ADSR_stop(int sig) {G_V_ADSR_stop[6] = 1;}
void V7_SH_ADSR_stop(int sig) {G_V_ADSR_stop[7] = 1;}
void V8_SH_ADSR_stop(int sig) {G_V_ADSR_stop[8] = 1;}

#endif

void SYNTH_init_ADSR_event_handlers(void)
 {

  uint8_t i;

  for(i = 1; i <= 8; i++)
   {
    G_V_ADSR_stop[i] = 0;
    G_V_ADSR_release_start[i] = 0;
   }

  G_ADSR_release_SH[1] = V1_SH_release;
  G_ADSR_release_SH[2] = V2_SH_release;
  G_ADSR_release_SH[3] = V3_SH_release;
  G_ADSR_release_SH[4] = V4_SH_release;
  G_ADSR_release_SH[5] = V5_SH_release;
  G_ADSR_release_SH[6] = V6_SH_release;
  G_ADSR_release_SH[7] = V7_SH_release;
  G_ADSR_release_SH[8] = V8_SH_release;  

  G_ADSR_stop_SH[1] = V1_SH_ADSR_stop;
  G_ADSR_stop_SH[2] = V2_SH_ADSR_stop;
  G_ADSR_stop_SH[3] = V3_SH_ADSR_stop;
  G_ADSR_stop_SH[4] = V4_SH_ADSR_stop;
  G_ADSR_stop_SH[5] = V5_SH_ADSR_stop;
  G_ADSR_stop_SH[6] = V6_SH_ADSR_stop;
  G_ADSR_stop_SH[7] = V7_SH_ADSR_stop;
  G_ADSR_stop_SH[8] = V8_SH_ADSR_stop;

 }


void SYNTH_filter_ADSR(void *arg)
 {

  uint8_t *voice_id, readed, input_msg;
  uint8_t attack_steps, decay_steps, release_steps;
  uint16_t mod_val;
  sig_atomic_t *ADSR_stop, *ADSR_release;
  int input_pipe;

  voice_id = (uint8_t *)arg;

  ADSR_stop = &G_V_ADSR_stop[*voice_id];
  ADSR_release = &G_V_ADSR_release_start[*voice_id];

  input_pipe = G_QSID_tasks[TASK_FILTER_ADSR_V1 + (*voice_id-1)].input_pipe[0];

  signal(SIGUSR1,G_ADSR_stop_SH[*voice_id]);
  signal(SIGUSR2,G_ADSR_release_SH[*voice_id]);

  SYS_debug(DEBUG_HIGH,"SYNTH_filter_ADSR: starting ADSR filter modulator for voice %d",*voice_id);

  while(1)
   {

    /* wait for note on */
    readed = read(input_pipe, &input_msg, sizeof(input_msg)); 

    attack_steps = decay_steps = release_steps = 0;
    *ADSR_stop = *ADSR_release = 0;
    mod_val = 0;

    SYS_debug(DEBUG_HIGH,"SYNTH_filter_ADSR: attack start (V%d) %d, %d", *voice_id, *ADSR_stop, *ADSR_release);

    /* attack */
    while( (attack_steps <= G_ADSR_attack_steps[G_current_patch.filter_adsr_attack]) && (!*ADSR_stop) && (!*ADSR_release) )
     {
      printf("mod\n");
      attack_steps++;
      if((mod_val+G_ADSR_attack_step_size[G_current_patch.filter_adsr_attack]+G_current_patch.filter_cutoff) < CUTOFF_RANGE)
       mod_val += G_ADSR_attack_step_size[G_current_patch.filter_adsr_attack];
      LIB_SID_modulate_cutoff(G_voice_inventory[1].address, G_current_patch.filter_cutoff + mod_val);
      usleep(G_ADSR_attack_intervals[G_current_patch.filter_adsr_attack]);
     }
    
    SYS_debug(DEBUG_HIGH,"SYNTH_filter_ADSR: decay start (V%d)", *voice_id);
   
    /* decay */
    while( (decay_steps <= ADSR_DECAY_STEPS_5) && (!*ADSR_stop) && (!*ADSR_release) )
     {
      decay_steps++;
      usleep(1000);
     }

    SYS_debug(DEBUG_HIGH,"SYNTH_filter_ADSR: sustain start (V%d)", *voice_id);

    /* sustain */
    while( (!*ADSR_stop) && (!*ADSR_release) )
     {
      usleep(500);
     }

    SYS_debug(DEBUG_HIGH,"SYNTH_filter_ADSR: note off - release start (V%d)", *voice_id);

    /* release */
    while( (release_steps <= ADSR_RELEASE_STEPS_5) && (!*ADSR_stop) )
     {
      release_steps++;
      usleep(1000);
     }

    SYS_debug(DEBUG_HIGH,"SYNTH_filter_ADSR: finished modulation (V%d)", *voice_id);
     
   }

 } 

