/*
 Q-SID OS  miscellaneous system routines


*/

#include "common.h"
#include "defs.h"
#include "inventory.h"
#include "QSID_config.h"
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



uint64_t SYS_get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

