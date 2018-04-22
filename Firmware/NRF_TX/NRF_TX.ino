/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH); //TURN ON THE MODULE

  
	Serial.begin(9600);
	 Serial.println("TX alive go to pro mini");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

int value = 0;
void loop() {
  //const char text[] = "test";
   value++;
  radio.write(&value, sizeof(value));
 // delay(5);

  if(value > 200){
    value =0;
  }
}
