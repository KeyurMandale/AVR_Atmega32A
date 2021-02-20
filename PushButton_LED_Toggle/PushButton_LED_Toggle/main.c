#include <avr/io.h>
#define F_CPU 1000000UL		// Define operating frequency of  uC for _delay_ms() function
#include <util/delay.h>
#include <avr/sfr_defs.h>	// Header file containing the bit_is_clear() function

int main(void)
{
	DDRB |= 1 << PINB0;		// Configure PINB0 as O/P i.e. LED1
	DDRB |= 1 << PINB2;		// Configure PINB2 as O/P i.e. LED2
	DDRB &= ~(1 << PINB1);	// Configure PINB1 as I/P i.e. Push Button
	PORTB |= 1 << PINB1;	// Make PINB1 High
	
	while (1)
	{
		if(bit_is_clear(PINB, 1))	// Function bit_is_clear will return True if PINB1 is read as '0' i.e. Button is pushed
		{
			//LED2 will keep toggling if Button is pushed
			PORTB &= ~(1 << PINB0); // Make PINB0 Low
			PORTB ^= 1 << PINB2;    // Toggle PINB2
		}
		else
		{
			//LED1 will keep toggling if Button is not pressed
			PORTB &= ~(1 << PINB2); // Make PINB2 Low
			PORTB ^= 1 << PINB0;	// Toggle PINB0;
		}
		_delay_ms(100);				// Delay of 100ms between each toggle to see toggling effect
	}
}

