/*
 Q-SID OS

 I2C I/O
*/

#ifndef I2C_BCM2835

#include "common.h"
#include "defs.h"
#include "qsid-config.h"

int8_t LIB_i2c_open(char *i2c_file_path)
 {
    int8_t i2c_descriptor;

    if (( i2c_descriptor = open(i2c_file_path, O_RDWR)) < 0) {
       SYS_debug(DEBUG_LOW,"Unable to open i2c bus at %s (%d)!", i2c_file_path, errno);
       return -1;
    }
   
   return i2c_descriptor;

 }

#endif

