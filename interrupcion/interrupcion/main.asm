.ORG 0x00
RJMP START
.ORG 0x02
RJMP INTER

START:
LDI R16,0x03 
STS EICRA,R16
SBI EIMSK,0
SEI 
LDI R17,(1<<DDB0)
OUT DDRB,R17 

PRENDER:
LDI R17,(0<<PORTB0)
OUT PORTB,R17
JMP PRENDER


INTER:
COM R17
OUT PORTB,R17
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
CALL ESPERAR
RETI



ESPERAR:
LDI R19,0x00
LDI R26,0
LDI R27,0
LD R20,X+
LD R21,X+
LDI R16,3
CPI R16,3
BREQ SUMA
SUMA:
ADD R16,R21
ST X,R16
RET