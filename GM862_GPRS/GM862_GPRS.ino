//Date: 2013.04.28
//Description: Connection to COSM.COM to report data through GPRS using GM862 module.
//Comments: GM.862 has been adapted for this code. Use attached source code.
//Luis Jaraquemaada S.

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
modem.settoInternet("bam.entelpcs.cl","entelpcs","entelpcs","api.xively.com", "80");
//modem.settoInternet("bam.clarochile.cl","clarochile","clarochile","api.xively.com", "80");

}
void loop()
{


// LJ: Sends data several times to server, incrementing the value each time
for (int increment=0; increment <= 40; increment=increment+5)
{
        modem.connecttoServer();
        byte i = 0;
        Serial.println("Uploading data ...");
        // Data to be reported to central server. This is a demo and not collected from real sensors.
        float temp_in=10.2345+increment;
        float temp_out=5.45+increment;
        //float main_door=1.11;
        
        // send the HTTP PUT
            ser.print("PUT /v2/feeds/");
            ser.print(831439071);
            ser.println(".csv HTTP/1.1");
            ser.println("Host: api.xively.com");
            ser.print("X-ApiKey: ");
            ser.println("nOdgzcmLarjSQCUBfL5chDIJ9IDowx8PfCvF32PUhTlHo9ez");
            ser.print("User-Agent: ");
            ser.println("GSM2_workingHTML2v3"); // O poner "Cosm-Arduino-Lib/1.0"
            ser.print("Content-Length: ");
        
            // calculate the length of the sensor reading in bytes:
            // 8 bytes for "temp_in," + number of digits of the data:
        
            ////int thisLength = 8 + getLength(thisData);
            int thisLength = 2 + 8 + countDigits(temp_in,2)+ 2 + 9 + countDigits(temp_out,2)/*+2 + 9 + countDigits(main_door,2)*/;
            
            ser.println(thisLength);
        
            // last pieces of the HTTP PUT request:
            ser.println("Content-Type: text/csv");
            ser.println("Connection: close");
            ser.println();
        
            // Content for PUT request:
            ser.print("temp_in,");
            ser.println(temp_in);
            ser.print("temp_out,");
            ser.println(temp_out);
        //    ser.print("main_door,");
        //    ser.println(main_door);
            Serial.println(thisLength);
                Serial.println(temp_in);
                Serial.println(temp_out);
        //        Serial.println(main_door);
        
        // write to socket
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
}


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

int countDigits(double number, int digits) {
  int n = 0;
  if (number < 0.0)
  {
    n++; // "-";
    number = -number;
  }
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i) {
    rounding /= 10.0;
  }
  number += rounding;
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  while (int_part > 0) {
    int_part /= 10;
    n++;
  }
  if (digits > 0) {
    n++; //".";
  }
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    n ++;
    remainder -= toPrint;
  }
  return n;
}
