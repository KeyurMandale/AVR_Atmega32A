/********************************LCD_Library_4_Bit_Mode********************************/
// In  4 bit mode the data/command is sent nibble wise.
// The upper nibble i.e. the 4 most significant bits are sent first
// The lower nibble i.e. the 4 least significant bits follow

// The LCD data pins D4 to D7 are connected to PORTB pins PINB4 to PINB7

#ifndef LCDLibrary
#define LCDLibrary

#include <avr/io.h>
#include <util/delay.h>


#define DataPort PORTB
#define DataPortDirect DDRB
#define ControlPort PORTD
#define ControlPortDirect DDRD
#define EnablePin 0
#define ReadWrite 1
#define RegisterS 2

void CheckBusy(void);
void EnableRoutine(void);
void SendCommand(unsigned char command);
void SendData(unsigned char data_char);
void Send_a_String(char *stringofchars, uint8_t x, uint8_t y);
void InitializeLCD(void);

void CheckBusy(void)
{
	DataPortDirect = 0x00;
	ControlPort |= 1 << ReadWrite;
	ControlPort &= ~(1 << RegisterS);
	while( DataPort >= 0x80)
	{
		EnableRoutine();
	}
	DataPortDirect = 0xFF;
}

void EnableRoutine(void)
{
	ControlPort |= 1 << EnablePin;
	asm volatile ("nop");
	asm volatile ("nop");
	ControlPort &= ~(1 << EnablePin);
}

void SendCommand(unsigned char command)
{
	CheckBusy();
	DataPort = command & 0xF0;							// command value is masked with 0xF0 to extract upper nibble i.e. 4 MSB bits
	ControlPort &= ~(1 << ReadWrite | 1 << RegisterS);	// upper nibble is sent to LCD
	EnableRoutine();
	DataPort = 0x00;
	DataPort = command << 4;							// command value is shifted by 4 bits to the left to extract lower nibble
	EnableRoutine();									// lower nibble is sent
	DataPort = 0x00;
}

void SendData(unsigned char data_char)
{
	CheckBusy();
	DataPort = data_char & 0xF0;			// data_char value is masked with 0xF0 to extract upper nibble i.e. 4 MSB bits
	ControlPort &= ~(1 << ReadWrite);		// upper nibble is sent to LCD
	ControlPort |= 1 << RegisterS;
	EnableRoutine();
	DataPort = 0x00;
	DataPort = data_char << 4;				// data_char value is shifted by 4 bits to the left to extract lower nibble i.e. 4 LSB bits 
	EnableRoutine();						// lower nibble is sent
	DataPort = 0x00;	
}
void Send_a_String(char *stringofchars, uint8_t x, uint8_t y)
{ 
	// x and y are location co-ordinates on the LCD
	// x represents the line number 0 is first line and 1 is second line
	// y represents the column number 
	
	if(x == 0)
	{
		// Send data to first line
		SendCommand(0x80+y);
	}
	else
	{
		// Send data to second line
		SendCommand(0xC0+y);
	}
	
	while(*stringofchars > 0)
	{
		SendData(*stringofchars++);
	}
}

void InitializeLCD(void)
{
    ControlPortDirect |= ( 1 << EnablePin | 1 << ReadWrite | 1 << RegisterS);
    _delay_ms(15);
    
    SendCommand(0x01); // Clear Screen
    _delay_ms(2);
    SendCommand(0x02); // 4 Bit mode
    _delay_us(50);
	SendCommand(0x28); // Function Set: 4-bit, 2 Line, 5x7 Dots
	_delay_us(50);
	SendCommand(0x06); // 	Entry Mode
    SendCommand(0b00001100); // Cursor Settings Display on Cursor off
    _delay_us(50);
}
#endif