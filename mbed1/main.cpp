#include "mbed.h"


Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
CAN can1(p9, p10);
CAN can2(p30, p29);
char counter = 0;

Serial pc(USBTX, USBRX);//Opens up serial communication through the USB port via the computer

void send() {
    printf("send()\n");
        printf("wloop()\n");
        stdio_mutex.unlock();
        counter++;
        stdio_mutex.lock();
        printf("Message sent: %d\n", counter);
        stdio_mutex.unlock();
    }
    led1 = !led1;
}

int main() {
    pc.printf("main()\n");
    ticker.attach(&send, 1);
    CANMessage msg;
        }
        wait(0.2);
    }
}
