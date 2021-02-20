#include <avr/io.h>
#define F_CPU 1000000UL			// Define operating frequency of  uC for _delay_ms() function
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>


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
void Send_a_String(char *stringofchars);

uint8_t int_flag =0;

int main(void)
{
    ControlPortDirect |= ( 1 << EnablePin | 1 << ReadWrite | 1 << RegisterS);
	_delay_ms(15);
	sei();			// Enable global interrupts
	
	SendCommand(0x01); // Clear Screen
	_delay_ms(2);
	SendCommand(0x38); // 8 Bit mode
	_delay_us(50);
	SendCommand(0b00001110);
	_delay_us(50);
	
	/*
		The internal counter of the uC will be used to count the seconds for the timer
		TCNT1 is the 16 bit timer/counter which counts the clock cycles
		TCCR1B is a control register for TCNT1
		TIMSK is the timer/counter interrupt mask register used to enable timer interrupts
		OCR1A is the 16-bit output compare register
	*/
		
	TCCR1B |= ( 1<<CS10 | 1 << CS11 | 1 << WGM12); // CS10,CS11 are clock select bits used to set pre-scalar to 64. 
												   // WGM12 enables the CTC(Clear Timer on Compare Match). Thus when the TCNT1 value matches the OCR1A value TCNT1 is reset to 0.
	TIMSK |= 1 << OCIE1A;                          // Enables the Timer/Counter1 Output Compare A Match Interrupt.
												   // This generates an interrupt when TCNT1 matches OCR1A to execute ISR
	OCR1A = 15624;								   // Pre-scalar is 64 and clock frequency is 1MHz
												   // 1000000/64 = 15625. So counter value will be 15624 after 1 second as counter counts from 0
	
	Send_a_String("TIMER HH:MM:SS");	// Display Timer Headings on LCD first row
	SendCommand(0xC6);					// Go to LCD second row at correct location below headings
	Send_a_String("00:00:00");			// Send timer initial value
	
	uint8_t i = 0;		// Initialize local variables
	uint8_t j = 0;
	uint8_t k = 0;
		
    while (1) 
    {
		// The timer value will only refresh when ISR sets the int_flag to 1
		
		if(int_flag == 1)
		{
			int_flag = 0;   // Reset the int_flag for next seconds interrupt
			char secs[2];   // character arrays to hold the timer values
			char mins[2];
			char hrs[2];

			i++;			// incrementing 'i' the seconds counter
			if(i < 60)	    // Updating the seconds counter
			{
				itoa(i,secs,10); // itoa converts the integer data type to string data type
				
				if(i < 10)		 // if i < 10 the seconds counter has to be upended witha leading '0'
				{
					SendCommand(0xCC);		// Go to location of seconds on the 2nd line of the LCD
					SendData(0x30);		    // Send '0'
					Send_a_String(secs);    // Send the seconds value
				}
				else
				{
								// if i > 10 the secs char array will hold the tens and units digits repectively
					SendCommand(0xCC);		// Go to location of seconds on the 2nd line of the LCD
					Send_a_String(secs);	// Send the seconds value
				}
			}
			else
			{
			    // Updating the minutes counter
				SendCommand(0xCC);	// Go to location of seconds on the 2nd line of the LCD
				SendData(0x30);		// Send '0'
				SendData(0x30);		// Send '0'
				
				/* The logic for updating the minutes and hours counter from hereon remains same as with seconds */
				
				j++;		
				i=0;	// Initialize seconds counter	
				
				if(j < 59)	 
				{
					itoa(j,mins,10);
					if(j < 10)
					{
						SendCommand(0xC9);
						SendData(0x30);
						Send_a_String(mins);
					}
					else
					{
						SendCommand(0xC9);
						Send_a_String(mins);
					}
				}
				else
				{
					SendCommand(0xC9);
					SendData(0x30);
					SendData(0x30);
					k++;
					j=0;   // Initialize minutes counter 
					itoa(k,hrs,10);
					if(k < 10)
					{
						SendCommand(0xC7);
						Send_a_String(hrs);
					}
					else
					{
						SendCommand(0xC6);
						Send_a_String(hrs);
					}
				}
			}
		}
	}
}

/**************************TIMER1_COMPA_ISR*****************************/
ISR(TIMER1_COMPA_vect)
{
	int_flag = 1;         // Setting interrupt flag 
						  // ISR should involve minimal programming
}

/**************************LCD_functions*******************************/
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
	DataPort = command;
	ControlPort &= ~(1 << ReadWrite | 1 << RegisterS);
	EnableRoutine();
	DataPort = 0x00;
}

void SendData(unsigned char data_char)
{
	CheckBusy();
	DataPort = data_char;
	ControlPort &= ~(1 << ReadWrite);
	ControlPort |= 1 << RegisterS;
	EnableRoutine();
	DataPort = 0x00;
}
void Send_a_String(char *stringofchars)
{
	while(*stringofchars > 0)
	{
		SendData(*stringofchars++);
	}
}