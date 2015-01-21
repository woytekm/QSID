#include "QSID_config.h"
#include "common.h"
#include "defs.h"
#include "SID_writer.h"
#include "MIDI_to_SID.h"
#include "SID.h"
#include "patch.h"
#include "task.h"

uint16_t G_MIDI_to_SID_reg[95] = { 274, 291, 308, 325, 346, 366, 388, 411, 435, 461, 489, 518,
                         549, 581, 616, 652, 691, 732, 776, 822, 871, 923, 978, 1036,
                         1097, 1163, 1232, 1305, 1383, 1465, 1552, 1644, 1742, 1845, 1955,
                         2071, 2195, 2325, 2463, 2610, 2765, 2930, 3104, 3288, 3484, 3691,
                         3910, 4143, 4389, 4650, 4927, 5220, 5530, 5859, 6207, 6577, 6968,
                         7382, 7821, 8286, 8779, 9301, 9854, 10440, 11060, 11718, 12415,
                         13153, 13935, 14764, 15642, 16572, 17557, 18601, 19709, 20897,
                         22121, 23436, 24830, 26306, 27871, 29528, 31234, 33144, 35115,
                         37203, 39415, 41759, 44242, 46873, 49660, 52613, 55741, 59056,
                         62657 };


uint8_t LIB_apply_patch_to_SID(uint8_t board_address, patch_data_t *patch)
 {

  SYS_debug(DEBUG_HIGH,"LIB_apply_patch_to_SID: applying patch \"%s\" to SID at %X",patch->patch_name, board_address); 

  SID_msg_t SID_msg;
  SID_msg.SID_addr = board_address;
 
 
  SID_msg.reg_addr = SID_OSC1_PW_LO;
  SID_msg.reg_data = patch->osc1_pw & 255;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC1_PW_HI;
  SID_msg.reg_data = patch->osc1_pw >> 8;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC1_ATTACK;
  SID_msg.reg_data = (patch->osc1_adsr_attack << 4) | patch->osc1_adsr_decay;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC1_SUSTAIN;
  SID_msg.reg_data = (patch->osc1_adsr_sustain << 4) | patch->osc1_adsr_release;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));


  SID_msg.reg_addr = SID_OSC2_PW_LO;
  SID_msg.reg_data = patch->osc2_pw & 255;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC2_PW_HI;
  SID_msg.reg_data = patch->osc2_pw >> 8;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC2_ATTACK;
  SID_msg.reg_data = (patch->osc2_adsr_attack << 4) | patch->osc2_adsr_decay;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC2_SUSTAIN;
  SID_msg.reg_data = (patch->osc2_adsr_sustain << 4) | patch->osc2_adsr_release;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));


  SID_msg.reg_addr = SID_OSC3_PW_LO;
  SID_msg.reg_data = patch->osc3_pw & 255; /* get lower byte */
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC3_PW_HI;
  SID_msg.reg_data = patch->osc3_pw >> 8;  /* shift the rest */
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC3_ATTACK;
  SID_msg.reg_data = (patch->osc3_adsr_attack << 4) | patch->osc3_adsr_decay;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_OSC3_SUSTAIN;
  SID_msg.reg_data = (patch->osc3_adsr_sustain << 4) | patch->osc3_adsr_release;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t)); 

  
  SID_msg.reg_addr = SID_FLT_CUTOFF_LO;
  SID_msg.reg_data = patch->filter_cutoff & 255;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_FLT_CUTOFF_HI;
  SID_msg.reg_data = patch->filter_cutoff >> 3;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_FLT_RESO_ROUTE;
  SID_msg.reg_data = (patch->filter_reso << 4) | patch->osc1_filter_on | patch->osc2_filter_on | patch->osc3_filter_on;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_FLT_MODE_VOL;
  SID_msg.reg_data = patch->filter_mode | patch->volume;
  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }


/* detune is calculated in SID register values, and it's base unit translates roughly to 1/10 of adjacent note interval (10 cents)
 detune can be anything from -10 to 10, which means that maximal detune value should set oscillator to the adjacent note pitch (+/- 100 cents in 10 cent units)
*/

int16_t LIB_SID_oscillator_detune(uint8_t MIDI_note, int8_t osc_detune)
 {

  float note_interval;

  if(!osc_detune) return;

  if(osc_detune > 0 )
    {
       note_interval = G_MIDI_to_SID_reg[(MIDI_note + 1)] - G_MIDI_to_SID_reg[(MIDI_note)];
       return  (note_interval / 10) * osc_detune;
    }
  else
   {
      note_interval = G_MIDI_to_SID_reg[(MIDI_note)] - G_MIDI_to_SID_reg[(MIDI_note - 1)];
      return  (note_interval / 10) * osc_detune;
   }

 }



void LIB_SID_note_on(uint8_t MIDI_note, uint8_t v_id, uint8_t board_address)
 {

   int8_t detune_osc1, detune_osc2, detune_osc3;
   uint8_t adsr_start_msg = 1;

   SID_msg_t SID_OSC1_msg, SID_OSC2_msg, SID_OSC3_msg;
 
#ifdef USE_SILENCERS 
   /* reset silencers for this voice */

   pthread_kill(G_QSID_tasks[(1 << 4) | v_id].task_id, SIGALRM);
   pthread_kill(G_QSID_tasks[(2 << 4) | v_id].task_id, SIGALRM);
   pthread_kill(G_QSID_tasks[(3 << 4) | v_id].task_id, SIGALRM);

#endif

   SID_OSC1_msg.SID_addr = SID_OSC2_msg.SID_addr = SID_OSC3_msg.SID_addr = board_address;

   /* turn off all oscillators using test bit */

   //SID_OSC1_msg.reg_data = 8;
   //SID_OSC1_msg.reg_addr = SID_OSC1_CTRL;
   //write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC1_msg, sizeof(SID_msg_t));

   //SID_OSC2_msg.reg_data = 8;
   //SID_OSC2_msg.reg_addr = SID_OSC2_CTRL;
   //write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));

   //SID_OSC3_msg.reg_data = 8;
   //SID_OSC3_msg.reg_addr = SID_OSC3_CTRL;
   //write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC3_msg, sizeof(SID_msg_t));

   /* load pitch registers (+detune value from patch), and then trigger ADSR on all three oscillators at once */
   
   SID_OSC1_msg.reg_data = G_MIDI_to_SID_reg[MIDI_note+G_current_patch.octave_transposition] + LIB_SID_oscillator_detune(MIDI_note, G_current_patch.osc1_fine);
   SID_OSC1_msg.reg_addr = SID_OSC1_FREQ_LO;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC1_msg, sizeof(SID_msg_t));

   SID_OSC2_msg.reg_data = G_MIDI_to_SID_reg[MIDI_note+G_current_patch.octave_transposition] + LIB_SID_oscillator_detune(MIDI_note, G_current_patch.osc2_fine);
   SID_OSC2_msg.reg_addr = SID_OSC2_FREQ_LO;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));

   SID_OSC3_msg.reg_data = G_MIDI_to_SID_reg[MIDI_note+G_current_patch.octave_transposition] + LIB_SID_oscillator_detune(MIDI_note, G_current_patch.osc3_fine);
   SID_OSC3_msg.reg_addr = SID_OSC3_FREQ_LO;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC3_msg, sizeof(SID_msg_t));

   SID_OSC1_msg.reg_data = G_current_patch.osc1_control_reg | 1;  /* set GATE bit */
   SID_OSC2_msg.reg_data = G_current_patch.osc2_control_reg | 1;  
   SID_OSC3_msg.reg_data = G_current_patch.osc3_control_reg | 1;

   SID_OSC1_msg.reg_addr = SID_OSC1_CTRL;
   SID_OSC2_msg.reg_addr = SID_OSC2_CTRL;
   SID_OSC3_msg.reg_addr = SID_OSC3_CTRL;

   write(G_QSID_tasks[TASK_FILTER_ADSR_V1+(v_id-1)].input_pipe[1], &adsr_start_msg, 1);
   
   if(G_current_patch.osc1_on) 
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC1_msg, sizeof(SID_msg_t));

   if(G_current_patch.osc2_on)
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC2_msg, sizeof(SID_msg_t));

   if(G_current_patch.osc3_on)
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_OSC3_msg, sizeof(SID_msg_t));

 }


void LIB_SID_OSC1_note_on(uint16_t SID_osc_pitch, uint8_t v_id, uint8_t board_address)
 {
 
   /* set oscillator pitch and set GATE bit on oscillator 1 */

   SID_msg_t SID_msg;

   SID_msg.reg_data = SID_osc_pitch;
   SID_msg.reg_addr = SID_OSC1_FREQ_LO;
   SID_msg.SID_addr = board_address;

   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_data = 17;
   SID_msg.reg_addr = SID_OSC1_CTRL;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }

void LIB_SID_OSC2_note_on(uint16_t SID_osc_pitch, uint8_t v_id, uint8_t board_address)
 {

   /* set oscillator pitch and set GATE bit on oscillator 2 */
 
   SID_msg_t SID_msg;

   SID_msg.reg_data = SID_osc_pitch;
   SID_msg.reg_addr = SID_OSC2_FREQ_LO;
   SID_msg.SID_addr = board_address;

   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_data = 17;
   SID_msg.reg_addr = SID_OSC2_CTRL;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }

void LIB_SID_OSC3_note_on(uint16_t SID_osc_pitch, uint8_t v_id, uint8_t board_address)
 {

   /* set oscillator pitch and set GATE bit on oscillator 3 */

   SID_msg_t SID_msg;

   SID_msg.reg_data = SID_osc_pitch;
   SID_msg.reg_addr = SID_OSC3_FREQ_LO;
   SID_msg.SID_addr = board_address;

   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_data = 17;
   SID_msg.reg_addr = SID_OSC3_CTRL;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

 }


void LIB_SID_note_off(uint8_t v_id, uint8_t board_address, uint8_t kill_note)
 {

  /* clear GATE bit on active oscillators, notify silencer threads, notify filter ADSR */
  
  SID_msg_t SID_msg;
  uint8_t silencer_msg = 1;

  SID_msg.SID_addr = board_address;

  if(kill_note)
   pthread_kill(G_QSID_tasks[TASK_FILTER_ADSR_V1+(v_id-1)].task_id, SIGUSR1);
  else
   pthread_kill(G_QSID_tasks[TASK_FILTER_ADSR_V1+(v_id-1)].task_id, SIGUSR2);

  if(G_current_patch.osc1_on)
   {
    SID_msg.reg_data = G_current_patch.osc1_control_reg;
    SID_msg.reg_addr = SID_OSC1_CTRL;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t)); 
#ifdef USE_SILENCERS
    if(!kill_note)
     write(G_QSID_tasks[(1 << 4) | v_id].input_pipe[1], &silencer_msg, 1);
#endif
   }

  if(G_current_patch.osc2_on)
   {
    SID_msg.reg_data = G_current_patch.osc2_control_reg;
    SID_msg.reg_addr = SID_OSC2_CTRL;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
#ifdef USE_SILENCERS
    if(!kill_note)
     write(G_QSID_tasks[(2 << 4) | v_id].input_pipe[1], &silencer_msg, 1);
#endif
   }
 
  if(G_current_patch.osc3_on)
   {
    SID_msg.reg_data = G_current_patch.osc3_control_reg;
    SID_msg.reg_addr = SID_OSC3_CTRL;
    write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
#ifdef USE_SILENCERS
    if(!kill_note)
     write(G_QSID_tasks[(3 << 4) | v_id].input_pipe[1], &silencer_msg, 1);
#endif
   }

 }


void LIB_SID_modulate_cutoff(uint8_t board_address, uint16_t apply_value)
 {
  SID_msg_t SID_msg;
  uint8_t cutoff_lo, cutoff_hi;

  SID_msg.SID_addr = board_address;

  cutoff_lo = apply_value & 7; /* lowest three bits */
  cutoff_hi = apply_value  >> 3;  /* shift out three bits */

  SID_msg.reg_addr = SID_FLT_CUTOFF_LO;
  SID_msg.reg_data = cutoff_lo;

  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

  SID_msg.reg_addr = SID_FLT_CUTOFF_HI;
  SID_msg.reg_data = cutoff_hi;

  write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
 }

void LIB_SID_modulate_PW1(uint8_t board_address, uint16_t apply_value)
 {
   SID_msg_t SID_msg;
   uint8_t pw_lo, pw_hi;
   uint8_t pw_mask = 255;

   pw_lo = apply_value & pw_mask;
   pw_hi = apply_value >> 8;

   SID_msg.SID_addr = board_address;

   SID_msg.reg_addr = SID_OSC1_PW_LO;
   SID_msg.reg_data = pw_lo;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC1_PW_HI;
   SID_msg.reg_data = pw_hi;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
 }


void LIB_SID_modulate_PW2(uint8_t board_address, uint16_t apply_value)
 {
   SID_msg_t SID_msg;
   uint8_t pw_lo, pw_hi;
   uint8_t pw_mask = 255;

   pw_lo = apply_value & pw_mask;
   pw_hi = apply_value >> 8;

   SID_msg.SID_addr = board_address;

   SID_msg.reg_addr = SID_OSC2_PW_LO;
   SID_msg.reg_data = pw_lo;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC2_PW_HI;
   SID_msg.reg_data = pw_hi;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
 }

void LIB_SID_modulate_PW3(uint8_t board_address, uint16_t apply_value)
 {
   SID_msg_t SID_msg;
   uint8_t pw_lo, pw_hi;
   uint8_t pw_mask = 255;

   pw_lo = apply_value & pw_mask;
   pw_hi = apply_value >> 8;

   SID_msg.SID_addr = board_address;

   SID_msg.reg_addr = SID_OSC3_PW_LO;
   SID_msg.reg_data = pw_lo;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));

   SID_msg.reg_addr = SID_OSC3_PW_HI;
   SID_msg.reg_data = pw_hi;
   write(G_QSID_tasks[TASK_SID_WRITER].input_pipe[1], &SID_msg, sizeof(SID_msg_t));
 }

void LIB_SID_modulate_PW_ALL(uint8_t board_address, uint16_t apply_value)
 {
  LIB_SID_modulate_PW1(board_address, apply_value);
  LIB_SID_modulate_PW2(board_address, apply_value);
  LIB_SID_modulate_PW3(board_address, apply_value);
 }

