#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
#endif

#include "mbed.h"
#include "rtos.h"

#define MBED_CONF_APP_CAN1_RD p30
#define MBED_CONF_APP_CAN1_TD p29
#define MBED_CONF_APP_CAN2_RD p9
#define MBED_CONF_APP_CAN2_TD p10

// defines msg
#define SOS 0xff
#define INIT 0xfe
#define END 0xfd

Mutex stdio_mutex;

Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);

Thread can_thread;

CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
CAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD);
char counter = 0;

void can_send();
void can_receive();
void process_msg(char *data);
