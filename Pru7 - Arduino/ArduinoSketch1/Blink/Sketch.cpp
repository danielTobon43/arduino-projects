
/*
 * Created: 25/10/2016 05:36:55 p.m.
 * Author : dmartinez
 */ 


#include "Mi_libreria/Mi_libreria.c"
#include <Arduino.h>


int	z, y=0;


void setup() {	// the setup function runs once when you press reset or power the board
  
	Serial.begin(115200);		// Configuración del puerto serial

	pinMode(13, OUTPUT);		// initialize digital pin 13 as an output.

	// Configuración del TIMER1
	
	 asm("LDI R16,0x05						;PRESCALER");
	 asm("STS 0x81,R16						;AQUÍ INICIA EL TIMER");	//0x81 = TCCR1B
	 
	 asm("LDI R16,0x00						;SE PRECARGA EL TIMER 1");
	 asm("STS 0x85,R16						;0x85 = TCNT1H");
	 asm("LDI R16,0x00						");						
	 asm("STS 0x84,R16						;0x84 = TCNT1L");
	 
	 asm("LDI R16,1<<0						;HABILITACIÓN DE INT");
	 asm("STS 0x6F,R16						;DE TIMER 1");				//0x6F = TIMSK1

	// Habilitación de Interrupciones
	
	sei();
	
}



void loop() {	// the loop function runs over and over again forever
	
	digitalWrite(13, HIGH);		// turn the LED on (HIGH is the voltage level)

	digitalWrite(13, LOW);		// turn the LED off by making the voltage LOW
  
}


ISR(TIMER1_OVF_vect){	// Subrutina de atención a la interrupción del Timer 1
	
	asm("LDI R16,0x00			;SE PRECARGA EL TIMER 1");
	asm("STS 0x85,R16			;0x85 = TCNT1H");
	asm("LDI R16,0x00			");
	asm("STS 0x84,R16			;0x84 = TCNT1L");

	z=incrementar(y);
	y=z;

	Serial.print("Valor = ");
	Serial.println(z);
 
}

