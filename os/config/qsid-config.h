//
// Q-SID OS
//
// system configuration
//

#define QSID_OS_VERSION_MAJOR 0
#define QSID_OS_VERSION_MINOR 1 

#ifdef I2C_BCM2835

#define I2C_VOICE_BUS 1
#define I2C_AUX_BUS 0

#else

#define I2C_VOICE_BUS "/dev/i2c-1"
#define I2C_AUX_BUS   "/dev/i2c-0"

#endif

#define QSID_DEBUG 1
#define QSID_DEBUG_LEVEL 1

