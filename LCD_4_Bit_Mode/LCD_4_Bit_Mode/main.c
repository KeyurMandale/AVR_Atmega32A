#include <avr/io.h>
#define F_CPU 1000000UL
#include <stdlib.h>
#include "LCDLibrary.h"			  // The LCD library holds all the LCD functions


int main(void)
{
	InitializeLCD();			  
	
	Send_a_String("Integer: ",0,0);		// Send_a_String function is modified to accept x and y location co-ordinates for string placement on LCD
	uint8_t a = 50;
	char    b[2];
	itoa(a,b,10);				
	Send_a_String(b,0,9);				// Send_a_String function is modified to accept x and y location co-ordinates for string placement on LCD
	
	Send_a_String("Float: ",1,0);		// Send_a_String function is modified to accept x and y location co-ordinates for string placement on LCD
	uint8_t number =175;
	float result;
	result = number/3.0;        
	char output[5];
	dtostrf(result,5,2,output); 
	Send_a_String(output,1,7);			// Send_a_String function is modified to accept x and y location co-ordinates for string placement on LCD
	
	while (1)
	{
	}
}

