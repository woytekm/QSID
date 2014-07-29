extern int8_t LIB_i2c_open(char *i2c_file_path);
extern int LIB_SID_via_mcp23017_reset(uint8_t i2c_dev, uint8_t ic_addr);
extern int LIB_SID_via_mcp23017_write(uint8_t i2c_dev, uint8_t ic_addr, uint8_t addr, uint8_t data);
extern uint8_t LIB_SID_via_mcp23017_read(uint8_t i2c_dev, uint8_t ic_addr, uint8_t addr);

