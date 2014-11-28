/*
 
Q-SID OS
Sysex message types
 
*/ 

typedef uint8_t (*callback_ptr_t)(unsigned char*, uint16_t);

callback_ptr_t G_sysex_callbacks[127];

#define QSID_SYSEX_MANUFACTURERS_ID 0x1   /* pretend we are Sequential Circuits synth  */

#define SYSEX_OSC1_STATE       0x1E
#define SYSEX_OSC1_STATE_LEN   5

#define SYSEX_OSC1_FINE        0x1F
#define SYSEX_OSC1_FINE_LEN    5

#define SYSEX_OSC1_COARSE      0x20
#define SYSEX_OSC1_COARSE_LEN  5

#define SYSEX_OSC1_PW          0x21
#define SYSEX_OSC1_PW_LEN      6

#define SYSEX_OSC1_WAVE        0x22
#define SYSEX_OSC1_RINGMOD     0x23
#define SYSEX_OSC1_SYNC        0x24
#define SYSEX_OSC1_ATTACK      0x25
#define SYSEX_OSC1_DECAY       0x26
#define SYSEX_OSC1_SUSTAIN     0x27
#define SYSEX_OSC1_RELEASE     0x28
#define SYSEX_OSC1_FILTER      0x29

#define SYSEX_OSC2_STATE       0x33
#define SYSEX_OSC2_FINE        0x34
#define SYSEX_OSC2_COARSE      0x35
#define SYSEX_OSC2_PW          0x36
#define SYSEX_OSC2_WAVE        0x37
#define SYSEX_OSC2_RINGMOD     0x38
#define SYSEX_OSC2_SYNC        0x39
#define SYSEX_OSC2_ATTACK      0x3A
#define SYSEX_OSC2_DECAY       0x3B
#define SYSEX_OSC2_SUSTAIN     0x3C
#define SYSEX_OSC2_RELEASE     0x3D
#define SYSEX_OSC2_FILTER      0x3E

#define SYSEX_OSC3_STATE       0x48
#define SYSEX_OSC3_FINE        0x49
#define SYSEX_OSC3_COARSE      0x4A
#define SYSEX_OSC3_PW          0x4B
#define SYSEX_OSC3_WAVE        0x4C
#define SYSEX_OSC3_RINGMOD     0x4D
#define SYSEX_OSC3_SYNC        0x4E
#define SYSEX_OSC3_ATTACK      0x4F
#define SYSEX_OSC3_DECAY       0x50
#define SYSEX_OSC3_SUSTAIN     0x51
#define SYSEX_OSC3_RELEASE     0x52
#define SYSEX_OSC3_FILTER      0x53

#define SYSEX_FLT_MODE         0x5D
#define SYSEX_FLT_RESO         0x5E
#define SYSEX_FLT_CUTOFF       0x5F
#define SYSEX_FLT_EXT          0x60

#define SYSEX_VOLUME           0x61
#define SYSEX_TRANSPOSE        0x62

#define SYSEX_LFO1_STATE       0x63
#define SYSEX_LFO1_SHAPE       0x64
#define SYSEX_LFO1_RATE        0x65
#define SYSEX_LFO1_DEPTH       0x66
#define SYSEX_LFO1_KEYSYNC     0x67
#define SYSEX_LFO1_ROUTE       0x68

#define SYSEX_LFO2_STATE       0x69
#define SYSEX_LFO2_SHAPE       0x6A
#define SYSEX_LFO2_RATE        0x6B
#define SYSEX_LFO2_DEPTH       0x6C
#define SYSEX_LFO2_KEYSYNC     0x6D
#define SYSEX_LFO2_ROUTE       0x6E

#define SYSEX_TRANSMT_BANK      0x6F
#define SYSEX_TRANSMT_BANK_DTA  0x70
#define SYSEX_TRANSMT_PATCH     0x71
#define SYSEX_TRANSMT_PATCH_DTA 0x72
#define SYSEX_RECV_BANK         0x73
#define SYSEX_RECV_PATCH        0X74


