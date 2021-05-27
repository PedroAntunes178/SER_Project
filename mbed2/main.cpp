#include "main.h"

int main(){
    // reset the xbees (at least 200ns)
    rst1 = 0;
    ThisThread::sleep_for(1);
    rst1 = 1;
    ThisThread::sleep_for(1);
    printf("main()\n");
    can_thread.start(can_receive);
    ticker.attach(&can_send, 1);

    char a = 0;
    char prev = 0;
    
    while (true) {
        if(xbee1.readable()){
            prev = a;
            a = xbee1.getc(); //XBee read

            if (a != prev){
                if (a < 10){
                    pc.putc(a);
                }
                if (a == 254 || a == 253){
                    pc.putc(0);
                }
            }
        }
        ThisThread::sleep_for(500);
    }
}

void can_send(){
    printf("send()\n");
    if (can1.write(CANMessage(1337, &counter, 1))) {
        printf("wloop()\n");
        counter++;
        printf("Message sent: %d\n", counter);
    }
    led1 = !led1;
}

void can_receive(){
    CANMessage msg;
    bool flag = false;
    printf("loop()\n");
    while (true) {
        if (can2.read(msg)) {
            pc.printf("Message received: %d\n", msg.data[0]);
            led2 = !led2;
        }
        ThisThread::sleep_for(200);
    }
}

void process_msg(char *data){
}
