
#include <avr/io.h>
#define F_CPU 12000000UL //DEFINE 16MHz DE CRISTAL
#include <util/delay.h>

int main(void)
{
	DDRB = 0B00001111; //PUERTO B CÓMO SALIDA
	
    asm("ESPERAR:");
	   PORTB = 0B00001100;
	   _delay_ms(8000);
	   PORTB = 0B00000000;
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		PORTB = 0B00000001;
		_delay_ms(8000);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		_delay_ms(500);
		PORTB = 0B00000010;
		_delay_ms(500);
		PORTB = 0B00000000;
		
	   asm("JMP ESPERAR");
		
    
}

