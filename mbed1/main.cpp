#include "mbed.h"
//#include "C12832.h"

// defines msg
#define SOS 0xff
#define INIT 0xfe
#define END 0xfd
#define OK 0xfc

//C12832 lcd(p5, p7, p6, p8, p11);

//pc mbed USB Slave function
// connect mbed to pc USB
RawSerial pc(USBTX, USBRX);
//mbed LED
DigitalOut led1(LED1);

AnalogIn sensorMQ2(p17);
CAN can(p30, p29);

//constants
const int numReadings = 500;

//MQ2
const float airRatioMQ2 = 10.0;
const float slopeMQ2 = -0.4687;
const float interceptMQ2 = 1.3969;

//globals for the sensor readings
float mq2sensorPPM = 0;

//globals for the R0 values
float r0MQ2 = 0.83142;


//gloabals for alarm values
float alarmMQ2 = 1000;

float calculateR0(AnalogIn s, float ratio) {
        float sensor_volt;
        float rs;
        float sensorValue = 0.0;
        float r0;

        //take 500 sensor readings and add them together
        for(int i = 0; i < numReadings; i++) {
            sensorValue = sensorValue + s.read();
        }

        sensorValue = sensorValue/numReadings;//average sensor value
        sensor_volt = sensorValue * 3.3;
        rs = ((3.3-sensor_volt)/sensor_volt);
        r0 = rs/ratio;
        pc.printf("RO VALUE: %f \n\n", r0);
        return r0;
}

float determinePPM(AnalogIn sensor, float R0, float m, float b) {
        //Slope and y-intercept of ppm graph line, and R0 from previous calculations
        float voltage = sensor.read() * 3.3;
        float RS_gas = ((3.3-voltage)/voltage);
        float ppmRatio = RS_gas/R0;
        float ppm_log = (log10(ppmRatio)-b)/m;
        float ppm = pow(10, ppm_log);
        if(ppm<0){
            ppm = 0.0;
        }
        if(ppm>10000){
            ppm = 10000;
        }
        return ppm;
}

void sendInfo()
{
    char temp = 0;
    led1 = !led1;
    while(pc.readable()) {
        temp = pc.getc();
        if (temp == 'w')
        {
            pc.printf("{\"MQ2\":%f}\n", mq2sensorPPM);
        }
    }
}

// main() runs in its own thread in the OS
int main() {
    //Uncomment if we want to reset R0 from default to our environment
    //r0MQ2 = calculateR0(sensorMQ2, airRatioMQ2);

    pc.attach(&sendInfo, Serial::RxIrq);
    pc.baud(9600);

    char msg;

    while (1) {

        mq2sensorPPM = determinePPM(sensorMQ2, r0MQ2, slopeMQ2, interceptMQ2);
        if(mq2sensorPPM>alarmMQ2) {
          msg = SOS;
          if (can1.write(CANMessage(1337, &msg, 1))) {
              printf("Message sent: %d\n", msg);
          }
        } else {
          msg = OK;
          if (can1.write(CANMessage(1337, &msg, 1))) {
              printf("Message sent: %d\n", msg);
          }
        }
    }
}
