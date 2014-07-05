//
// Q-SID OS  basic user input / output
//
//

#include "common.h"

void SYS_user_error(char *error_msg)
 {
  fprintf(stderr,"USER: ");
  fprintf(stderr,error_msg);
  fprintf(stderr,"\n");

 }


