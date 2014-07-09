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
  struct voice_info *next;
 } voice_info_t;

//
// begin of the voice board inventory linked list
//

struct voice_info_t *G_voice_inventory;
struct voice_info_t *SYS_detect_voices(void);

