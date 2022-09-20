start:
/*
LDI R16, 0xFF
OUT DDRC, R16
LDI R16, 0x00
OUT PORTC, R16

LDI R16, (1<<XMM1)
STS XMCRB, R16

LDI R16, 
0xaa
STS 0x0001+OFFSET, R16

LDI R16, (0<<XMM1)
STS XMCRB, R16

LDI R16, 0x55
STS 0x0001+OFFSET, R16

CLR R27
LDI R26,0
LD R16,X+
LD R17,X+
MOV R18,R16
MUL R18,R17
NOP
*/

/*EJEMPLO SUBRUTINA
MOV R16,R0
CALL CHECK
NOP
CHECK:
CPI R16,0x2A
BREQ ERROR
RET
ERROR:
RJMP ERROR
*/

.DEF TEMP 	= R16	;REGISTRO TEMPORAL
.DEF TEMP1	= R17	;REGISTRO TEMPORAL1

;sensores	=>	PORTA; Tiene conectado todos los sensores
;visualizador	=>	PORTB; Visualiza el estado de los sensores
;actuadores	=>	PORTC; Tiene conectado todos los actuadores

		LDI	TEMP1,0x00
		OUT	DDRA,TEMP1

		LDI	TEMP1,0xFF
		OUT	DDRB,TEMP1

		LDI	TEMP1,0xFF
		OUT	DDRC,TEMP1

inicio:
     	IN	TEMP,PINA	; Lee el estado de los sensores
		CALL	visualizar	; Muestra el estado de los sensores
		SBRS	TEMP,0		; Salta a apagar
		JMP	prender
apagar:
    	CBI	PORTC,0	; Si el sensor del bit0 está encendido
		JMP	inicio		; apaga el actuador bit 0
prender:
     	SBI	PORTC,0	; Si el sensor del bit0 está apagado
		JMP	inicio		; enciende el actuador bit 0

visualizar:
    	OUT	PORTB,TEMP
		LDI	TEMP,0x0
		RET
