//
// Q-SID OS
//
// hardware inventory 
//

#define QSID_MAX_VOICES 8

int8_t  G_inventory_i2c_voice;  // do we have I2C bus for voices?
int8_t  G_inventory_i2c_aux;    // do we have auxiliary I2C bus (for DSP and other stuff)?
uint8_t G_inventory_voice_count;  // how much voices do we have?

typedef struct {
  uint8_t address; // voice board address (unique ID of voice)
  uint8_t state;   // if this voice is currently playing - this will be more than 0 (values from 1 to G_inventory_voice_count - if polyphony is in use)
  uint16_t note;   // if it currently plays - which note it plays?
 } voice_info_t;

voice_info_t G_voice_inventory[QSID_MAX_VOICES];   // 8 voice polyphony max (hardware limitations of MCP23017) 
uint8_t SYS_detect_voices(void);     


