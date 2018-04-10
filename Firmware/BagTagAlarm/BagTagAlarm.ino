/*
MIT License

Copyright (c) 2018 Kevin Kuwata

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
	
	
*/

#include "BagTagAlarm.h"
//#include <Spi.h>
//avr sleep 
// avr eeprom

#define LED0_D2			2
#define NRF_POWER_RESET		A3	
#define ACCELEROMETER_X		A0	
#define	ACCELEROMETER_Y		A1	
#define ACCELEROMETER_Z		A2
#define	NRF_CSN				10
#define NRF_CE				9
#define NRF_IRQ				8
#define MAG_SWITCH			6
#define ALARM				5
#define ACCELEROMETER_SELF_TEST		4
#define ACCELEROMETER_PWR_DOWN		3
#define SCK					13 
#define MOSI				11
#define MISO				12
#define BIT0 0b00000000
#define BIT1 0b00000001
#define BIT2 0b00000010
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000


void setup(){
	pinMode(LED0_D2, OUTPUT);
	pinMode(ALARM, OUTPUT); //equivently: DDRD|= 0b00100000; |= bit5;
	pinMode(MAG_SWITCH, INPUT);
	
	pinMode(ACCELEROMETER_PWR_DOWN, OUTPUT);
	pinMode(ACCELEROMETER_PWR_DOWN, OUTPUT);
	pinMode(ACCELEROMETER_X, INPUT);
	pinMode(ACCELEROMETER_Y, INPUT);
	pinMode(ACCELEROMETER_Z, INPUT);

	
	digitalWrite(LED0_D2, LOW);
	digitalWrite(ALARM, LOW);
	
}


void loop(){
	while(1){
	PORTD |=BIT5; // turn on
	delayMicroseconds(125);
	PORTD ^= (BIT5); //turn off
	delayMicroseconds(125);//	heartBeat();
	}
}
/*
	@name: soundalarm
	@brief: will sound the piezeo buzzer with 50% duty cycle square wave with 4Khz. This produces the loudest alarm. 
			Manual port manpulation to create the desired frequency
	@Output:  PortD Pin5 is the buzzer
	uses a delay micro to create the alarm. 
	@input: duration = a byte to describe how long to sound alarm in seconds. 
*/
void soundAlarm(byte duration){
	PORTD |=BIT5; // turn on
	delayMicroseconds(125);
	PORTD ^= ~(BIT5); //turn off
	delayMicroseconds(125);
}	

void heartBeat(){
	PORTD |= BIT2;
	delay(250);
	PORTD ^= ~BIT2;
	delay(250);
}