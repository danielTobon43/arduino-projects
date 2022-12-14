;===================================================
;LABORATORIO N0.1 MICROPROCESADORES
;CONVERSI?N SE?AL A/D --> FILTRO PASABAJAS --> CONVERSI?N D/A ***
;INTEGRANTES:
;DANIEL TOB?N COLLAZOS
;DAVID FERN?NDEZ L?PEZ
;JUAN CAMILO P?REZ
;===================================================

.ORG 0x00
JMP START
.ORG 0x2E
JMP OVERFLOW

.DEF TEMP=R16

;===================================================
;INICIO PROGRAMA
;===================================================
 START:
 
 //REFERENCIA VOLTAJE DEL ADC (AVCC=VCC=5V)
		LDI TEMP,(1<<REFS0)|(1<<ADLAR)	;CARGA 1 EN EL BIT REFS0 AL REGISTRO 16 (REFERENCIA EXTERNA AVCC DEL ADC)
		STS ADMUX,TEMP		;ALMACENA EN EL REGISTRO ADMUX UN 1 L?GICO EN EL BIT REFS0

		LDI TEMP,(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
		STS 0x7A,TEMP		;HABILITA EL ADC

//HABILITAR PUERTO DE SALIDA 
		LDI TEMP,0xFF	;CARGA unos EN EL REGISTRO 16
		OUT DDRB,TEMP	;DEFINE C?MO SALIDA EL PUERTO B

//HABILITAR PUERTO DE ENTRADA 
		LDI TEMP,0x00	;CARGA ceros EN EL REGISTRO 16
		OUT DDRF,TEMP	;DEFINE C?MO ENTRADA EL PUERTO F (PUERTO DEL ADC)

;===================================================
;CONFIGURACI?N DEL TIMER
;===================================================
		LDI R16,(1<<CS02)|(1<<CS00)	;PRESCALER DE 1024
		OUT TCCR0B,R16 ;TCCR0B ES EL REGISTRO DONDE SE DEFINE CU?L PRESCALER SE VA A UTILIZAR
		LDI R16,(1<<TOIE0) ;HABILITACI?N DE LA INTERRUPCI?N POR TIMER (OVERFLOW)
		STS TIMSK0,R16 ;TIMER INTERRUPT ENABLE (OVERFLOW)
		SEI ;HABILITACI?N GLOBAL DE INTERRUPCIONES

//INICIO DE CONVERSI?N A/D
		LDI R16,239		;ES EL INICIO DEL TIMER, CU?NDO LLEGUE A 255 HABR?N TRANSCURRIDO 1ms 
		OUT TCNT0,R16	;CU?NDO LLEGUE A 255 OCURRE EL OVERFLOW
		LDS			TEMP,0x7A		; Registro ADCSRA
		ORI			TEMP,(1<<ADSC)
		STS			0x7A,TEMP		; Genera el inicio de conversi?n

		ESPERAR:		;RUTINA EN D?NDE ESPERA QUE TERMINE EL TIEMPO
		NOP
		JMP ESPERAR		;DE UN 1ms

;===================================================
;RUTINA DE ATENCI?N A LA INTERRUPCI?N POR TIMER
;===================================================

		OVERFLOW:

		FinCONV:
		LDS TEMP,0x7A
		SBRS TEMP,4
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
;RECARGA DEL TEMPORIZADOR/RE-INICIO DE CONVERSI?N
;===================================================
		LDI R17,239			;ES EL INICIO DEL TIMER, CU?NDO LLEGUE A 255 HABR? TRANSCURRIDO 1ms 
		OUT TCNT0,R17		;CU?NDO LLEGUE A 255 OCURRE EL OVERFLOW
		LDS	R17,0x7A		;ADCSRA
		ORI	R17,(1<<ADSC)	;
		STS	0x7A,R17		; Genera el inicio de conversi?n

;===================================================
;RETRASOS DE LA SE?AL DE ENTRADA
;===================================================
		STS 0x05,R21	;X2=X1
		STS 0x04,TEMP	;X1=X
				
RETI ;FIN DE LA RUTINA DE ATENCI?N POR INTERRUPCI?N POR TIMER

;===================================================
;FIN DE PROGRAMA
;===================================================