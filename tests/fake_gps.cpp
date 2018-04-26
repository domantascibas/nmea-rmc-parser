#include "mbed.h"

/* 

To run this test, two microcontrollers are required:
- one is for running the main.cpp from the 'src' folder
- the other one is for running this "fake gps" data

 */

DigitalOut led(LED1);
Serial sender(PA_9, PA_10);

int main(void) {
  led = 0;
  sender.baud(9600);

  while(1) {
    led = !led;
    // sender.printf("$GPRMB,A,4.08,L,EGLL,EGLM,5130.02,N,00046.34,W,004.6,213.9,122.9,A*3D\r\n");
    // sender.printf("$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68\r\n");
    // sender.printf("$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62\r\n");
    sender.printf("$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70\r\n");
    // sender.printf("$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n");
    // sender.printf("$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A,V*57\r\n");
    wait(1);
  }
  return 0;
}