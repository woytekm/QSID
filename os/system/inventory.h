//
// Q-SID OS
//
// hardware inventory 
//


int8_t  G_inventory_i2c_voice;
int8_t  G_inventory_i2c_aux;
uint8_t G_inventory_voice_count;

typedef struct {
  uint8_t address;
  uint8_t state;
 } voice_info_t;

voice_info_t G_voice_inventory[8];    
uint8_t SYS_detect_voices(void);


