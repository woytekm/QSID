/* flags indicating events */
 
sig_atomic_t G_V_ADSR_stop[9];

sig_atomic_t G_V_ADSR_release_start[9];

/* signal handler arrays */

void (*G_ADSR_release_SH[9])(int);
void (*G_ADSR_stop_SH[9])(int);

/* modulation steps & intervals */

static const uint16_t G_ADSR_attack_steps[16] = {5,10,20,25,25,25,25,25,25,25,25,25,50,50,50,50};
static const uint32_t G_ADSR_attack_intervals[16] = {400,800,800,960,1520,2240,2720,3200,4000,10000,20000,
                                                     32000,20000,60000,100000,160000};
static const uint16_t G_ADSR_attack_step_size[16] = {409,204,102,81,81,81,81,81,81,81,81,81,40,40,40,40};

#define ADSR_ATTACK_STEPS_0 5
#define ADSR_ATTACK_STEPS_1 10
#define ADSR_ATTACK_STEPS_2 20
#define ADSR_ATTACK_STEPS_3 25
#define ADSR_ATTACK_STEPS_4 25
#define ADSR_ATTACK_STEPS_5 25
#define ADSR_ATTACK_STEPS_6 25
#define ADSR_ATTACK_STEPS_7 25
#define ADSR_ATTACK_STEPS_8 25
#define ADSR_ATTACK_STEPS_9 25
#define ADSR_ATTACK_STEPS_10 25
#define ADSR_ATTACK_STEPS_11 25
#define ADSR_ATTACK_STEPS_12 50
#define ADSR_ATTACK_STEPS_13 50
#define ADSR_ATTACK_STEPS_14 50
#define ADSR_ATTACK_STEPS_15 50

#define ADSR_ATTACK_MOD_INTERVAL_0 400     /* 2ms */
#define ADSR_ATTACK_MOD_INTERVAL_1 800     /* 8ms */
#define ADSR_ATTACK_MOD_INTERVAL_2 800     /* 16ms */
#define ADSR_ATTACK_MOD_INTERVAL_3 960     /* 24ms */
#define ADSR_ATTACK_MOD_INTERVAL_4 1520    /* 38ms */
#define ADSR_ATTACK_MOD_INTERVAL_5 2240    /* 56ms */
#define ADSR_ATTACK_MOD_INTERVAL_6 2720    /* 68ms */
#define ADSR_ATTACK_MOD_INTERVAL_7 3200    /* 80ms */
#define ADSR_ATTACK_MOD_INTERVAL_8 4000    /* 100ms */
#define ADSR_ATTACK_MOD_INTERVAL_9 10000    /* 250ms */
#define ADSR_ATTACK_MOD_INTERVAL_10 20000   /* 500ms */
#define ADSR_ATTACK_MOD_INTERVAL_11 32000   /* 800ms */
#define ADSR_ATTACK_MOD_INTERVAL_12 20000   /* 1s    */
#define ADSR_ATTACK_MOD_INTERVAL_13 60000   /* 3s    */
#define ADSR_ATTACK_MOD_INTERVAL_14 100000   /* 5s    */
#define ADSR_ATTACK_MOD_INTERVAL_15 160000   /* 8s    */

#define ADSR_ATTACK_STEP_SIZE_0 409
#define ADSR_ATTACK_STEP_SIZE_1 204
#define ADSR_ATTACK_STEP_SIZE_2 102
#define ADSR_ATTACK_STEP_SIZE_3 81
#define ADSR_ATTACK_STEP_SIZE_4 81
#define ADSR_ATTACK_STEP_SIZE_5 81
#define ADSR_ATTACK_STEP_SIZE_6 81
#define ADSR_ATTACK_STEP_SIZE_7 81
#define ADSR_ATTACK_STEP_SIZE_8 81
#define ADSR_ATTACK_STEP_SIZE_9 81
#define ADSR_ATTACK_STEP_SIZE_10 81
#define ADSR_ATTACK_STEP_SIZE_11 81
#define ADSR_ATTACK_STEP_SIZE_12 40
#define ADSR_ATTACK_STEP_SIZE_13 40
#define ADSR_ATTACK_STEP_SIZE_14 40
#define ADSR_ATTACK_STEP_SIZE_15 40


#define ADSR_DECAY_STEPS_1 3
#define ADSR_DECAY_STEPS_2 7
#define ADSR_DECAY_STEPS_3 15
#define ADSR_DECAY_STEPS_4 20
#define ADSR_DECAY_STEPS_5 30
 
#define ADSR_RELEASE_STEPS_1 3
#define ADSR_RELEASE_STEPS_2 7
#define ADSR_RELEASE_STEPS_3 15
#define ADSR_RELEASE_STEPS_4 20
#define ADSR_RELEASE_STEPS_5 30
 

/* prototypes */

void SYNTH_filter_ADSR(void *arg);
void SYNTH_init_ADSR_event_handlers(void);
void SYNTH_init_ADSR_arrays(void);

