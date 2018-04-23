#define notfixed
#ifdef notfixed
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
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  Serial.println("RX alive UNO");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    //char text[32] = "";
    int valueRX = 0;
    radio.read(&valueRX, sizeof(valueRX));
    Serial.println(valueRX);
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
