.ORG 0x00
JMP START
.ORG 0x2E
JMP OVERFLOW

START:
LDI R16,(1<<DDB0)
OUT DDRB,R16

LDI R16,(1<<CS12)|(1<<CS10)
STS TCCR1B,R16 ;PRESCALER 1024
LDI R16,(1<<TOIE1)
STS TIMSK1,R16 ;TIMER INTERRUPT ENABLE
SEI ;I GLOBAL ENABLE


LDI R16,0B00000001
OUT PORTB,R16

LDI R17,0x39
STS TCNT1L,R17
LDI R17,0xD2
STS TCNT1H,R17

ESPERAR:
JMP ESPERAR

OVERFLOW:
COM R16
OUT PORTB,R16
LDI R17,0x39
STS TCNT1L,R17
LDI R17,0xD2
STS TCNT1H,R17
RETI