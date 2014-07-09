//
// Q-SID OS
//
// SID low level access routines
//

#include "common.h"
#include "MCP23017.h"


int SID_via_mcp23017_reset(uint8_t i2c_dev, uint8_t ic_addr)
 {

    //write reset bit pattern to SID...
    LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b10100000);
    LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b00000000);
    usleep(10); // eight cycles should be enough, but...
    LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b10100000);

    return 0;

 }


