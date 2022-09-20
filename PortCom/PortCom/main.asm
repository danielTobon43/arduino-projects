

INICIO: 				;Punto de entrada al programa
	call borra       	        ;borra la pantalla
	call msjuno			;envio de mensaje					
	call inicializacom

leetec:	call leeteclado		;lee caracter de teclado con ah= 01 y int 21, la tecla leida queda en al
	mov cl, 30		;compara si se pulso un cero, con el 0 sale
	cmp cl, al
	jz  term
	call outcom
	jmp leetec
term:	call msjlinea		;salta de linea	
	call msjdos
	call vueltados


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;procesos;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


inicializacom proc near
	MOV AL,0F3h 		;(velocidad = 9600, 8 bits de datos, 1 bit de paro, sin paridad)(esto depende como quieras configurar el puerto)
	MOV DX,0000h		; determina que se usa el puerto COM1
	MOV AH,00h		; inicializa puerto serial
	INT 14h
	ret
inicializacom endp

outcom  proc near
	MOV DX,0000h		;envia caracter leido al puerto COM1
	MOV AH,01h		;funcion 01h-enviando un caracter al COM con int 14
	INT 14h 
	ret			;saca x el com el valor de al
outcom  endp


leeteclado proc near
	MOV AH,01h
	INT 21h			;la tecla leida queda en al
	ret
leeteclado endp


vueltados proc near
	mov ah,4ch 		; vuelta al DOS
	mov al,1
	int 21h
	ret
vueltados endp

borra  proc near		;borra la pantalla
	Mov AH, 0Fh             ;Obtiene modo de video actual
   	Int 10h
   	Mov AH, 00              ;Cambia el modo de video al mismo anterior
   	Int 10h 
	ret
borra endp

msjuno  proc near				; escribe msj 1
	MOV AX,DATA 				; Pone direccion en AX
	MOV DS,AX 				; Pone la direccion en los registros
	MOV DX,OFFSET msjini			; Obtiene direccion del mensaje
	MOV AH,09H 				; Funcion: Visualizar cadena
	INT 21H 
	ret
msjuno endp

msjdos	proc near				; escribe msj 2
	MOV AX,DATA 				; Pone direccion en AX
	MOV DS,AX 				; Pone la direccion en los registros
	MOV DX,OFFSET msjfin			; Obtiene direccion del mensaje
	MOV AH,09H 				; Funcion: Visualizar cadena
	INT 21H 
	ret
msjdos  endp

msjlinea proc near				; deja una linea en blanco
	MOV AX,DATA 				; Pone direccion en AX
	MOV DS,AX 				; Pone la direccion en los registros
	MOV DX,OFFSET linea			; Obtiene direccion del mensaje
	MOV AH,09H 				; Funcion: Visualizar cadena
	INT 21H 
	ret
msjlinea endp

print	proc near				;en dx se guarda previamente el offset del msj
	MOV AX,DATA 				; Pone direccion en AX
	MOV DS,AX 				; Pone la direccion en los registros
	MOV AH,09H 				; Funcion: Visualizar cadena
	INT 21H 
	ret
print endp


	CODE ENDS
END INICIO 
