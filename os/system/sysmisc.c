//
// Q-SID OS  miscellaneous system routines
//
//
//

#include "common.h"
#include "defs.h"
#include "qsid-config.h"
#include "inventory.h"
#include "voice.h"
#include "midi.h"
#include "task.h"


void SYS_halt(void)
 {

   SYS_user_error("Q-SID operating system terminated.");
   exit(-1);

 }


void SYS_error(char *error_msg, ...)
 {

  va_list ap;
  char message_cr[MAXMSG];

  snprintf(message_cr,MAXMSG,"ERROR: %s\n",error_msg);
  va_start(ap, error_msg);
  vfprintf(stderr, message_cr, ap);
  va_end(ap);

 }


void SYS_debug(uint8_t debug_level, char *debug_msg, ...)
 {

#ifdef QSID_DEBUG
  va_list ap;
  char message_cr[MAXMSG];

  if(debug_level <= QSID_DEBUG_LEVEL)
   {
    snprintf(message_cr,MAXMSG,"%s\n",debug_msg);
    va_start(ap, debug_msg);
    vfprintf(stderr, message_cr, ap);
    va_end(ap);
   }
#endif
   
 }


void SYS_init(void)
 {

   G_inventory_voice_count = 0;
   G_inventory_i2c_voice = 0;
   G_inventory_i2c_aux = 0;
   G_playing_voices = 0;
   G_QSID_task_count = 0;
   
 
#ifdef I2C_BCM2835

LIB_i2c_bcm2835_init();
G_i2c_voice_bus = 1;
G_i2c_aux_bus = 0;
G_inventory_i2c_voice = 1;
G_inventory_i2c_aux = 1;

#else

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

#endif

 }


uint64_t SYS_get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

