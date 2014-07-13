//
// RPi SID synth defines
//

#define SID_OSC1_FREQ_LO 0
#define SID_OSC1_FREQ_HI 1
#define SID_OSC1_PW_LO 2
#define SID_OSC1_PW_HI 3
#define SID_OSC1_CTRL 4
#define SID_OSC1_ATTACK 5
#define SID_OSC1_SUSTAIN 6

#define SID_OSC2_FREQ_LO 7
#define SID_OSC2_FREQ_HI 8
#define SID_OSC2_PW_LO 9
#define SID_OSC2_PW_HI 10
#define SID_OSC2_CTRL 11
#define SID_OSC2_ATTACK 12
#define SID_OSC2_SUSTAIN 13

#define SID_OSC3_FREQ_LO 14
#define SID_OSC3_FREQ_HI 15
#define SID_OSC3_PW_LO 16
#define SID_OSC3_PW_HI 17
#define SID_OSC3_CTRL 18
#define SID_OSC3_ATTACK 19
#define SID_OSC3_SUSTAIN 20

#define SID_FLT_CUTOFF_LO 21
#define SID_FLT_CUTOFF_HI 22
#define SID_FLT_RESO_ROUTE 23
#define SID_FLT_MODE_VOL 24

#define SID_POTX 25
#define SID_POTY 26
#define SID_OSC3 27
#define SID_ENV3 28

#define SID_MSG_QUEUE_LEN 64


typedef struct _SID_msg {
	uint8_t addr;
        uint16_t data;
        uint8_t prio;
       } SID_msg_t;

typedef struct  _SID_msg_queue_t{
        uint16_t rpos;
        uint16_t wpos;
        SID_msg_t SID_msg_pipe[SID_MSG_QUEUE_LEN];
       } SID_msg_queue_t;

SID_msg_queue_t G_SID_msg_queue;

