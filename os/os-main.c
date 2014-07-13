//
// Q-SID OS main
//

#include "common.h"
#include "defs.h"

#include "qsid-config.h"
#include "inventory.h"
#include "i2c.h"


//
// OS main
//

void main(void)

 {

  SYS_debug(DEBUG_LOW,"Q-SID OS version %d.%d",QSID_OS_VERSION_MAJOR, QSID_OS_VERSION_MINOR);
  // open i2c control files for voice and aux bus

  SYS_init();

  if( (G_i2c_voice_bus = LIB_i2c_open(I2C_VOICE_BUS)) < 0 )  
   {
    SYS_error("I2C (voice) open error!");
    SYS_halt();
   }

  else G_inventory_i2c_voice = 1;

  if( (G_i2c_aux_bus = LIB_i2c_open(I2C_AUX_BUS)) < 0 )  
   {
    SYS_error("I2C (aux) open error!");
    G_inventory_i2c_aux = 0;
   }
  else G_inventory_i2c_aux = 1;


 // detect voice boards - return address to voice inventory linked list

 if(!SYS_detect_voices())
  {
   SYS_error("No voice boards detected or detect error!");
   SYS_halt();
  } 
 else
  SYS_debug(DEBUG_LOW,"Detected %d voices in this synth",G_inventory_voice_count);


 }



