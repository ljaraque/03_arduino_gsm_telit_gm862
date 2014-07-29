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
#include <Cosm.h>
#define BUF_LENGTH 100
#define txGprs_pin 3
#define rxGprs_pin 2
SoftwareSerial ser(rxGprs_pin,txGprs_pin);
GM862 modem(&ser);

CosmClient ;
char generalBuffer[500];


//With COSM
char sensorId[] = "0";

CosmDatastream datastreams[] = {
  CosmDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};

// Wrap the datastream into a feed
CosmFeed feed(128352, datastreams, 1 /* number of datastreams */);



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

modem.connecttoServer();
byte i = 0;
Serial.println("Uploading data ...");
sendData(18);

}
void loop()
{
/*modem.connecttoServer();
byte i = 0;
Serial.println("Uploading data ...");
*/

//modem.sendd("GET / HTTP/1.0\r\n"); // search twitter for gm862
//modem.sendd("Connection: keep-alive\r\n");




/*
//without Cosm
// send the HTTP PUT request:
    ser.print("PUT /v2/feeds/");
    ser.print("128352");
    ser.println(".csv HTTP/1.1");
    ser.println("Host: api.pachube.com");
    ser.print("X-PachubeApiKey: ");
    ser.println("huDMRaP1zz0NQ4uGd8pDWpKaAs6SAKxEN3ZraVhtVkpWST0g");
    ser.print("User-Agent: ");
    ser.println("Cosm-Arduino-Lib/1.0");
    ser.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    // 8 bytes for "sensor1," + number of digits of the data:

    //int thisLength = 9 + 5;
    ser.println("2");

    // last pieces of the HTTP PUT request:
    ser.println("Content-Type: text/csv");
    ser.println("Connection: close");
    ser.println();

    // here's the actual content of the PUT request:
    ser.print("temp,");
    ser.println("17");
 //   Serial.println(thisLength);
        Serial.println(17);

*/




//modem.sendd("PUT /");
//modem.sendd("{\"version\":\"1.0.0\",\n\"datastreams\":[\n{\"id\":\"0\", \"current_value\":\"0.65\"}]}");
//modem.sendd("X-ApiKey: huDMRaP1zz0NQ4uGd8pDWpKaAs6SAKxEN3ZraVhtVkpWST0g");
//modem.sendd("http://api.cosm.com/v2/feeds/128352");

/*
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
*/
}

// send the supplied value to Cosm, printing some debug information as we go
void sendData(int sensorValue) {
  datastreams[0].setFloat(sensorValue);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  Serial.println("Uploading to Cosm");
  int ret = cosmclient.put("128352", "huDMRaP1zz0NQ4uGd8pDWpKaAs6SAKxEN3ZraVhtVkpWST0g");
  Serial.print("PUT return code: ");
  Serial.println(ret);

  Serial.println();
}

