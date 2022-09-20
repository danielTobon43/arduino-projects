;Suma con acarreo
;start:
;	LDI R18,2
;	LDI R19,6
;	LDI R20,8
;	LDI R21,5
;   ADD R20,R18
;	ADC R21,R19
	
	
;Ejemplo 1
	
	;CLR R27
	;LDI R26,$2
	;LD R0,X+
	;LD R1,X
	;LDI R26,$63
	;LD R2,X
	;LD R3,-X
	
;Ejemplo 2

	;CLR R27
	;LDI R26,$60
	;ST X+,R0
	;ST X, R1
	;LDI R26,$63
	;ST X,R2
	;ST -X,R3

;Ejemplo 3
	;Carga indirecta de datos con el apuntador X
	;CLR R27			;Resetea en ceros la parte alta del registro apuntador X
	;LDI R16,$05		;Carga el valor 5 en el registro 16
	;LDI R18,$04		;Carga el valor 4 en el registro 18
	;LDI R19,$03		;Carga el valor 3 en el registro 19
	;LDI R20,$02		;Carga el valor 2 en el registro 20
	;LDI R21,$01		;Carga el valor 1 en el registro 21
	;LDI R22,$00		;Carga el valor 0 en el registro 22
	;LDI R23,$03		;Carga el valor 3 en el registro 23
	;CLR R0			;Resetea el registro 0
	;COM R0			;Complemento A1 en el registro 0
	;LDI R26,16		;Posiciona el apuntador X a la posición de memoria 16
	;LD R24,X		;Carga indirectamente el registro 24 con el contenido de la posición de memoria al que apunta el apuntador X

;Ejemplo 4
	;IN R25,$16
	;CPI R25,4
	;BREQ exit

;Ejemplo 5
	;CLR R16
	;SER R17
	;OUT $18,R16
	;NOP
	;OUT $18,R17

;Ejemplo 6

;ldi r16,(1<<PB7)|(1<<PB6)|(1<<PB1)|(1<<PB0)
;out PORTB,r16
;ldi r17,(1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0)
;out DDRB,r17
; Insert nop for synchronization
;nop
; Read port pins
;in r16,PINB
;OFFSET is defined to 0x4000 to ensure external memory 
; access
; Configure Port C (address high byte) to output 0x00 when the 
; pins are released for normal Port Pin operation
;ldi r16, 0xFF
;out DDRC, r16
;ldi r16, 0x00
;out PORTC, r16
; release PC7:6
;ldi r16, (1<<XMM1)
;sts XMCRB, r16
; write 0xAA to address 0x0001 of external memory
;ldi r16, 0xaa
;sts 0x0001+4, r16
; re-enable PC7:6 for external memory
;ldi r16, (0<<XMM1)
;sts XMCRB, r16
; store 0x55 to address (OFFSET + 1) of external memory
;ldi r16, 0x55
;sts 0x0001+4, r16

;Ejemplo 7

;start:
;CLR R0
;COM R0
;LDI R26,0
;LDI R27,0
;LD R16,X

;sisas:
;LDI R26,0
;LDI R27,0
;LD R16,X+
;LD R17,X+
;MOV R18,R16
;ADD R18,R17
;MOV R19,R16
;SUB R19,R17
;MOV R20,R16
;AND R20,R17
;MOV R21,R16
;OR R21,R17
;MOV R23,R16
;COM R23
;ST X+,R18
;ST X+,R19
;ST X+,R20
;ST X+,R21
;ST X,R23

ldi r16, 0xFF
out DDRC, r16
ldi r16, 0x00
out PORTC, r16



	

	

	
	