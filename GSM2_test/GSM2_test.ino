#include <SoftwareSerial.h>
#include <string.h>
#include "GM862.h"
#define BUF_LENGTH 100
#define txGprs_pin 3
#define rxGprs_pin 2
NewSoftSerial ser(rxGprs_pin,txGprs_pin);
GM862 modem(&ser);
void setup()
{
Serial.begin(9600);
modem.switchOn();
modem.init();
while (!modem.isRegistered())
{
delay(1000);
modem.checkNetwork();
}
}
void loop()
{
modem.sendSMS("+56998300000", "Hello World!");
digitalWrite(ST_LED_pin,HIGH);
delay(2000);
modem.switchOff();
while(1);
}
