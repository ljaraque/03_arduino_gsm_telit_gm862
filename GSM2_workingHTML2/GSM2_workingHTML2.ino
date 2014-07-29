/*
File:
GM862_SMS.pde
Author: Moisés Campos
MCI Electronics
www.olimex.cl
*/
#include <SoftwareSerial.h>
#include <string.h>
#include "GM862.h"
#define BUF_LENGTH 100
#define txGprs_pin 3
#define rxGprs_pin 2
SoftwareSerial ser(rxGprs_pin,txGprs_pin);
GM862 modem(&ser);

char generalBuffer[500];

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
modem.settoInternet("bam.clarochile.cl","clarochile","clarochile","cosm.com", "80");
}
void loop()
{
modem.connecttoServer();
byte i = 0;
Serial.println("putting data html...");
//modem.sendd("GET / HTTP/1.0\r\n"); // search twitter for gm862
//modem.sendd("Connection: keep-alive\r\n");

modem.sendd("PUT /");
modem.sendd("{\"version\":\"1.0.0\",\n\"datastreams\":[\n{\"id\":\"0\", \"current_value\":\"0.65\"}]}");
modem.sendd("X-ApiKey: huDMRaP1zz0NQ4uGd8pDWpKaAs6SAKxEN3ZraVhtVkpWST0g");
modem.sendd("http://api.cosm.com/v2/feeds/128352");

// write on the socket
modem.sendd("\r\n");
Serial.println("receiving ...");
while (i++ < 50) {
  modem.receive(generalBuffer);
if (strlen(generalBuffer) > 0) {
Serial.print("buf:");
Serial.println(generalBuffer);
i--;
}
}
Serial.println("done");
modem.clossedConnection();
digitalWrite(ST_LED_pin,HIGH);
modem.switchOff();
while(1);
}


