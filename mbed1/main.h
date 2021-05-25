/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
#endif

#define MBED_CONF_APP_CAN1_RD p30
#define MBED_CONF_APP_CAN1_TD p29
#define MBED_CONF_APP_CAN2_RD p9
#define MBED_CONF_APP_CAN2_TD p10

#define MBED_CONF_APP_XBee_RD p13
#define MBED_CONF_APP_XBee_TD p14
#define MBED_CONF_APP_XBee_reset p11

// defines msg
#define SOS 0xff
#define INIT 0xfe
#define END 0xfd


#include "mbed.h"
#include "rtos.h"


Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);

Thread can_thread;

Serial xbee1(MBED_CONF_APP_XBee_RD, MBED_CONF_APP_XBee_TD); //Creates a variable for serial comunication through pin 9 and 10
Serial pc(USBTX, USBRX);//Opens up serial communication through the USB port via the computer

DigitalOut rst1(MBED_CONF_APP_XBee_reset); //Digital reset for the XBee, 200ns for reset
  
CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
CAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD); 
char counter = 0;

void can_send();
void can_receive();
void process_msg(char *data);