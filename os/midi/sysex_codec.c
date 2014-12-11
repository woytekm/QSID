/*

*/

#include "common.h"
#include "defs.h"
#include "sysex.h"

int16_t MIDI_encode_sysex(uint8_t *input_data, uint16_t input_len, uint8_t device_id, uint8_t sysex_code,
                          uint8_t *output_buff)
 {
   uint16_t i = 0,j = SYSEX_DATA_POS; /* set counters to appropriate offsets */

   output_buff[0] = 0xF0;      /* build sysex header */
   output_buff[1] = 0x07;      /* QSID device type 0x77F  - two bytes */
   output_buff[2] = 0x7F;            
   output_buff[3] = device_id;  /* MIDI channel */
   output_buff[4] = sysex_code;

   /* encode data */

   while(i < input_len)
    {
     output_buff[j] = input_data[i] >> 7;        /* MSB */
     output_buff[j+1] = input_data[i] & 127;     /* LSB */
     i++;
     j+=2;
    }

   output_buff[j] = 0xF7;

   return j+1; /* total sysex buffer length */
 }

int16_t MIDI_decode_sysex(uint8_t *input_data, uint16_t input_len, uint8_t *output_buff)
 {
   uint16_t i = SYSEX_DATA_POS, j = SYSEX_DATA_POS;  /* set counters to appropriate offsets */

   if(input_data[0] != 0xF0)
    return 0;

   /* copy sysex magic byte, manufacturer's ID, device ID, and sysex code: */

   output_buff[0] = input_data[0];
   output_buff[1] = input_data[1];
   output_buff[2] = input_data[2];
   output_buff[3] = input_data[3];
   output_buff[4] = input_data[4];

   /* decode data: */

   while(i < input_len - 1) /* do not parse trailing 0xF7 */
    {
     output_buff[j]  = (input_data[i] << 7) | input_data[i+1];  /* merge MSB and LSB */
     j++;
     i+=2;
    }

   output_buff[j] = 0xF7;

   return j+1; /* decoded data length along with the 3-byte header, but without heading/trailing 0xF0/0xF7 */
 }
 

