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

byte SOUND_THE_ALARM = 255; 
unsigned long currentTime = 0;
unsigned long elapsedTime = 0;



#include "BagTagAlarm.h"
//#include <Spi.h>
//avr sleep 
// avr eeprom

#define LED0_D2			2	//
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

#define ACCELEROMETER_AVG_COUNT 		20 //arbitrary averaging
#define ACCELEROMETER_SENSITIVY_X		250 // again arbitrarily chosen
#define ACCELEROMETER_SENSITIVY_Y		5
#define	ACCELEROMETER_SENSITIVY_Z		5



/* ====================================================================== *//* ====================================================================== */
/* ====================================================================== *//* ====================================================================== */
///All of my defines for Testing	
//#define TEST_ACCL			//turns on an led if change in motion
//#define TEST_ALARM		//tests the alarm

//#define TEST_SOLIDLED 		//I THINK WE HAVE A BUNCH OF RESETS.

//#define TEST_BLINK_LED_ONLY	
//#define TEST_LOOPTIME	// put the final code in this define to check to execution time of the loop. 

//#define TEST_ACCL_THRESH //threshold tests to get analog values with scope.

#define TEST_TIME_AVG		//averaging over a time period with millis()
#define UART_DEBUG 	// only useful on an uno. 

/* ====================================================================== */
/* ====================================================================== *//* ====================================================================== */


void setup(){
	pinMode(LED0_D2, OUTPUT);
	pinMode(ALARM, OUTPUT); //equivently: DDRD|= 0b00100000; |= bit5;
	pinMode(MAG_SWITCH, INPUT);
	
	pinMode(ACCELEROMETER_PWR_DOWN, OUTPUT);
	pinMode(ACCELEROMETER_PWR_DOWN, OUTPUT);
	pinMode(ACCELEROMETER_X, INPUT);
	pinMode(ACCELEROMETER_Y, INPUT);
	pinMode(ACCELEROMETER_Z, INPUT);
	
	digitalWrite(ACCELEROMETER_PWR_DOWN, LOW);	//NORMAL MODE, NOT SLEEP
	digitalWrite(ACCELEROMETER_SELF_TEST, LOW); //NORMAL MODE

	
#ifdef UART_DEBUG
Serial.begin(9600);
Serial.println("test started");
#endif
	
	//blink the led so we know code is running
	for(int i=0; i < 10; i++){
		digitalWrite(LED0_D2, HIGH);
		delay(100);
		digitalWrite(LED0_D2, LOW);
		delay(100);
	}
	
	
	digitalWrite(LED0_D2, LOW);
	digitalWrite(ALARM, LOW);
	
	#ifdef TEST_SOLIDLED
	digitalWrite(LED0_D2, HIGH);
	while(1);
	#endif
	
	
}


#ifdef TEST_TIME_AVG

void loop(){
	int z_avg = 0;
	z_avg = averageOverZ(50);
	
	int x_avg = 0;
	x_avg = averageOverX(50);
	
	int y_avg = 0;
	y_avg = averageOverY(50);


	Serial.print("avgX: \t ");
	Serial.print(x_avg);
	
	
	Serial.print("\t return X: \t");
	Serial.print(	alarmFromDeltaX(100,300,3));//1 second movement, 2 second idle, and 3 ADC points of change required
	Serial.print("\t return Y \t");
	Serial.print(	alarmFromDeltaY(100,300,3));//1 second movement, 2 second idle, and 3 ADC points of change required
	Serial.print("\t return Z: \t");
	Serial.println(	alarmFromDeltaZ(100,300,3));//1 second movement, 2 second idle, and 3 ADC points of change required
	
	#ifdef UART_DEBUG3
	Serial.print("avgX: \t ");
	Serial.print(x_avg);

	Serial.print("  \t avgY: \t ");
	Serial.print(y_avg);
	
	Serial.print(" \t avgZ: \t ");
	Serial.print(z_avg);
	
	Serial.print("\t pure X: \t");
	Serial.print(analogRead(ACCELEROMETER_X));
	
	Serial.print("\t pure Y: \t");
	Serial.print(analogRead(ACCELEROMETER_Y));

	Serial.print("\t pure Z: \t");
	Serial.println(analogRead(ACCELEROMETER_Z));
	
	
	
	
	#endif
	//delay(1000);
	
}




/* ================================================================================================*/
// Alarm Trigger Functions
/* ================================================================================================*/

/* 

			X
			
			
	get the change in X, over a period of time and return a 1 if alarm should sound
	return 0 if alarm should not sound.
	
	inputs: allowableMovementTime_mS [milliseconds] how long it can move
	 ie 2 seconds, if moment persists for more than 2 seconds alarm sounds.
	xSensitivity 1 being only allow 1 ADC point difference_type
	3 being allow 3 ADC point differences. 
	
	idleTime_mS  = the time required for the sensor to be idle for alarm not to sound. [seconds]
	
*/
int  alarmFromDeltaX(unsigned long allowableMovementTime_mS, unsigned long idleTime_mS, byte xSensitivity){
	
	unsigned long startTime = 0;
	unsigned long elapsedTime = 0;
	
	//unsigned long allowableMovementTime_mS = allowableMovementTime_S * 1000;
	//unsigned long idleTime_mS = idleTime_S * 1000;

	
	unsigned long deltaX = 0;
	unsigned long x_avg_first = averageOverX(50); //50mS
	unsigned long x_avg_second = averageOverX(50);
	
	deltaX = abs(x_avg_first - x_avg_second);
	while(deltaX != 0){
		
	while(deltaX >= xSensitivity){
		if(startTime == 0 ){
			startTime = millis();	
		}
		
		x_avg_first = averageOverX(50); //50mS
		x_avg_second = averageOverX(50);
	  
	  	deltaX = abs(x_avg_first - x_avg_second);
		
		elapsedTime = millis() - startTime;
		
		if(elapsedTime > allowableMovementTime_mS){
			return 1;
		}
	}
	
	
	x_avg_first = averageOverX(50); //50mS
	x_avg_second = averageOverX(50);
	deltaX = abs(x_avg_first - x_avg_second);
	startTime = 0;
	elapsedTime = 0;
	while(deltaX < xSensitivity){
		if(startTime == 0 ){
			startTime = millis();
		}
		
		x_avg_first = averageOverX(50);
		x_avg_second = averageOverX(50);
		
		deltaX = abs(x_avg_first - x_avg_second);
		
		elapsedTime = millis() - startTime;
		if(elapsedTime > idleTime_mS){
			return 0;
		}
	}
}
	return 53;
}

/*
				Y
				
				
	get the change in X, over a period of time and return a 1 if alarm should sound
	return 0 if alarm should not sound.
	
	inputs: allowableMovementTime_mS [milliseconds] how long it can move
	 ie 2 seconds, if moment persists for more than 2 seconds alarm sounds.
	xSensitivity 1 being only allow 1 ADC point difference_type
	3 being allow 3 ADC point differences. 
	
	idleTime_mS  = the time required for the sensor to be idle for alarm not to sound. [seconds]
	
*/
int  alarmFromDeltaY(unsigned long allowableMovementTime_mS, unsigned long idleTime_mS, byte sensitivity){
	
	unsigned long startTime = 0;
	unsigned long elapsedTime = 0;
	
	//unsigned long allowableMovementTime_mS = allowableMovementTime_S * 1000;
	//unsigned long idleTime_mS = idleTime_S * 1000;

	
	unsigned long deltaY = 0;
	unsigned long y_avg_first = averageOverY(50); //50mS
	unsigned long y_avg_second = averageOverY(50);
	
	deltaY = abs(y_avg_first - y_avg_second);
	while(deltaY != 0){
		
	while(deltaY >= sensitivity){
		if(startTime == 0 ){
			startTime = millis();	
		}
		
		y_avg_first = averageOverX(50); //50mS
		y_avg_second = averageOverX(50);
	  
	  	deltaY = abs(y_avg_first - y_avg_second);
		
		elapsedTime = millis() - startTime;
		
		if(elapsedTime > allowableMovementTime_mS){
			return 1;
		}
	}
	
	
	y_avg_first = averageOverX(50); //50mS
	y_avg_second = averageOverX(50);
	deltaY = abs(y_avg_first - y_avg_second);
	startTime = 0;
	elapsedTime = 0;
	while(deltaY < sensitivity){
		if(startTime == 0 ){
			startTime = millis();
		}
		
		y_avg_first = averageOverX(50);
		y_avg_second = averageOverX(50);
		
		deltaY = abs(y_avg_first - y_avg_second);
		
		elapsedTime = millis() - startTime;
		if(elapsedTime > idleTime_mS){
			return 0;
		}
	}
}
	return 54;
}

/* 				Z


	get the change in X, over a period of time and return a 1 if alarm should sound
	return 0 if alarm should not sound.
	
	inputs: allowableMovementTime_mS [milliseconds] how long it can move
	 ie 2 seconds, if moment persists for more than 2 seconds alarm sounds.
	xSensitivity 1 being only allow 1 ADC point difference_type
	3 being allow 3 ADC point differences. 
	
	idleTime_mS  = the time required for the sensor to be idle for alarm not to sound. [seconds]
	
*/
int  alarmFromDeltaZ(unsigned long allowableMovementTime_mS, unsigned long idleTime_mS, byte sensitivity){
	
	unsigned long startTime = 0;
	unsigned long elapsedTime = 0;
	
	//unsigned long allowableMovementTime_mS = allowableMovementTime_S * 1000;
	//unsigned long idleTime_mS = idleTime_S * 1000;

	
	unsigned long deltaZ = 0;
	unsigned long z_avg_first = averageOverX(50); //50mS
	unsigned long z_avg_second = averageOverX(50);
	
	deltaZ = abs(z_avg_first - z_avg_second);
	while(deltaZ != 0){
		
	while(deltaZ >= sensitivity){
		if(startTime == 0 ){
			startTime = millis();	
		}
		
		z_avg_first = averageOverX(50); //50mS
		z_avg_second = averageOverX(50);
	  
	  	deltaZ = abs(z_avg_first - z_avg_second);
		
		elapsedTime = millis() - startTime;
		
		if(elapsedTime > allowableMovementTime_mS){
			return 1;
		}
	}
	
	
	z_avg_first = averageOverX(50); //50mS
	z_avg_second = averageOverX(50);
	deltaZ = abs(z_avg_first - z_avg_second);
	startTime = 0;
	elapsedTime = 0;
	while(deltaZ < sensitivity){
		if(startTime == 0 ){
			startTime = millis();
		}
		
		z_avg_first = averageOverX(50);
		z_avg_second = averageOverX(50);
		
		deltaZ = abs(z_avg_first - z_avg_second);
		
		elapsedTime = millis() - startTime;
		if(elapsedTime > idleTime_mS){
			return 0;
		}
	}
}
	return 55;
}

/* ================================================================================================*/
// averaging functions
/* ================================================================================================*/

/*
	give the time which to average over [mS]
	, and return the average
	
	for the z axis. a steady voltage of 2.32 is observed in 1 orientation
	this converts to 719 or around that.
*/
int averageOverZ(int timeAverageOver){
	
	unsigned long startTime = millis();
	unsigned long elapsedTime = 0;
	int z_count = 0;
	unsigned long z = 0;

	while(elapsedTime < timeAverageOver){ //1/4 second avg
		z += analogRead(ACCELEROMETER_Z);
		z_count++;
		elapsedTime = millis() - startTime;
	}
	
	
	#ifdef UART_DEBUG2
	Serial.print("\t z sum:  \t");
	Serial.print(z);
	Serial.print("\t count \t");
	Serial.println(z_count);
	delay(500);
	#endif
	
	z = z / z_count;
		
		
	#ifdef UART_DEBUG2
	
	Serial.print("z  avg: \t");
	Serial.println(z);
	delay(500);
	
	#endif
	
	
	return z;
}

int averageOverY(int timeAverageOver){
	
	unsigned long startTime = millis();
	unsigned long elapsedTime = 0;
	int y_count = 0;
	unsigned long y = 0;

	while(elapsedTime < timeAverageOver){ //1/4 second avg
		y += analogRead(ACCELEROMETER_Y);
		y_count++;
		elapsedTime = millis() - startTime;
	}
	
	
	#ifdef UART_DEBUG2
	Serial.print("\t y sum:  \t");
	Serial.print(y);
	Serial.print("\t count \t");
	Serial.println(y_count);
	delay(500);
	#endif
	
	y = y / y_count;
		
		
	#ifdef UART_DEBUG2
	
	Serial.print("y  avg: \t");
	Serial.println(y);
	delay(500);
	
	#endif
	
	
		return y;
}


int averageOverX(int timeAverageOver){
	
	unsigned long startTime = millis();
	unsigned long elapsedTime = 0;
	int z_count = 0;
	unsigned long z = 0;

	while(elapsedTime < timeAverageOver){ //1/4 second avg
		z += analogRead(ACCELEROMETER_X);
		z_count++;
		elapsedTime = millis() - startTime;
	}
	
	
	#ifdef UART_DEBUG2
	Serial.print("\t X sum:  \t");
	Serial.print(z);
	Serial.print("\t count \t");
	Serial.println(z_count);
	delay(500);
	#endif
	
	z = z / z_count;
		
		
	#ifdef UART_DEBUG2
	
	Serial.print("z  avg: \t");
	Serial.println(z);
	delay(500);
	
	#endif
	
	
		return z;
}

#endif


#ifdef TEST_ACCL_THRESH
int x, y, z;
void loop(){
	//heartBeat(); // the XOR is an issue and I was having brown out. current limit is 5 amp now and the device is not resetting.
	//delay(1);
	//PORTD &= ~BIT2;
	//delay(1);
	
	PORTD |= 0x04; //bit 2
	 x = analogRead(ACCELEROMETER_X);
	 y = analogRead(ACCELEROMETER_Y);
	 z = analogRead(ACCELEROMETER_Z);
	PORTD = 0x00;
}
#endif

#ifdef TEST_LOOPTIME
void loop(){
	PORTD = 0b00000100; //bit 2?
	PORTD = 0x00;
} 
#endif


#ifdef TEST_BLINK_LED_ONLY
void loop(){
	digitalWrite(LED0_D2, HIGH);
	delay(1000);
	digitalWrite(LED0_D2, LOW);
	delay(1000);
}
#endif


#ifdef TEST_SOLIDLED
void loop(){
	while(1);
}
#endif


/*
	@name: void loop()
	@brief: tests the accelerometer by averaging the x, y, z values.	
		if significant change light the on board led on pin 2
		update the average every 20 cycles, arbitrarily chosen. 
	
*/
#ifdef TEST_ACCL

unsigned int x_measure = 0;
unsigned int y_measure = 0;
unsigned int z_measure = 0;
unsigned int old_X_average = 0;
unsigned int old_Y_average = 0;
unsigned int old_Z_average = 0;

byte SOUND_THE_ALARM = 255; 
unsigned long currentTime = 0;
unsigned long elapsedTime = 0;


void loop(){
	//reset
	
	PORTD|= BIT2;
	
	x_measure = 0;
	y_measure = 0;
	z_measure = 0;
	
	//collect for average
	for(int i=0; i <ACCELEROMETER_AVG_COUNT; i++){
	x_measure += analogRead(ACCELEROMETER_X);
	y_measure += analogRead(ACCELEROMETER_Y);
	z_measure += analogRead(ACCELEROMETER_Z);
	}
	
	//average
	x_measure /= ACCELEROMETER_AVG_COUNT;
	y_measure /= ACCELEROMETER_AVG_COUNT;
	z_measure /= ACCELEROMETER_AVG_COUNT;

	//calculate change
	byte x_change = abs(x_measure - old_X_average);
	byte y_change = abs(y_measure - old_Y_average);
	byte z_change = abs(z_measure - old_Z_average);
	
	//if bigger than threshold 	
	//if( (x_change> ACCELEROMETER_SENSITIVY_X ) || (y_change > ACCELEROMETER_SENSITIVY_Y) || (z_change > ACCELEROMETER_SENSITIVY_Z)){
	if( x_change > ACCELEROMETER_SENSITIVY_X ){
		digitalWrite(LED0_D2, HIGH);
		delay(100);
		digitalWrite(LED0_D2, LOW);
		delay(100);
		
		SOUND_THE_ALARM = 1;
		x_change = 0;
	}
	
	
	if(SOUND_THE_ALARM == 1){
		soundAlarm(1000); //sound alarm for 2 seconds
		SOUND_THE_ALARM = 255;
	}
	
	
	
	//update values;
	old_X_average = x_measure;
	old_Y_average = y_measure;
	old_Z_average = z_measure;
	
	PORTD &= ~BIT2; //FOR MEASURING THE LOOP
}
#endif

/*
	@name: void loop()
	@brief: will test the alarm by just port mainpulation. assuming set up was done correctly. 
	@input: none
	@output: 4khz 50% square wave 3.3v
	
	@bug: 11khz chopping frequency plus the 4khz frequency from the square wave. 50% duty cycle spot on.
	
	something about how the loop works. 
*/
#ifdef TEST_ALARM
void loop(){
	while(1){
	PORTD |=BIT5; // turn on
	delayMicroseconds(125);
	PORTD ^= (BIT5); //turn off
	delayMicroseconds(125);//	heartBeat();
	}
}

#endif

/*
	@name: soundalarm
	@brief: will sound the piezeo buzzer with 50% duty cycle square wave with 4Khz. This produces the loudest alarm. 
			Manual port manpulation to create the desired frequency
	@Output:  PortD Pin5 is the buzzer
	uses a delay micro to create the alarm. 
	@input: duration = a byte to describe how long to sound alarm in seconds. 
*/
void soundAlarm(byte duration){
	bool durationUp = false;
	currentTime = millis();
	
	while(durationUp == false){
	PORTD |=BIT5; // turn on alarm
	delayMicroseconds(125);
	PORTD ^= (BIT5); //turn off
	delayMicroseconds(125);
	
	elapsedTime = millis() - currentTime; //update how much time has gone by. 
		if(elapsedTime >= (duration)){
			durationUp = true;
				delay(1000); //let it settle
			break;
		}
	}
}	

void heartBeat(){
	PORTD |= BIT2;
	delay(250);
	PORTD ^= ~BIT2;
	delay(250);
}