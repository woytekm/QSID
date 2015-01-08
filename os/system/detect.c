/*
 Q-SID OS

 hardware autodetection routines
*/

#include "common.h"
#include "inventory.h"
#include "defs.h"

#include "QSID_config.h"
#include "SID.h"
#include "MCP23017.h"


/*
 this will hopefully autodetect voice boards and fill in G_voice_inventory array
*/

uint8_t SYS_detect_voices(void) 
 {

    uint8_t SID_readin = 0;
    uint8_t loop;

    uint8_t ioaddr = MCP23017_FIRSTADDR; 

    while( ioaddr <= MCP23017_LASTADDR )
     {

       if(LIB_set_i2c_register(G_i2c_voice_bus, ioaddr, MCP23017_IODIR_A, MCP23017_GPIO_OUT))
         SYS_debug(DEBUG_HIGH,"SYS_detect_voices: nothing at 0x%x",ioaddr);
       else
        {
         SYS_debug(DEBUG_NORMAL,"SYS_detect_voices: found possible MCP23017 at 0x%x",ioaddr);
         LIB_set_i2c_register(G_i2c_voice_bus, ioaddr, MCP23017_IODIR_B, MCP23017_GPIO_OUT);
         LIB_SID_via_mcp23017_reset(G_i2c_voice_bus, ioaddr);
         LIB_SID_via_mcp23017_write(G_i2c_voice_bus, ioaddr, SID_OSC3_FREQ_LO, 128); // fire up OSC3 to get some reading from 0x1B

         for(loop = 0; loop <= 10; loop++)
          {
           SID_readin += LIB_SID_via_mcp23017_read(G_i2c_voice_bus, ioaddr, 0x1B); // this _should_ be more than zero
           usleep(1000); 
          }

         SYS_debug(DEBUG_HIGH,"SYS_detect_voices: SID_readin: %d",SID_readin);

         if(SID_readin > 0)
          {
           SYS_debug(DEBUG_LOW,"SYS_detect_voices: detected SID voice board at 0x%x",ioaddr);

           G_inventory_voice_count++;
           G_voice_inventory[G_inventory_voice_count].address = ioaddr;
           G_voice_inventory[G_inventory_voice_count].state = 0;
           LIB_SID_via_mcp23017_reset(G_i2c_voice_bus, ioaddr);
 
          }
        }
         
       ioaddr++;
       SID_readin = 0;

     }

    if(G_inventory_voice_count)
     return 1;
    else
     return 0;

  }

