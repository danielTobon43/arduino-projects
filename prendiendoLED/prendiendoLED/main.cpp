/*
 * prendiendoLED.cpp
 *
 * Created: 21/09/2016 8:53:31:PM
 * Author : Daniel
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
int main(void) {
	DDRB = 0xff ;
	while (1) {
		PORTB= 0xff;
		_delay_ms(500);
		PORTB = 0x00;
		_delay_ms(500);
	}
}

