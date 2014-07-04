//
// Q-SID OS
//
// polyphony manager data structures
//


typedef struct {
 _voice_t *next;
 uint8_t index;
 uint8_t state;
} _voice_t;
