//
// Q-SID OS  miscellaneous system routines
//
//
//

#include "common.h"
#include "vars.h"
#include "qsid-config.h"


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


void SYS_debug(char *debug_msg, ...)
 {

#ifdef QSID_DEBUG
  va_list ap;
  char message_cr[MAXMSG];

  snprintf(message_cr,MAXMSG,"%s\n",debug_msg);
  va_start(ap, debug_msg);
  vfprintf(stderr, message_cr, ap);
  va_end(ap);
#endif
   
 }


