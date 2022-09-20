;
; Primer laboratorio.asm
;
; Created: 24/10/2016 10:50:48 p. m.
; Author : Karem
;
; Replace with your application code

.def TEMP= R16	; REGISTRO TEMPORAL
.def BCD1 = r21
.org 0x0000		; Vector del Reset
JMP 		start

.org $0028		; Vector Timer1 overflow
JMP  InterrupcionT1


start:

ldi r20,0xFF 

out DDRB,r20  //Puerto de control: RS=Bit0, R/W=Bit1, E=Bit2.
out DDRC,r20  //Puerto para manejo del bus de datos 8Bits.


call enable
ldi r16,0x30 //1.Funtion set
out PORTC,r16
call delay

call enable
ldi r18,0x0E //2.Display on/off
out PORTC,r18
call delay

call enable
ldi r17,0x01 //3.Clear display
out PORTC,r17
call delay

call enable
ldi r19,0x07 //4.Entry mode set
out PORTC,r19
call delay

;====================================================================
; CONFIGURACION TIMER1
;====================================================================
ldi r16,0x00
sts TCCR1A,r16//DECLARAR TEMPORIZADOR
ldi r17,0x04 // preescala a 256
sts TCCR1B,r17
ldi r18,0x01
sts TIMSK1,r18
sei



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
; BUCLE INFINITO : LOOP
;====================================================================	

BUCLE:      NOP
			JMP			BUCLE							;BUCLE INFINITO

;====================================================================
; Subrutina de Atención a la Interrupción del Timer 0
;====================================================================	

InterrupcionT1:

				; Lectura del ADC

			LDS			TEMP,0x7A		; Registro ADCSRA
			ORI			TEMP,(1<<ADSC)
			STS			0x7A,TEMP		; Genera el inicio de conversión

Esperar_EOC:

LDS			TEMP,0x7A	; Espera el fin de conversión
SBRC		TEMP,6		; ADSC se pone en 0 cuando finaliza la conversión
JMP			Esperar_EOC

LDS			TEMP,0x79		; Escribe el valor del ADC (ADCH) en el puerto C
lsl         TEMP

;====================================================================
; CONVERSOR A BCD
;====================================================================	

BCD:
	clr	BCD1			;clear temp reg
BCDa:
	subi	Temp,10		;input = input - 10
	brcs	BCDb		;abort if carry set
	subi	BCD1,-$10		;BCD1 = BCD1 + 10
    rjmp	BCDa	;loop again

BCDb:
	subi Temp,-10		;compensate extra subtraction
	add	Temp,BCD1

    swap TEMP 	
		
	ldi r19,0x0F //
	ldi r20,0XF0
    ldi r21,0x00
	ldi r22,0x00
	MOV r21,TEMP
	MOV r22, TEMP
	AND r21,r19  //segundo
	AND r22,r20  //primero
	swap r22

LDI R24, 0b00110000
ADD R21,R24
ADD R22,R24


call enable3  // letra T
ldi r26,84
out PORTC,r26
call delay

call enable3  // Letra E
ldi r27,69
out PORTC,r27
call delay

call enable3  // Letra M
ldi r28,77
out PORTC,r28
call delay

call enable3 // Letra p
ldi r29,80
out PORTC,r29
call delay

call enable3 // Letra =
ldi r30,61
out PORTC,r30
call delay

call enable2  // numero 1
out PORTC,r21
call delay

call enable2  // numero 2
out PORTC,r22
call delay

call delay
call enable
ldi r19,0x06 //4.Entry mode set
call delay
out PORTC,r19

call enable
ldi r16,0x02 //1.return home
out PORTC,r16
call delay	

call enable
ldi r19,0x06 //4.Entry mode set
out PORTC,r19
call delay

RETI

enable:
ldi r21,0x00
ldi r22,0x04 //Enviar al puerto B el direccionamiento
out PORTB,r21
call delay
out PORTB,r22
ret

enable2:
ldi r25,0x01
ldi r16,0x05
out PORTB,r25
call delay
out PORTB,r16
ret

enable3:
ldi r26,0x01
ldi r16,0x05
out PORTB,r26
call delay
out PORTB,r16
ret

delay:
.def cont1=r23
.def cont2=r24                
ldi cont1,46
lazo2:
Ldi cont2,46
lazo1:
dec cont2
brne lazo1
dec cont1
brne lazo2
ret
