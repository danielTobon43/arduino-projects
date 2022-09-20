.ORG 0x0000
JMP START
.ORG 0x2E
JMP OVERFLOW
.ORG 0x0032
JMP RECCOMPLETED

START:

.DEF TEMP=R16 ;VARIABLE TEMPORAL

;===================================================
;INICIO PROGRAMA 
;===================================================
 
//HABILITAR PUERTO DE SALIDA 
		LDI TEMP,0xFF	;CARGA unos EN EL REGISTRO 16
		OUT DDRB,TEMP	;DEFINE CÓMO SALIDA EL PUERTO B

		LDI R16,(1<<PE1) ;CARGA 1 EN EL BIT 1 DEL PUERTO E
		OUT DDRE,R16	;DEFINE CÓMO SALIDA EL BIT 1 DEL PUERTO E

//HABILITAR PUERTO DE ENTRADA 
		LDI TEMP,0x00	;CARGA ceros EN EL REGISTRO 16
		OUT DDRF,TEMP	;DEFINE CÓMO ENTRADA EL PUERTO F (PUERTO DEL ADC)

;===================================================
;CONFIGURACIÓN DEL ADC
;===================================================

//REFERENCIA VOLTAJE DEL ADC (AVCC=VCC=5V) -->> SE VA UTILIZAR EL ADC0
		LDI TEMP,(1<<REFS0)|(1<<ADLAR)	;CARGA 1 EN EL BIT REFS0 AL REGISTRO 16 (REFERENCIA EXTERNA AVCC DEL ADC)
		STS ADMUX,TEMP		;ALMACENA EN EL REGISTRO ADMUX UN 1 LÓGICO EN EL BIT REFS0

		LDI TEMP,(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
		STS 0x7A,TEMP		;HABILITA EL ADC
		
;===================================================
;CONFIGURACIÓN DEL TIMER
;===================================================
		LDI R16,(1<<CS02)|(1<<CS00)	;PRESCALER DE 1024
		OUT TCCR0B,R16 ;TCCR0B ES EL REGISTRO DONDE SE DEFINE CUÁL PRESCALER SE VA A UTILIZAR
		LDI R16,(1<<TOIE0) ;HABILITACIÓN DE LA INTERRUPCIÓN POR TIMER (OVERFLOW)
		STS TIMSK0,R16 ;TIMER INTERRUPT ENABLE (OVERFLOW)
		SEI ;HABILITACIÓN GLOBAL DE INTERRUPCIONES

;=====================================================
;CONFIGURACIÓN USART 
;=====================================================
		//LDI R16,(1<<U2X0)		;DOBLE SPEED USART
		//STS 0xC0,R16			;REGISTRO UCSR0A
		LDI R16,(1<<RXEN0)|(1<<RXCIE0)	;HABILITAR MODO RECEPCIÓN Y INTERRUPCIÓN POR RECEPCIÓN
		STS UCSR0B,R16			;REGISTRO UCSR0B
		LDI R16,(1<<UCSZ01)|(1<<UCSZ00)	;MODO ASINCRONO, 8 BITS, 1 BIT DE PARADA, NO BITS DE PARIDAD
		STS UCSR0C,R16			;REGISTRO UCSR0C
		LDI R16,103				;103 ->>VELOCIDAD DE 9600 BAUDIOS (BITS POR SEGUNDO)
		STS UBRR0L,R16			;REGISTRO UBRROL 
		LDI R17,0x61			;CARACTER "a" EN HEX
		STS 0x07,R17			;ALMACENA EL CARACTER "a" EN R7
		LDI R18,0x73			;CARACTER "s" EN HEX
		STS 0x08,R18			;ALMACENA EL CARACTER "s" EN R8
		
//INICIO DE CONVERSIÓN A/D
		LDI			R16,239		;ES EL INICIO DEL TIMER, CUÁNDO LLEGUE A 255 HABRÁN TRANSCURRIDO 1ms 
		OUT			TCNT0,R16	;CUÁNDO LLEGUE A 255 OCURRE EL OVERFLOW
		LDS			TEMP,0x7A		;REGISTRO ADCSRA
		ORI			TEMP,(1<<ADSC)
		STS			0x7A,TEMP		;INICIO DE CONVERSIÓN

		ESPERAR:		;RUTINA EN DÓNDE ESPERA QUE TERMINE EL TIEMPO
		JMP ESPERAR		;DE UN 1ms

;===================================================
;RUTINA DE ATENCIÓN A LA INTERRUPCIÓN POR TIMER
;===================================================

		OVERFLOW:

		FinCONV:
		LDS TEMP,0x7A	;DIRECCIÓN DEL REGISTRO ADCSRA
		SBRS TEMP,4		;SKIP NEXT INSTRUCTION IF BIT 4 IS SET IN TEMP
		JMP FinCONV		
			
;===================================================
;DATO DE SALIDA DEL ADC 
;===================================================
		
		LDS R25,0x78	;ADCL
		LDS TEMP,0x79	;ADCH
				
;===================================================
;FILTRO
;===================================================
		MOV R18,TEMP	;X
		LSR R18			;X/2
		LSR R18			;X/4 
		LDS R19,0x04	;X1
		MOV R21,R19		;R21=X1
		LSR R19			;X1/2
		LDS R20,0x05	;R20=X2
		LSR R20			;X2/2
		LSR R20			;X2/4
		ADD R18,R19		;SUMA: X*0.25+(2*0.25*X1)
		ADD R18,R20		;SUMA: X*0.25+(2*0.25*X1)+X2*0.25
		
;===================================================
;ESCRITURA DEL FILTRADO EN EL PUERTO B
;===================================================
		
		OUT PORTB,R18	;ESCRIBE EN EL PUERTO B LA SALIDA DIGITAL FILTRADA

;===================================================
;COMPARACIÓN DEL CARACTER DE ENTRADA DEL USART
;===================================================
		
		CP R8,R6
		BREQ PERIODO2
		CP R7,R6
		BREQ PERIODO1
		

//CARGA DEL TEMPORIZADOR POR DEFECTO
		LDI R17,239			;ES EL INICIO DEL TIMER, CUÁNDO LLEGUE A 255 HABRÁ TRANSCURRIDO 1ms 
		OUT TCNT0,R17		;CUÁNDO LLEGUE A 255 OCURRE EL OVERFLOW
		LDS	R17,0x7A		;ADCSRA
		ORI	R17,(1<<ADSC)	;
		STS	0x7A,R17		; Genera el inicio de conversión
		JMP ATRASO
				
;===================================================
;RECARGA DEL TEMPORIZADOR/RE-INICIO DE CONVERSIÓN
;===================================================
PERIODO1:
		LDI R17,239			;ES EL INICIO DEL TIMER, CUÁNDO LLEGUE A 255 HABRÁ TRANSCURRIDO 1ms 
		OUT TCNT0,R17		;CUÁNDO LLEGUE A 255 OCURRE EL OVERFLOW
		LDS	R17,0x7A		;ADCSRA
		ORI	R17,(1<<ADSC)	;
		STS	0x7A,R17		; Genera el inicio de conversión
		JMP ATRASO

PERIODO2:
		LDI R17,223			;ES EL INICIO DEL TIMER, CUÁNDO LLEGUE A 255 HABRÁ TRANSCURRIDO 2ms 
		OUT TCNT0,R17		;CUÁNDO LLEGUE A 255 OCURRE EL OVERFLOW
		LDS	R17,0x7A		;ADCSRA
		ORI	R17,(1<<ADSC)	;
		STS	0x7A,R17		; Genera el inicio de conversión
		JMP ATRASO

;===================================================
;RETRASOS DE LA SEÑAL DE ENTRADA
;===================================================
ATRASO:
		STS 0x05,R21	;X2=X1
		STS 0x04,TEMP	;X1=X
				
		RETI ;FIN DE LA RUTINA DE ATENCIÓN POR INTERRUPCIÓN POR TIMER

;===================================================
;RUTINA DE ATENCIÓN A LA INTERRUPCIÓN POR USART 
;===================================================
RECCOMPLETED:
		RECEND:
		LDS R16,0xC0	;CARGA LA DIRECCIÓN DEL REGISTRO UCSR0A
		SBRS R16,7		;SKIP NEXT INTRUCTION IF BIT 7 IS SET IN R16 (BIT DE BANDERA DATO RECIBIDO)
		JMP RECEND		

		LEERDATO:
		LDS R16,0xC6	;CARGA LA DIRECCIÓN DEL REGISTRO UDR0
		STS 0x06,R16
		
		LDI R16,(0<<RXC0)	;CARGA 0 EN EL BIT DE BANDERA DE DATO RECIBIDO
		STS 0xC0,R16		;ALMACENA EL REGISTRO UCSR0A

		LDI R16,(1<<UDRE0)
		STS 0xC0,R16


		RETI;FIN DE LA RUTINA DE ATENCIÓN POR INTERRUPCIÓN POR USART


;===================================================
;FIN DE PROGRAMA
;===================================================