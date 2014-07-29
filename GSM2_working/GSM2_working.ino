/* 
LJ:
I have edited GM862.h and GM862.cpp
changing all NewSoftSeria to SotwareSerial
WConstans.h to Arduino.h

And In GM862 a print command to a write command because BYTE was not supported, instead I have used write(char(26))
*/


#include <SoftwareSerial.h>
#include <string.h>
#include "GM862.h"
#define BUF_LENGTH 100
#define txGprs_pin 3
#define rxGprs_pin 2
SoftwareSerial ser(rxGprs_pin,txGprs_pin);
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
modem.sendSMS("+56999915809", "Prueba N2");
digitalWrite(ST_LED_pin,HIGH);
delay(2000);
modem.switchOff();
while(1);
}

