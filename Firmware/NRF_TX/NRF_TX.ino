#define notfixed

#ifdef notfixed
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
#endif



#ifdef fixed
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
  pinMode(2, OUTPUT); //SO WE KNOW  can't use 13 cuz Radio is using it. SCK....
  digitalWrite(2, HIGH);
  
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);

  
  Serial.begin(9600);
  Serial.println("TX alive ");
  Serial.println("list of commands as char inputs");
  Serial.println(" \t o - off \n \t 'l' - led on  \n \t a - sound alarm once");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

int value = 0;
int characterRecieved = 0;
void loop() {
	
	/*
		Collect input from User we know the first byte is the command. 
		
	*/
	byte byteCounter = 0;
	while(Serial.available() > 0){
		if(byteCounter == 0){
			characterRecieved = Serial.read();
		}
		Serial.read(); //toss the rest we don't care
		byteCounter++;
	}
	
	transmitOverRadio(characterRecieved);
	
	value = 63;
	radio.write(&value, sizeof(value));	

}

void transmitOverRadio(byte character){
	int value = 53; /// arbitrary number that doesn't mean anything.
	
	
	switch(character){
		case 'a': //turn on the alarm
		Serial.println("ring ring");
		value = 1;
		character = 53;
		break;
	
		case 'l': //turn on the led
		Serial.println("let there be light");
		digitalWrite(2, HIGH);
		value = 2;
		character = 53;
		break;
		
		case 'o': //letter o, turn off the led
		value = 0;
		Serial.println("no light for you!");
		digitalWrite(2, LOW);
		character = 53;
		break;
		
		case 'h':
		Serial.println(" \n \n \n list of commands as char inputs");
		Serial.println(" \t o - off \n \t 'l' - led on  \n \t a - sound alarm once \n \n");
		character = 53;
		break;
		
		default:		
		return;
	}
	radio.write(&value, sizeof(value));	
	characterRecieved = 0; //reset the global so we don't constantly trigger this.
}

#endif