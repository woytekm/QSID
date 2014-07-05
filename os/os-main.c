//
// Q-SID OS main
//

#include "common.h"
#include "vars.h"

#include "qsid-config.h"

#include "inventory.h"

#include "i2c.h"


//
// OS main
//

void main(void)

 {

  // open i2c control files for voice and aux bus

  if( (G_i2c_voice_bus = LIB_i2c_open(I2C_VOICE_BUS)) < 0 )  
   {
    SYS_user_error("I2C (voice) open error!", ERROR_FATAL);
    SYS_halt();
   }
  else G_inventory_i2c_voice = 1;

  if( (G_i2c_aux_bus = LIB_i2c_open(I2C_AUX_BUS)) < 0 )  
   {
    SYS_user_error("I2C (aux) open error!", ERROR_NONFATAL);
    G_inventory_i2c_aux = -1;
   }


 // detect voice boards - return address to voice inventory linked list

 if( (G_voice_inventory = SYS_detect_voices()) == NULL )
  {
   SYS_user_error("No voice boards detected or detect error!", ERROR_FATAL);
   SYS_halt();
  } 


 }



