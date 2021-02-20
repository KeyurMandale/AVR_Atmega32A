/******************************Header_Files**********************************/
#include <avr/io.h>
#define F_CPU 1000000UL // Define operating frequency of  uC for _delay_ms(),_delay_us() function
#include <util/delay.h>
#include <stdio.h>

/*********************************MACROS**************************************/

#define DataPort		  PORTB
#define DataPortDirect	  DDRB
#define ControlPort		  PORTD
#define ControlPortDirect DDRD
#define EnablePin 0
#define ReadWrite 1
#define RegisterS 2

/***************************Function_Prototypes******************************/

void CheckBusy(void);
void EnableRoutine(void);
void SendCommand(unsigned char command);
void SendData(unsigned char data_char);
void Send_a_String(char *stringofchars);

/*******************************Main****************************************/

int main(void)
{
    ControlPortDirect |= ( 1 << EnablePin | 1 << ReadWrite | 1 << RegisterS); // Configure the LCD control pins as O/P
	_delay_ms(15);
	
	SendCommand(0x01);			// Clear LCD Screen
	_delay_ms(2);
	SendCommand(0x38);			// 8 Bit mode
	_delay_us(50);
	SendCommand(0b00001110);	// Set display on, cursor on
	_delay_us(50);              // Delays are added to give LCD sufficient time to process previous instructions
	
	Send_a_String("Merry XMas and a"); // Send a String
	SendCommand(0xC0);				   // Move Cursor to second line of 16x2 LCD
	Send_a_String("Happy New Year!");  // Send a String
	
    while (1) 
    {
    }
}

/*******************************************Functions*************************************************/

// This function checks if the LCD is busy or not. D7 is High if LCD is busy
void CheckBusy(void)
{
	DataPortDirect = 0x00;				// Configure DataPort as I/P
	ControlPort |= 1 << ReadWrite;		// Read Mode
	ControlPort &= ~(1 << RegisterS);   // Command Mode
	while( DataPort >= 0x80)			// To check if pin D7 is High 
	{
		EnableRoutine();				// Enable function
	}
	DataPortDirect = 0xFF;				// Configure DataPort as O/P
}

// This function gives the Enable pulse to the LCD
void EnableRoutine(void)
{
	ControlPort |= 1 << EnablePin;    // Make EnablePin High
	asm volatile ("nop");		      // "nop" will perform no operation but will consume clock cycles
	asm volatile ("nop");		      // "nop" will perform no operation but will consume clock cycles
	ControlPort &= ~(1 << EnablePin); // Make EnablePin Low
}

// This function sends a command to the LCD
void SendCommand(unsigned char command)
{
	CheckBusy();										// Check if LCD is busy
	DataPort = command;									// Move the command value to the DataPort. i.e if 0xC0 is sent as command PORTB will show 0xC0;
	ControlPort &= ~(1 << ReadWrite | 1 << RegisterS);  // Write Mode Command Mode
	EnableRoutine();									// Enable the LCD to accept the sent command 
	DataPort = 0x00;									// Clear DataPort					
}

// This function sends a data character to the LCD
void SendData(unsigned char data_char)
{
	CheckBusy();							// Check if LCD is busy
	DataPort = data_char;					// Move the data char to DataPort i.e. if character 'A' is to be moved the ASCII value for 'A' which is 0x41 is moved to PORTB
	ControlPort &= ~(1 << ReadWrite);		// Read Mode		
	ControlPort |= 1 << RegisterS;			// Data Mode
	EnableRoutine();						// Enable the LCD to accept the sent data
	DataPort = 0x00;						// Clear DataPort
}

// This function sends a string of characters
void Send_a_String(char *stringofchars)
{
	// *stringofchars is a pointer to the memory location containing the string
	
	while(*stringofchars > 0)
	{
		SendData(*stringofchars++);
	}
}