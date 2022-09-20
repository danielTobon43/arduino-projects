/*
 * ProyectoFinalMicro.cpp
 *
 * Created: 09/11/2016 07:42:51 p. m.
 * Author : Daniel Tobon, Camilo Perez & David Fernandez 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 12000000UL



 void encenderMotor();
 void apagarMotor();
 void encenderValvula();
 void apagarValvula ();
 void encenderVentilador();
 void apagarVentilador(); 

int main(void)
{
    //===================================================
    //INICIO PROGRAMA
    //===================================================
    cli();
    
    //HABILITAR PUERTOS DE SALIDA Y ENTRADA
    DDRB = 0xFF; //DEFINE CÓMO SALIDA EL PUERTO B,PUERTOS QUE ENVIARA LAS SEÑALES DE ACTIVACION DEL MOTOR Y LA ELECTROVALVULA 
    DDRE = (0<<PINE4)|(0<<PINE5)|(0<<PINE6); //DEFINE COMO ENTRADA DE LAS INTERRUPCIONES 4 Y 5.
    DDRF = (0<<PINF0); //DEFINE CÓMO ENTRADA EL PUERTO F (PUERTO DEL ADC), ENTRARA LA SEÑAL ENVIADA POR EL LM35
    DDRD = (0<<PIND0)|(0<<PIND1)|(0<<PIND2)|(0<<PIND3); // DEFINE EL PUERTO D COMO ENTRADA DE SEÑALES LOGICAS ( SENSORES DE POSICION Y FINALES DE CARRERA E INTERRUPCIONES )
	DDRA = (1<<PINA0)|(1<<PINA1);
    //===================================================
    //CONFIGURACIÓN DEL ADC
    //===================================================
    
    ADMUX = (1<<REFS0)|(1<<ADLAR); //REFERENCIA AVCC=5V,JUSTIFICACIÓN DEL DATO A LA IZQUIERDA
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //HABILITACIÓN DEL ADC,PRESCALER DE 128
    ADCSRB = 0x00; //SELECCIÓN DEL ADC (ADC0)
	
	//===================================================
	// CONFOGURACION DE INTERRUPIONES EXTERNAS
	//===================================================
	
	EIMSK = (1<<INT0)|(1<<INT1)|(1<<INT2)|(1<<INT3)|(1<<INT4)|(1<<INT5);
	EICRA = (1<<ISC01)|(1<<ISC00);
	    
    //===================================================
    //CONFIGURACIÓN DEL TIMER 1
    //===================================================
    	
    TCCR1B = (1<<CS12)|(1<<CS10); //PRESCALER DE 1024
    TIMSK1 = (1<<TOIE1); //HABILITACIÓN DE LA INTERRUPCIÓN POR OVERFLOW
	
	
    //===================================================
    //CONFIGURACIÓN USART
    //===================================================
     UCSR0B = (1<<TXEN0)|(1<<TXCIE0); // MODO DE TRANSMISION  E INT_PORT TRANSMISION DEL DATO DE TEMPERATURA 
     UCSR0B = (1<<RXEN0)|(1<<RXCIE0);	//MODO RECEPCIÓN E INT_POR RECEPCIÓN
     UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);//MODO ASINCRONO, 8 BITS, 1 BIT DE PARADA, NO BITS DE PARIDAD
     UBRR0L = 71;					//103 ->>VELOCIDAD DE 9600 BAUDIOS (BITS POR SEGUNDO)
	 
     sei(); //HABILITACIÓN GLOBAL DE INTERRUPCIONES
	 
	
	 
	asm("ESPERAR:");
	asm("JMP ESPERAR");
	  
	
	 }
	 
	 // INTERRUPCION BOTON DE ENCENDIDO, ENCIENDE LED VERDE
	/*
	 ISR(INT0_vect){		 
		 PORTB=0x01;
			  } 	*/
	// INTERRUPCION FINAL DE CARRERA 1, ENCIENDE LED AZUL	  
	ISR(INT1_vect){
		
		PORTB=0B00000001;
		
	    }
	
	// INTERRUPCION BOTON DE MARCHA , MUEVE MOTOR
	ISR(INT2_vect){
		if (PORTB&(1<<PINB0))
		{
			PORTB = 0B0000011;
		}
		else
		{
			PORTB = 0B0000001;
		
		}
		}
	// INTERRUPCION SENSOR NUMERO 1, ENCIENDE ELECTROVALVULA Y APAGA MOTOR 		
	ISR(INT3_vect){
		
		
		encenderMotor();
		
		
	}	
	// INTERRUPCION SENSOR NUMERO 2, ENCIENDE VENTILADOR
	ISR (INT4_vect)	{
		
			apagarMotor();
			
			TCNT1H = 0X85;
		TCNT1L = 0xEE;}
		
	
        
		
	ISR(INT5_vect){
		if (PORTB== 0B00001001){
			apagarMotor();
			encenderVentilador();
			TCNT1H = 0X85;
			TCNT1L = 0xEE;
		}
		else {
			encenderMotor();
			
		}	
	}
	
	ISR(INT6_vect){
		if (PORTD&(1<<DDD5)){
		apagarMotor();
		PORTB = 0B00000100;
	}
	
	else {
		encenderMotor();
	}
		
	}
	
	 
	
	  
	  
	// BOTON DE MARCHA ACCIONADO - MUEVE EL MOTOR HASTA EL PRIMER SENSOR DE POSICION
		 
		void encenderMotor ()
	    { 
				PORTB = 0b00000111;
		 
		 }
		 
	// FUNCION DE APAGAR MOTOR	 
	 void apagarMotor() {
		  
			 PORTB = 0B00001011;
		  		 
	 }
	 
	// FUNCION DE ENCENDER VALVULA 
	 void encenderValvula ()	{
		
			 PORTB = 0B00010001;
		 
	 }  
	 // FUNCION DE APAGAR VALVULA 
	 void apagarValvula(){
		 
		 PORTB = 0B00000001;
	 }
	 
	 // FUNCION DE ENCENDER VENTILADOR
	 void encenderVentilador()
	 {
		 PORTB = 0B00100001;
	 }
     // FUNCION DE APAGAR VENTILADOR		 
	void apagarVentilador(){
		
		PORTB = 0B00000001;
	}
		 	 
		 
	// ATENCION A LA INTERRUPCION POR TIMER NUMERO 1.
	 	 
	ISR(TIMER1_OVF_vect){
		if (PORTB == 0B00010001){
			apagarValvula();
			encenderMotor();
		} 
		else if (PORTB == 0B00100001)
		{
	       apagarVentilador();
		   encenderMotor();
	    }
	}
	 
			
			
			
			
			
		
		 
	

