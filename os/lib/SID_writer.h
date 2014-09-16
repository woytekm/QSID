//
//
//

typedef struct _SID_msg {
        uint8_t SID_addr;
        uint8_t reg_addr;
        uint16_t reg_data;
        uint8_t prio;
       } SID_msg_t;


void LIB_SID_tx_thread(void);

