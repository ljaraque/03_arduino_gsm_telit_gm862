//send an SMS using a TC35 gsm module
//some more AT commands can be given as well (ATD,AT,SMSC)
//communication between tc35-arduino using software serial port
//RXD0 TC35->arduino pin 2
//TXD0 TC35->arduino pin 3
//dont forget to connect the ground
//PIN was disabled
 
#include <SoftwareSerial.h>
#include <string.h>
#include "GM862.h"

SoftwareSerial gsmSerial(2,3);
GM862 modem(&gsmSerial);

char gsm_char,gsm_char_send;
 
String cellContent=""; //string we're sending
String ctlZ="\x1A"; //the code for Ctrl-Z
String SMScontent="Test"; //the actual SMS message
 
void setup(){
 Serial.begin(9600);
 gsmSerial.begin(9600);
 modem.switchOn();
 modem.init();

 
 Serial.println("\r\nCommunication started\r\n");
 

}
void loop(){
 
  //start communication with GSM. mover todo hasta while para que loop pueda ejecutarse
 gsmSerial.print("AT+CMGF=1\r");
 delay(3000);
 gsmSerial.print("AT+CMGS=+56999915809\r");
 delay(3000);
 gsmSerial.print("hello this is a test message");
 delay(3000);
 gsmSerial.print(ctlZ);
 delay(5000);
 modem.switchOff();
 while(1);
  
 if(gsmSerial.available()>0){
 gsm_char_send=gsmSerial.read();
 Serial.print(gsm_char_send);
 }
 
  if(Serial.available()>0){
 gsm_char=Serial.read();
 gsmSerial.print(gsm_char);
 delay(400);
 }
 /*
 //when the character c is received from the terminal of the computer
 //call a number (feature just for testing TC35 board)
 if(Serial.available() >0){
 gsm_char=Serial.read();
 
 if(gsm_char=='c')
 {
 //call number
 gsmSerial.write("ATD+31619027454;\r");
 }
 else
 if(gsm_char=='s')
 {
 //add message to the CellContent string
 cellContent+=SMScontent;
 //add the Crtl-Z character
 cellContent+=ctlZ;
 
 //send SMS in text mode
 gsmSerial.print("AT+CMGF=1\r");
 delay(400);
 
 sendSMS();
 
 }
 else if(gsm_char=='t')
 {
 gsmSerial.print("AT\r");
 delay(400);
 }
 else if(gsm_char=='d'){
 //set SMSC number (if needed)
 gsmSerial.print("AT+CSCA=\"+31626000230\"\r");
 delay(100);
 }
 
 }*/
}
 
void sendSMS(){
 
 gsmSerial.println("AT+CMGS=\"0619027454\""); //AT command to send SMS
 delay(500); //the length of this delay is very important. 400 is too low
 gsmSerial.print(cellContent); //Print the message
 delay(15);
}
