//
// Q-SID OS  miscellaneous system routines
//
//
//

#include "common.h"


void SYS_halt(void)
 {

   SYS_user_error("Q-SID operating system terminated.");
   exit(-1);

 }


void SYS_error(char *error_msg)
 {
 
  fprintf(stderr,error_msg);

 }


