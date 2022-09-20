
#include <avr/io.h> //INCLUYE EL ARCHIVO FUENTE
#include <avr/interrupt.h> //INCLUYE LA LIBRERÍA DE INTERRUPCIONES
#define F_CPU 16000000UL //DEFINE 16MHz DE CRISTAL

unsigned char filtro(unsigned char,unsigned char,unsigned char); //DECLARA LA FUNCIÓN FILTRO
unsigned char caracter; //DECLARA "caracter" CÓMO VARIABLE GLOBAL

int main(void)//INICIO DEL PROGRAMA PRINCIPAL
{
		
	//===================================================
	//INICIO PROGRAMA
	//===================================================
	
	
	//HABILITAR PUERTOS DE SALIDA Y ENTRADA
	DDRB = 0xFF; //DEFINE CÓMO SALIDA EL PUERTO B
	DDRE = 0x01; //DEFINE CÓMO SALIDA EL PIN TXD (PUERTO DEL USART)
	DDRF = 0x00; //DEFINE CÓMO ENTRADA EL PUERTO F (PUERTO DEL ADC)
	
	//===================================================
	//CONFIGURACIÓN DEL ADC
	//===================================================
	
	ADMUX = (1<<REFS0)|(1<<ADLAR); //REFERENCIA AVCC=5V,JUSTIFICACIÓN DEL DATO A LA IZQUIERDA
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //HABILITACIÓN DEL ADC,PRESCALER DE 128
    ADCSRB = 0x00; //SELECCIÓN DEL ADC (ADC0)
   
   //===================================================
   //CONFIGURACIÓN DEL TIMER
   //===================================================
   
   TCCR0B = (1<<CS02)|(1<<CS00); //PRESCALER DE 1024
   TIMSK0 = (1<<TOIE0); //HABILITACIÓN DE LA INTERRUPCIÓN POR OVERFLOW
   sei(); //HABILITACIÓN GLOBAL DE INTERRUPCIONES
   
   //===================================================
   //CONFIGURACIÓN USART
   //===================================================
  
   UCSR0B = (1<<RXEN0)|(1<<RXCIE0);	//MODO RECEPCIÓN E INT_POR RECEPCIÓN
   UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);//MODO ASINCRONO, 8 BITS, 1 BIT DE PARADA, NO BITS DE PARIDAD
   UBRR0L = 103;					//103 ->>VELOCIDAD DE 9600 BAUDIOS (BITS POR SEGUNDO)
     
   //INICIO DE CONVERSIÓN
   TCNT0 = 239; //PRECARGA DEL TIMER (1ms)
   ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADSC); //INICIO DE CONVERSIÓN
   
	asm("ESPERAR:");//CICLO DE ESPERA DE 1ms
	asm("JMP ESPERAR");  
  
}//FIN DEL PROGAMA PRINCIPAL

//====================================================================================


 //===================================================
 //FUNCIÓN DEL FILTRO
 //===================================================
 
	unsigned char filtro (unsigned char actual,unsigned char prAtraso, unsigned char segAtraso){
	unsigned char y;
	y = actual*0.25+(2*0.25*prAtraso)+segAtraso*0.25;
	return y;
	
}

   //===================================================
   //RUTINA DE ATENCIÓN A LA INTERRUPCIÓN POR TIMER
   //===================================================
   
	ISR(TIMER0_OVF_vect){
		
		asm("FinCONV:");		
		asm("LDS R16,0x7A");	//DIRECCIÓN DEL REGISTRO ADCSRA
		asm("SBRS R16,4");		//SKIP NEXT INSTRUCTION IF BIT 4 IS SET IN R16
		asm("JMP FinCONV");
		
				unsigned char datoACTUALadc=0; //x[k]
				unsigned char dato1AtrasoADC=0;//x[k-1] 
				unsigned char dato2AtrasoADC=0;//x[k-2]
				unsigned char caracter2=0x61; //CARACTER "a" EN HEX
				unsigned char caracter1=0x73;//CARACTER "s" EN HEX				
				unsigned char datoFiltrado; //DECLARA VARIABLE DATO FILTRADO
								
				//===================================================
				//DATO DE SALIDA DEL ADC
				//===================================================
				
				asm("LDS R25,0x78");	//ADCL
				datoACTUALadc = ADCH; //ASIGNA LA PARTE ALTA DEL ADC A LA VARIABLE "datoACTUALadc"
				
				//===================================================
				//FILTRO
				//===================================================
				
				datoFiltrado=filtro(datoACTUALadc,dato1AtrasoADC,dato2AtrasoADC); //LLAMA LA FUNCIÓN FILTRO
				
				//===================================================
				//ESCRITURA DEL FILTRADO EN EL PUERTO B
				//===================================================
				
				PORTB = datoFiltrado; //ESCRIBE EN EL PUERTO B LA SEÑAL FILTRADA
				
				//===================================================
				//COMPARACIÓN DEL CARACTER DE ENTRADA DEL USART
				//===================================================
				
				if (caracter==caracter2){ //¿caracter = a?
					TCNT0 =239; //CARGA EN EL TIMER 1ms
					ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADSC); //RE-INICIO DE CONVERSIÓN DEL ADC
					
				}else if (caracter==caracter1) //¿caracter = s?
				{
					TCNT0=223;		//CARGA EN EL TIMER 2ms
					ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADSC);//RE-INICIO DE CONVERSIÓN DEL ADC
					
					}else{
					TCNT0 =239; //CARGA DEL TIMER DE 1ms
					ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADSC);//RE-INICIO DE CONVERSIÓN DEL ADC
				}
				
				//===================================================
				//RETRASOS DE LA SEÑAL DE ENTRADA
				//===================================================
				
				dato2AtrasoADC=dato1AtrasoADC;
				dato1AtrasoADC=datoACTUALadc;
			
	}
	
  //===================================================
  //RUTINA DE ATENCIÓN A LA INTERRUPCIÓN POR USART
  //===================================================
  
	ISR(USART0_RX_vect){
		
		asm("RECEND:");
		asm("LDS R16,0xC0");//CARGA LA DIRECCIÓN DEL REGISTRO UCSR0A
		asm("SBRS R16,7");	//SKIP NEXT INTRUCTION IF BIT 7 IS SET IN R16 (BIT DE BANDERA DATO RECIBIDO)
		asm("JMP RECEND");
		
		caracter=UDR0; //CARGA EL VALOR DE LA TECLA PRESIONADA EN "caracter"		
		UCSR0A=(1<<UDRE0);//CARGA EN 1 EL BIT DE BANDERA INDICANDO QUE EL REGISTRO ESTÁ VACÍO 
		//Y AHORA PUEDE ACEPTAR NUEVO CONTENIDO

}

 //===================================================
 //FIN DE PROGRAMA
 //===================================================