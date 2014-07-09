//
// Q-SID OS  basic user input / output
//
//

#include "common.h"
#include "vars.h"

void SYS_user_error(char *error_msg, ...)
 {
  va_list ap;
  char message_cr[MAXMSG];

  snprintf(message_cr,MAXMSG,"USER: %s\n",error_msg);
  va_start(ap, error_msg);
  vfprintf(stderr, message_cr, ap);
  va_end(ap);
 }


