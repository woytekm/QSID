#include "common.h"
#include "defs.h"
#include "patch.h"
#include "lfo.h"
#include "task.h"
#include "inventory.h"

void SYNTH_LFO1(void)
 {
  int LFO1_input_pipe;
  uint8_t LFO_step, retrigger, waveform_changed, routing_changed, input_msg, lfo_off, current_destination, i;
  uint16_t mod_value, apply_value, apply_value_osc1,
           apply_value_osc2, apply_value_osc3;
  uint16_t LFO_ranges[10];
  uint16_t LFO_steps[10];

  lfo_off = 1;
  mod_value = 0;
  apply_value = apply_value_osc1 = apply_value_osc2 = apply_value_osc3 = 0;

  LFO1_input_pipe = G_QSID_tasks[TASK_LFO1].input_pipe[0];

  if (fcntl(LFO1_input_pipe, F_SETFL, O_NONBLOCK) == -1) 
   {
    SYS_debug(DEBUG_NORMAL,"SYNTH_lfo1: fcntl on input pipe failed: %d", errno);
    return;
   }

  LFO_ranges[LFO_ROUTING_CUTOFF] = LFO_CUTOFF_RANGE;
  LFO_ranges[LFO_ROUTING_PW_1] = LFO_PW_RANGE;
  LFO_ranges[LFO_ROUTING_PW_2] = LFO_PW_RANGE;
  LFO_ranges[LFO_ROUTING_PW_3] = LFO_PW_RANGE;
  LFO_ranges[LFO_ROUTING_PW_ALL] = LFO_PW_RANGE;

  LFO_ranges[LFO_ROUTING_DETUNE_1] = LFO_DETUNE_RANGE;
  LFO_ranges[LFO_ROUTING_DETUNE_2] = LFO_DETUNE_RANGE;
  LFO_ranges[LFO_ROUTING_DETUNE_3] = LFO_DETUNE_RANGE;
  LFO_ranges[LFO_ROUTING_DETUNE_ALL] = LFO_DETUNE_RANGE;

  LFO_ranges[LFO_ROUTING_MAIN_VOLUME] = LFO_VOLUME_RANGE;

  LFO_steps[LFO_ROUTING_CUTOFF] = 20;
  LFO_steps[LFO_ROUTING_PW_1] = 50;
  LFO_steps[LFO_ROUTING_PW_2] = 50;
  LFO_steps[LFO_ROUTING_PW_3] = 50;
  LFO_steps[LFO_ROUTING_PW_ALL] = 50;

  LFO_steps[LFO_ROUTING_DETUNE_1] = 1;
  LFO_steps[LFO_ROUTING_DETUNE_2] = 1;
  LFO_steps[LFO_ROUTING_DETUNE_3] = 1;
  LFO_steps[LFO_ROUTING_DETUNE_ALL] = 1;

  LFO_steps[LFO_ROUTING_MAIN_VOLUME] = 1; 

  while( 1 )
   {

     usleep(LFO_BASE_TICK + (100000/G_current_patch.LFO1_rate));

     if(read(LFO1_input_pipe, input_msg, 1))
      {
       SYS_debug(DEBUG_VERYHIGH,"SYNTH_LFO1: input pipe data: %c",input_msg);
       if((input_msg >= 0) && (input_msg <= 14))
        {
         if(input_msg == 0)
          {
           lfo_off = 1;
           //reset register values to patch settings here!
          }
         else if(input_msg <= 10)
          {
           routing_changed = 1;
           current_destination = input_msg;
           lfo_off = 0;
          }
         else 
           waveform_changed = 1;
        }
      }

     if(lfo_off) continue;

     if((waveform_changed) || (routing_changed))
      {
       mod_value = 0;
       LFO_step = LFO_steps[current_destination];
       waveform_changed = routing_changed = 0;
      }

     switch(G_current_patch.LFO1_shape)
      {
       case LFO_SHAPE_TRIANGLE:
        mod_value += LFO_step;
        if((mod_value >= ((G_current_patch.LFO1_depth*LFO_step)/2)) || 
          ((mod_value + LFO_step) >= LFO_ranges[current_destination]))
            LFO_step = LFO_steps[current_destination] * -1;
        else if((mod_value <= ((G_current_patch.LFO1_depth*LFO_step)/2)) || 
               ((mod_value + LFO_step) <= 0))
            LFO_step = LFO_steps[current_destination];
        break;

       case LFO_SHAPE_SAWTOOTH:
        mod_value += LFO_step;
        if( (mod_value >= (G_current_patch.LFO1_depth*LFO_step)/2) || ((mod_value + LFO_step) >= LFO_CUTOFF_RANGE) ) 
          mod_value = ((G_current_patch.LFO1_depth*LFO_step)/2) * -1;
        break;
      }

    // clip the value if it went out of range

    if(apply_value < 0) apply_value = 0;
    if(apply_value > LFO_ranges[current_destination]) apply_value = LFO_ranges[current_destination];

    switch(current_destination)
     {
        case LFO_ROUTING_CUTOFF:
        apply_value = G_current_patch.filter_cutoff + mod_value;
        for(i = 1; i <= G_inventory_voice_count; i++)
          LIB_apply_LFO_CUTOFF(i,apply_value);
        break;

        case LFO_ROUTING_PW_1:
        apply_value = G_current_patch.osc1_pw + mod_value;
        for(i = 1; i <= G_inventory_voice_count; i++)
          LIB_apply_LFO_PW1(i,apply_value);
        break;

        case LFO_ROUTING_PW_2:
        apply_value = G_current_patch.osc2_pw + mod_value;
        for(i = 1; i <= G_inventory_voice_count; i++)
          LIB_apply_LFO_PW2(i,apply_value);
        break;

        case LFO_ROUTING_PW_3:
        apply_value = G_current_patch.osc3_pw + mod_value;
        for(i = 1; i <= G_inventory_voice_count; i++)
          LIB_apply_LFO_PW3(i,apply_value);
        break;

        case LFO_ROUTING_PW_ALL:
        apply_value_osc1 = G_current_patch.osc1_pw + mod_value;
        apply_value_osc2 = G_current_patch.osc2_pw + mod_value;
        apply_value_osc3 = G_current_patch.osc3_pw + mod_value;
        for(i = 1; i <= G_inventory_voice_count; i++)
         {
          LIB_apply_LFO_PW1(i,apply_value_osc1);
          LIB_apply_LFO_PW2(i,apply_value_osc2);
          LIB_apply_LFO_PW3(i,apply_value_osc3);
         }
        break;
     }

   }

  }

