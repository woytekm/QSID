//
// Q-SID OS
//
// hardware inventory 
//


int8_t G_inventory_i2c_voice;
int8_t G_inventory_i2c_aux;
uint8_t G_voice_boards;

typedef struct {
  uint8_t address;
  uint8_t state;
  struct voice_info *next;
 } voice_info;

//
// begin of the voice board inventory linked list
//

struct voice_info *G_voice_inventory;

