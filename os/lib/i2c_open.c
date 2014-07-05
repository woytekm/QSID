//
// Q-SID OS
//
// I2C I/O
//

#include "common.h"
#include "vars.h"
#include "qsid-config.h"
#include "i2c.h"


int8_t LIB_i2c_open(char *i2c_file_path)
 {
    int8_t i2c_descriptor;
    char errormsg[MAXERR];

    // Open a connection to I2C bus
    if (( i2c_descriptor = open(i2c_file_path, O_RDWR)) < 0) {
        snprintf(errormsg,MAXERR,"Unable to open i2c bus at %s (%d)!\n", i2c_file_path, errno);
        SYS_error(errormsg);
        return -1;
    }
   
   return i2c_descriptor;

 }


