/*
This is a code for connecting Arduino through GPRS to Cosm.com using Telit GM862
*/


#include <SoftwareSerial.h>
#include <string.h>
#include "GM862.h"
#include "HttpClient.h"
#include "Cosm.h"

#define BUF_LENGTH 100
#define txGprs_pin 3
#define rxGprs_pin 2
SoftwareSerial ser(rxGprs_pin,txGprs_pin);
GM862 modem(&ser);

char generalBuffer[100];

//Configuration of Arduino Setup
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
modem.settoInternet("APN", "username", "password", "domain", "port");
}

//Arduino actions in Loop
void loop()
  {
  modem.connecttoServer();
  char iteracion =0;
  while(iteracion < 4)
    {
    char tmp;
    Serial.println("");
    float temperature = float(analogRead(0));
    temperature = processinSensor(temperature,-21.244,1.7908,-0.0278,0.0003,0,(100.0/1024));
    strcpy(generalBuffer,"Temperatura Actual = ");

    char *pTemporal = generalBuffer + strlen(generalBuffer);
    ftoa(pTemporal,temperature,1);
    modem.transmittoServer(generalBuffer);
    delay(5000);
    iteracion++;
    Serial.println(generalBuffer);
    }
  modem.clossedConnection();
  digitalWrite(ST_LED_pin,HIGH);
  delay(2000);
  modem.switchOff();
  while(1);
  }
  
float processinSensor(float sensor, float a, float ax,float axx, float axxx,float axxxx,
float factor) {
  sensor=(sensor*factor);//valor normalizado, y luego transformado en %
  sensor= (axxxx*pow(sensor,4)+axxx*pow(sensor,3)+axx*pow(sensor,2)+ax*sensor+a);
  return sensor;}
  void *ftoa(char *a, double f, int precision){
  float p[] = {0.0,10.0,100.0,1000.0,10000.0,100000.0,1000000.0,10000000.0,100000000.0};
  char *ret = a;
  char mantiza[10];
  long heiltal = long(f);
  ltoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long decimal = abs(long((f - float(heiltal)) * p[precision]));
  ltoa(decimal, mantiza, 10);
  for(;strlen(mantiza) < precision; precision--)
  *a++ = '0';
  strcpy(a , mantiza);
  }

