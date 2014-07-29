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
modem.settoInternet("bam.clarochile.cl","clarochile","clarochile","api.cosm.com", "80");
}
void loop()
{
modem.connecttoServer();
byte i = 0;
Serial.println("Uploading data ...");
//modem.sendd("GET / HTTP/1.0\r\n"); // search twitter for gm862
//modem.sendd("Connection: keep-alive\r\n");

int thisData=17;

// send the HTTP PUT request:
    ser.print("PUT /v2/feeds/");
    ser.print(128352);
    ser.println(".csv HTTP/1.1");
    ser.println("Host: api.cosm.com");
    ser.print("X-ApiKey: ");
    ser.println("huDMRaP1zz0NQ4uGd8pDWpKaAs6SAKxEN3ZraVhtVkpWST0g");
    ser.print("User-Agent: ");
    ser.println("GSM2_workingHTML2v2"); // O poner "Cosm-Arduino-Lib/1.0"
    ser.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    // 8 bytes for "sensor1," + number of digits of the data:

    int thisLength = 8 + getLength(thisData);
    
    //int thisLength = 9 + 5;
    //int thisLength = 2 + 2;
    ser.println(thisLength);

    // last pieces of the HTTP PUT request:
    ser.println("Content-Type: text/csv\n");
    ser.println("Connection: close");
    ser.println();

    // here's the actual content of the PUT request:
    ser.print("sensor1,");
    ser.println(thisData);
    Serial.println(thisLength);
        Serial.println(thisData);



//modem.sendd("PUT /");
//modem.sendd("{\"version\":\"1.0.0\",\n\"datastreams\":[\n{\"id\":\"0\", \"current_value\":\"0.65\"}]}");
//modem.sendd("X-ApiKey: huDMRaP1zz0NQ4uGd8pDWpKaAs6SAKxEN3ZraVhtVkpWST0g");
//modem.sendd("http://api.cosm.com/v2/feeds/128352");

// write on the socket
modem.sendd("\r\n");
Serial.println("Waiting for server feedback ...");
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

int getLength(int someValue)
{
  // there's at least one byte:
  int digits = 1;
  
  // continually divide the value by ten,
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0)
  {
    dividend = dividend /10;
    digits++;
  }
  
  // return the number of digits:
  return digits;
}
