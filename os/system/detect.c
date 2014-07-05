//
// Q-SID OS
//
// hardware autodetection routines
//

#include "common.h"

#include "inventory.h"
#include "qsid-config.h"
#include "i2c.h"
#include "MCP23017.h"


//
// this will hopefully autodetect voice boards and fill in G_voice_inventory struct
//

voice_info *SYS_detect_voices(void) 
 {

    uint8_t ioaddr = MCP23017_FIRSTADDR; 

    while( ioaddr <= MCP23017_LASTADDR )
     {

       //set all lines of MCP23017 to output  
       set_i2c_register(G_i2c_voice_bus, ioaddr, MCP23017_IODIR_A, 0);
       set_i2c_register(G_i2c_voice_bus, ioaddr, MCP23017_IODIR_B, 0);

       ioaddr++;

     }

    //reset SID
    //SID_via_mcp23017_reset(global_i2c_1_descriptor, global_i2c_1_io);

    return NULL;

  }

