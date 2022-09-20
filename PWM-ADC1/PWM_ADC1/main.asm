;
; PWM_ADC1.asm
;
; Created: 19/10/2016 12:54:59 p.m.
; Author : dmartinez
;
;====================================================================
; DEFINICIONES INICIALES
;====================================================================     

			.DEF TEMP 	= R16	; REGISTRO TEMPORAL

;====================================================================
; VECTORES DE INTERRUPCION
;====================================================================     
			
			.ORG		0x0000		; Vector del Reset
			JMP 		Inicio


			.ORG		0x002E		; Ventor Timer0 OV 
			JMP 		InterrupcionT0

;====================================================================
; PROGRAMA
;====================================================================

Inicio:

;====================================================================
; CONFIGURACIÓN ADC
;====================================================================


			LDI			TEMP,1<<REFS0|1<<ADLAR	; Vref en AVCC (VCC de la Mega), Ajuste a la izquierda del resultado y Selecciona el canal 0
			STS			0x7C,TEMP				; Registro ADMUX

			LDI			TEMP,1<<ADEN|1<<ADPS2|1<<ADPS1|1<<ADPS0	; Habilita el ADC y establece un prescaler de 128
			STS			0x7A,TEMP;							; Registro ADCSRA

			LDI			TEMP,0x00
			STS			0x7B,TEMP							; Registro ADCSRB

;====================================================================
; CONFIGURACIÓN PWM - Timer0
;====================================================================

			LDI			TEMP,0xFF
			OUT			DDRB,TEMP						; Define el Puerto B como salida, Salida PWM - OC0A
			OUT			DDRC,TEMP						; Define el Puerto B como salida, allí se escriben los valores convertidos por el ADC


			LDI			TEMP,1<<COM0A1|0<<COM0A0|1<<WGM01|1<<WGM00	; Compara con OCR0A, Fast PWM Modo 3
			STS			0x44,TEMP									; TCCR0A

			;CONFIGURACIÓN DEL SET TIMER
			LDI			TEMP,0x1F						; SE PRECARGA EL TIMER 0, TCNT0, 
			STS			0x46,TEMP						; CON 31

			LDI			TEMP,0x64						; VALOR DE COMPARACIÓN PWM, OCR0A,
			STS			0x47,TEMP						; DESBORDAR A 1Seg

			;CONFIGURACIÓN DEL PRESCALER - AQUÍ INICIA EL TIMER
			LDI			TEMP,0<<CS02|0<<CS01|1<<CS00	; PRESCALER DE 1, TCCR0B,
			STS			0x45,TEMP

;====================================================================
; HABILITAR INTERRUPCIONES TIMER 0 Y GLOBALES
;====================================================================	
			LDI			TEMP,1<<TOIE0					; Habilitación de INT_TIMER_0
			STS			TIMSK0,TEMP			

			SEI											; Habilitador global de interrupciones

;====================================================================
; BUCLE INFINITO : LOOP
;====================================================================	

BUCLE:      NOP
			JMP			BUCLE							;BUCLE INFINITO

;====================================================================
; Subrutina de Atención a la Interrupción del Timer 0
;====================================================================	

InterrupcionT0:
	
			;CONFIGURACIÓN DEL SET TIMER

			LDI			TEMP,0x1F						; SE PRECARGA EL TIMER 0, TCNT0, 
			STS			0x46,TEMP						; CON 31

			; Lectura del ADC

			LDS			TEMP,0x7A		; Registro ADCSRA
			ORI			TEMP,(1<<ADSC)
			STS			0x7A,TEMP		; Genera el inicio de conversión

Esperar_EOC:	LDS			TEMP,0x7A	; Espera el fin de conversión
				SBRC		TEMP,6		; ADSC se pone en 0 cuando finaliza la conversión
				JMP			Esperar_EOC

			LDS			TEMP,0x79		; Escribe el valor del ADC (ADCH) en el puerto C
			OUT			PORTC,TEMP
			
			RETI
			

;======================================================================
; FIN DEL PROGRAMA
;======================================================================

