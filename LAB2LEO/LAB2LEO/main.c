/*
 * LAB2LEO.c
 *
 * Created: 7/11/2016 5:55:29 p. m.
 * Author : Karem
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU/(BAUDRATE* 16UL)))-1)


//funciones globales

void iniciolcd(void);
void ADC_inicio(void);
void enable2();
void enable();
void Retardo();
void USART();

#define BINBCD(val)((((val)/10)<<4)+ (val)%10)


volatile char R;
volatile char N1=0;
volatile char N2=0;

char String []="Temp:";
 
ISR(TIMER1_OVF_vect){
	UDR0=N1;
	UDR0=N2;
	
	ADCSRA |=((1<<ADSC));
	while((ADSC&(1<<6))); //espera star conver
	char val=0;
	
	val=ADCH;
	val= val*2;
	R=BINBCD(val);
	N1= R & 0xF0;
	N2= R & 0x0F;
	N1= ((N1 & 0x0F)<<4)|((N1 & 0xF0)>>4); 
	N2= N2+ 48;
	N1= N1+ 48;
	
	//USART_send(' ');
	
	enable2();
	PORTC= N1;
	Retardo();
	
	enable2();
	PORTC= N2;
	Retardo();

	Retardo();
	enable();
	PORTC=0x06;
	
	enable();
	PORTC=0x02;
	Retardo ();
	
}
Retardo(){
	asm("LDI r20,104");
	asm("CICLO2: LDI r21,255");
	asm("CICLO1: DEC	r21");
	asm("BRNE	CICLO1");
	asm("DEC	r20");
	asm("BRNE	CICLO2");
	return 0;
}

enable (){
	PORTB=0x00;
	Retardo();
	PORTB=0x04;
}

enable2(){
	PORTB= 0x01;
	Retardo();
	PORTB= 0x05;
}

iniciolcd(){
	DDRB= DDRC = 0xFF;
	enable();
	PORTC= 0x30; // funtion set
	Retardo();
	
	enable();
	PORTC= 0x0E; // display on/off
	Retardo();
	
	enable();
	PORTC= 0x01; // clear display
	Retardo();
	
	enable();
	PORTC= 0x07; // entry mode set
	Retardo();
}


 USART(){

UCSR0B=0b00011000;//UCSR0B, Habilita la transmisión y recepción
UCSR0C=0b00000110; //UCSR0C, asincrono, sin paaridad, 1 bit de stop, 8 bits de datos
UBRR0=0x08;

}

int main(void)
{
   TCCR1B=0b00000101;
   TCNT1L=0xF6;
   TCNT1H=0xC2;
   TIMSK1=1;
   iniciolcd();
   USART(); // inicializa usart
   ADMUX= 0x60;
   ADCSRA= 0x87;
   ADCSRB= 0x00;
   sei();
    while (1) 
    {
    }

}

