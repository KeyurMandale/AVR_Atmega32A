#include <avr/io.h>
#define F_CPU 1000000UL
#include <stdlib.h>
#include "LCDLibrary.h"			  // The LCD library holds all the LCD functions


int main(void)
{
	InitializeLCD();			  // Initialize the LCD
	
	Send_a_String("Integer: ");
	uint8_t a = 50;
	char    b[2];
	itoa(a,b,10);				// itoa function converts the integer data into string type data
								// 'a' is the first argument which is the integer to be converted
								// 'b' is the second argument which is the character array in which converted value will be stored
								// '10' is  the base of the converted number i.e. 10 means decimal
	Send_a_String(b);			// Send the converted string to LCD
	
	SendCommand(0xC0);			// Go to second line of the LCD
	Send_a_String("Float: ");
	uint8_t number =175;
	float result;
	result = number/3.0;        // The divisor has to be a float number for the result to be float value
	char output[5];
	dtostrf(result,5,2,output); // dtostrf function converts the float data into string type data
								// 'result' is the first argument which is the float number to be converted
	
								// '5' is the second argument which denotes the total length of the converted string
								// i.e. if the string will be '5.25' then this argument will be 4
	
								// '2' is the second argument which denotes the precision of the converted float number
								// i.e. if this value is '2' the float number will have 2 digits post the decimal point
	
								// 'output' the fourth argument that holds the converted string
	Send_a_String(output);
	while (1)
	{
	}
}

