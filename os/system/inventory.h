/*
 Q-SID OS

 hardware inventory 
*/

#define QSID_MAX_VOICES 8

int8_t  G_inventory_i2c_voice;    
int8_t  G_inventory_i2c_aux;      
uint8_t G_inventory_voice_count;  

typedef struct {
  uint8_t address;              /* voice board address (unique ID of voice)  */
  uint8_t state;                /* if this voice is currently playing - this will be more than 0 */
  uint16_t note;                /* currently played note */
  uint64_t playstart;           /* timestamp  */
 } voice_info_t;

voice_info_t G_voice_inventory[QSID_MAX_VOICES];   /* 8 voice polyphony max (hardware limitations of MCP23017) */
uint8_t SYS_detect_voices(void);     


