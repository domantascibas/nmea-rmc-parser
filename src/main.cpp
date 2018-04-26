#include "mbed.h"
#include "MessageParser.h"

DigitalOut led(D9);
Serial receiver(PA_9, PA_10);
Serial pc(USBTX, USBRX);

void printData(rmc_t *rmcMessage)
{
  printf("*** DATA RECEIVED ***\r\n");
  printf("rmc time: %d:%d:%d.%d\r\n", rmcMessage->time.hours, rmcMessage->time.minutes, rmcMessage->time.seconds, rmcMessage->time.milliseconds);
  printf("rmc status %c\r\n", rmcMessage->status ? 'A' : 'V');
  printf("rmc latitude %c %ld\r\n", rmcMessage->ns ? 'S' : 'N', rmcMessage->latitude);
  printf("rmc longitude %c %ld\r\n", rmcMessage->ew ? 'W' : 'E', rmcMessage->longitude);
  printf("rmc speed %d kmh\r\n", rmcMessage->speed);
  printf("rmc course %d deg\r\n", rmcMessage->course);
  printf("rmc date %d/%d/%d\r\n", rmcMessage->date.day, rmcMessage->date.month, rmcMessage->date.year);
  printf("\r\n");
}

void Gnss_OnRmcMessageReceived(rmc_t *rmc) {
  printData(rmc);
}

void Uart_OnByteReceived(uint8_t byte) {
  MessageParser_readByte(byte, Gnss_OnRmcMessageReceived);
}

void Rx_interrupt(void) {
  Uart_OnByteReceived(receiver.getc());
}

int main(void) {
  pc.baud(115200);
  receiver.baud(9600);
  receiver.attach(&Rx_interrupt);

  MessageParser_init();
  printf("Start\r\n");

  while(1) {
  }
}