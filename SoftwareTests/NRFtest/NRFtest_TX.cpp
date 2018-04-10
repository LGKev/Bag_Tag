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

#include <SPI.h>   
#include "RF24.h"  

RF24 myRadioTX(7, 8); // CE, CS Pins

byte addresses[][6] = {"1Node"}; // Create address for 1 pipe.
int dataTransmitted;  // Data that will be Transmitted from the transmitter

void setup(){
	myRadioTX.begin();
	myRadioTX.setChannel(108); //2.508 Ghz
   // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  myRadio.setPALevel(RF24_PA_MIN); //testing
 // myRadio.setPALevel(RF24_PA_MAX);
  myRadio.openWritingPipe( addresses[0]); 
  delay(1000); 
 
}

int i = 0;

void loop(){

	
	if(i >= 100){
		i = 0; ///reset data
	}
	
	myRadio.write( &dataTransmitted, sizeof(i) ); //start at 0 go to 100
	delay(1000);
	i++;
}


/*

      radio.read( &myData, sizeof(myData) );             // Get the data
myData holds the data its an address so no need to pass anyhting


we are using a sytcut here so we can send multiple bytes 


*/
