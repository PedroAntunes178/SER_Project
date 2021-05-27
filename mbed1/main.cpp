#include "main.h"

int main(){
    printf("main()\n");
    can_thread.start(can_receive);
    ticker.attach(&can_send, 1);

    while (true) {
        ThisThread::sleep_for(500);
    }
}

void can_send(){
    printf("send()\n");
    if (can1.write(CANMessage(1337, &counter, 1))) {
        stdio_mutex.lock();
        printf("wloop()\n");
        stdio_mutex.unlock();
        counter++;
        stdio_mutex.lock();
        printf("Message sent: %d\n", counter);
        stdio_mutex.unlock();
    }
    led1 = !led1;
}

void can_receive(){
    CANMessage msg;
    bool flag = false;
    stdio_mutex.lock();
    printf("loop()\n");
    stdio_mutex.unlock();
    while (true) {
        if (can2.read(msg)) {
          stdio_mutex.lock();
          printf("Message received: %d\n", msg.data[0]);
          stdio_mutex.unlock();
          led2 = !led2;
        }
        ThisThread::sleep_for(200);
    }
}

void process_msg(char *data){
}
