//
// Prototypes for SID control functions
//


void LIB_SID_note_on(uint16_t SID_osc_pitch, uint8_t board_address);
void LIB_SID_OSC1_note_on(uint16_t SID_osc_pitch, uint8_t board_address);
void LIB_SID_OSC2_note_on(uint16_t SID_osc_pitch, uint8_t board_address);
void LIB_SID_OSC3_note_on(uint16_t SID_osc_pitch, uint8_t board_address);
void LIB_SID_note_off(uint8_t board_address);

void LIB_SID_remote_control(void);

