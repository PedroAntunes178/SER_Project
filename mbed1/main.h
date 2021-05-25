 #include "mbed.h"

 Ticker ticker;
 DigitalOut led1(LED1);
 DigitalOut led2(LED2);
 CAN can1(p9, p10);
 CAN can2(p30, p29);
 char counter = 0;
