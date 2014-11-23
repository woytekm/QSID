/*
 Q-SID OS

 SID low level access routines
*/

#include "common.h"
#include "defs.h"
#include "MCP23017.h"


int LIB_SID_via_mcp23017_reset(uint8_t i2c_dev, uint8_t ic_addr)
 {

    SYS_debug(DEBUG_HIGH,"LIB_SID_via_mcp23017_reset: resetting SID...");
    /* write reset bit pattern to SID... */
    LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b10100000);
    LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b00000000);
    usleep(10); /* eight cycles should be enough, but... */
    LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, 0b10100000);

    return 0;

 }


int LIB_SID_via_mcp23017_write(uint8_t i2c_dev, uint8_t ic_addr, uint8_t addr, uint8_t data)
 {

  uint8_t cshi_rsthi_mask=160; /* RST = 1, CS = 1 [101xxxxx] where x = address bits  */
  uint8_t cslo_rsthi_mask=128; /* RST = 1, CS = 0 [100xxxxx] where x = address bits  */
 
  uint8_t out_addr = 0; 

  out_addr = cslo_rsthi_mask | addr; /* merge five bits of address with control lines pattern (bitwise OR)  */

  SYS_debug(DEBUG_VERYHIGH,"LIB_SID_via_mcp23017_write: writing [0x%x] to SID at 0x%x",data, ic_addr);

  LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);
  LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_A, data);

  usleep(2); /* sleep two cycles of the clock - this should be enough to get CS=0, CLOCK=1 pattern  */

  out_addr = cshi_rsthi_mask | out_addr;  /* clear write pattern (CS=1)  */
  LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);

 }


uint8_t LIB_SID_via_mcp23017_read(uint8_t i2c_dev, uint8_t ic_addr, uint8_t addr)
 {

  uint8_t cshi_rsthi_mask=224; /* RST = 1, CS = 1 [111xxxxx] where x = address bits  */
  uint8_t cslo_rsthi_mask=192; /* RST = 1, CS = 0 [110xxxxx] where x = address bits  */
 
  uint8_t out_addr = 0; 
  uint8_t data = 0;

  out_addr = cslo_rsthi_mask | addr; /* merge five bits of address with control lines pattern (bitwise OR) */

  SYS_debug(DEBUG_VERYHIGH,"SID_via_mcp23017_read: about to read addr [0x%x] from SID at 0x%x...",addr, ic_addr);

  LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_GPPU_A, 255);
  LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);

  usleep(2); /* sleep two cycles of the clock - this should be enough to get CS=0, CLOCK=1 pattern */

  LIB_get_i2c_register(i2c_dev, ic_addr, MCP23017_GPIO_A, &data);
  out_addr = cshi_rsthi_mask | out_addr;  /* clear write pattern (CS=1)  */
  LIB_set_i2c_register(i2c_dev, ic_addr, MCP23017_OLAT_B, out_addr);

  return data;

 }

