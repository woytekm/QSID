/*
 
Q-SID OS
Sysex message types
 
*/ 

typedef uint8_t (*callback_ptr_t)(unsigned char*, uint16_t);

callback_ptr_t G_sysex_handlers[127];

#define QSID_SYSEX_MANUFACTURERS_ID 0x77F /* 1919 decimal, two bytes */

#define SYSEX_DEV_ID_POS 3
#define SYSEX_CODE_POS 4
#define SYSEX_DATA_POS 5
#define SYSEX_CONTROL_BYTES 6

/* sysex message structure:                             */
/* message_buffer[0] = 0xF0                             */
/* message_buffer[1] = manufacturer's ID 0x07           */
/* message_buffer[2] = manufacturer's ID 0x7F           */
/* message_buffer[3] = device ID (MIDI channel 0 - 15)  */
/* message_buffer[4] = sysex code                       */
/* message_buffer[5,6,...,buffer_len] = sysex subcode (optional) and encoded value for code/subcode */

/* lengths of messages are counted with heading/trailing 0xF0 0xF7, manufacturer's ID, sysex code, device ID  - entire sysex msg */


#define SYSEX_COMP_ATTACK      0x1A
#define SYSEX_COMP_ATTACK_LEN  8

#define SYSEX_COMP_DECAY       0x1B
#define SYSEX_COMP_DECAY_LEN   8

#define SYSEX_COMP_SUSTAIN     0x1C
#define SYSEX_COMP_SUSTAIN_LEN 8

#define SYSEX_COMP_RELEASE     0x1D
#define SYSEX_COMP_RELEASE_LEN 8

#define SYSEX_OSC1_STATE       0x1E
#define SYSEX_OSC1_STATE_LEN   8


#define SYSEX_OSC1_FINE        0x1F
#define SYSEX_OSC1_FINE_LEN    8

#define SYSEX_OSC1_COARSE      0x20
#define SYSEX_OSC1_COARSE_LEN  8

#define SYSEX_OSC1_PW          0x21
#define SYSEX_OSC1_PW_LEN      8

#define SYSEX_OSC1_WAVE        0x22
#define SYSEX_OSC1_WAVE_LEN    8

#define SYSEX_OSC1_RINGMOD     0x23
#define SYSEX_OSC1_RINGMOD_LEN 8

#define SYSEX_OSC1_SYNC        0x24
#define SYSEX_OSC1_SYNC_LEN    8

#define SYSEX_OSC1_ATTACK      0x25
#define SYSEX_OSC1_ATTACK_LEN  8

#define SYSEX_OSC1_DECAY       0x26
#define SYSEX_OSC1_DECAY_LEN   8

#define SYSEX_OSC1_SUSTAIN     0x27
#define SYSEX_OSC1_SUSTAIN_LEN 8

#define SYSEX_OSC1_RELEASE     0x28
#define SYSEX_OSC1_RELEASE_LEN 8

#define SYSEX_OSC1_FILTER      0x29
#define SYSEX_OSC1_FILTER_LEN  8



#define SYSEX_OSC2_STATE       0x33
#define SYSEX_OSC2_STATE_LEN   8

#define SYSEX_OSC2_FINE        0x34
#define SYSEX_OSC2_FINE_LEN    8

#define SYSEX_OSC2_COARSE      0x35
#define SYSEX_OSC2_COARSE_LEN  8

#define SYSEX_OSC2_PW          0x36
#define SYSEX_OSC2_PW_LEN      8

#define SYSEX_OSC2_WAVE        0x37
#define SYSEX_OSC2_WAVE_LEN    8

#define SYSEX_OSC2_RINGMOD     0x38
#define SYSEX_OSC2_RINGMOD_LEN 8

#define SYSEX_OSC2_SYNC        0x39
#define SYSEX_OSC2_SYNC_LEN    8

#define SYSEX_OSC2_ATTACK      0x3A
#define SYSEX_OSC2_ATTACK_LEN  8

#define SYSEX_OSC2_DECAY       0x3B
#define SYSEX_OSC2_DECAY_LEN   8

#define SYSEX_OSC2_SUSTAIN     0x3C
#define SYSEX_OSC2_SUSTAIN_LEN 8

#define SYSEX_OSC2_RELEASE     0x3D
#define SYSEX_OSC2_RELEASE_LEN 8

#define SYSEX_OSC2_FILTER      0x3E
#define SYSEX_OSC2_FILTER_LEN  8



#define SYSEX_OSC3_STATE       0x48
#define SYSEX_OSC3_STATE_LEN   8

#define SYSEX_OSC3_FINE        0x49
#define SYSEX_OSC3_FINE_LEN    8

#define SYSEX_OSC3_COARSE      0x4A
#define SYSEX_OSC3_COARSE_LEN  8

#define SYSEX_OSC3_PW          0x4B
#define SYSEX_OSC3_PW_LEN      8

#define SYSEX_OSC3_WAVE        0x4C
#define SYSEX_OSC3_WAVE_LEN    8

#define SYSEX_OSC3_RINGMOD     0x4D
#define SYSEX_OSC3_RINGMOD_LEN 8

#define SYSEX_OSC3_SYNC        0x4E
#define SYSEX_OSC3_SYNC_LEN    8

#define SYSEX_OSC3_ATTACK      0x4F
#define SYSEX_OSC3_ATTACK_LEN  8

#define SYSEX_OSC3_DECAY       0x50
#define SYSEX_OSC3_DECAY_LEN   8

#define SYSEX_OSC3_SUSTAIN     0x51
#define SYSEX_OSC3_SUSTAIN_LEN 8

#define SYSEX_OSC3_RELEASE     0x52
#define SYSEX_OSC3_RELEASE_LEN 8

#define SYSEX_OSC3_FILTER      0x53
#define SYSEX_OSC3_FILTER_LEN  8



#define SYSEX_FLT_MODE         0x5D
#define SYSEX_FLT_MODE_LEN     8

#define SYSEX_FLT_RESO         0x5E
#define SYSEX_FLT_RESO_LEN     8

#define SYSEX_FLT_CUTOFF       0x5F
#define SYSEX_FLT_CUTOFF_LEN   8

#define SYSEX_FLT_EXT          0x60
#define SYSEX_FLT_EXT_LEN      8

#define SYSEX_VOLUME           0x61
#define SYSEX_VOLUME_LEN       8

#define SYSEX_TRANSPOSE        0x62
#define SYSEX_TRANSPOSE_LEN    8



#define SYSEX_LFO1_STATE       0x63
#define SYSEX_LFO1_STATE_LEN   8

#define SYSEX_LFO1_SHAPE       0x64
#define SYSEX_LFO1_SHAPE_LEN   8

#define SYSEX_LFO1_RATE        0x65
#define SYSEX_LFO1_RATE_LEN    8

#define SYSEX_LFO1_DEPTH       0x66
#define SYSEX_LFO1_DEPTH_LEN   8

#define SYSEX_LFO1_KEYSYNC     0x67
#define SYSEX_LFO1_KEYSYNC_LEN 8

#define SYSEX_LFO1_ROUTE       0x68
#define SYSEX_LFO1_ROUTE_LEN   8



#define SYSEX_LFO2_STATE       0x69
#define SYSEX_LFO2_STATE_LEN   8

#define SYSEX_LFO2_SHAPE       0x6A
#define SYSEX_LFO2_SHAPE_LEN   8

#define SYSEX_LFO2_RATE        0x6B
#define SYSEX_LFO2_RATE_LEN    8

#define SYSEX_LFO2_DEPTH       0x6C
#define SYSEX_LFO2_DEPTH_LEN   8

#define SYSEX_LFO2_KEYSYNC     0x6D
#define SYSEX_LFO2_KEYSYNC_LEN 8

#define SYSEX_LFO2_ROUTE       0x6E
#define SYSEX_LFO2_ROUTE_LEN   8



#define SYSEX_TRANSMT_BANK      0x6F
#define SYSEX_TRANSMT_BANK_DTA  0x70
#define SYSEX_TRANSMT_PATCH     0x71
#define SYSEX_TRANSMT_PATCH_DTA 0x72
#define SYSEX_RECV_BANK         0x73
#define SYSEX_RECV_PATCH        0X74


