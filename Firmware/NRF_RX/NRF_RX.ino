#define TEST_BASIC


#ifdef TEST_BASIC
/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
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
  Serial.begin(9600);
  Serial.println("RX alive 1");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  
}

char valueRX[2] = "";
char trashRX[32] = "";
void loop() {
	byte byteCounter = 0;
  while (radio.available()) {
    //int valueRX = 0;
	if(byteCounter ==0){
    radio.read(&valueRX, sizeof(valueRX));
    Serial.println(valueRX);
	}
	radio.read(&trashRX, sizeof(trashRX));
	byteCounter++;
  }
  if(valueRX[0] == 'a'){
digitalWrite(2, HIGH);
 }
  
  if(valueRX[0] == 'b'){
digitalWrite(2, LOW);
  }
  
  if(valueRX[0] == 'M'){
	  digitalWrite(2, HIGH);
  }
  
  if(valueRX[0] == 'N'){
	  digitalWrite(2, LOW);
  }
  
  

  
}
#endif

#ifdef fixed
/*


* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


const byte LED0_D2 = 2;
const byte NRF_POWER_RESET = 17;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  Serial.println("RX alive UNO");
  
  
	//Turn on the NRF
	
	pinMode(NRF_POWER_RESET, OUTPUT);
	digitalWrite(NRF_POWER_RESET, HIGH);
  
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(LED0_D2, OUTPUT);
}


 int valueRX = 53;
void loop() {
  if (radio.available()) {
    //char text[32] = "";
    radio.read(&valueRX, sizeof(valueRX));
	digitalWrite(LED0_D2, HIGH);
  }
  
   Serial.println(valueRX);

  digitalWrite(LED0_D2, LOW);
}
#endif
