#include <avr/io.h>
#define F_CPU 1000000UL		// Define operating frequency of the uC for the _delay_ms function
#include <util/delay.h>

int main(void)
{
	DDRB |= 1 << PINB0;		// Configure PINB0 as Output
	PORTB = 0b00000001;		// Making PINB0 High ( Switching on the LED )
	
	while(1)				// The following code block will loop till power is applied to uC
	{
       PORTB ^= 1 << PINB0; // Toggling PINB0 ( If pin is high it will go low and vice-versa. i.e. switching LED on and off)
	   _delay_ms(300);		// Delay added to see the toggling effect
	}
}