;1. Hacer una rutina recursiva que calcule la potencia de 2 números pasados como ;parámetro.

BASE EQU 2
EXP EQU 3

MAIN:	PUSH 4
	PUSH 2
	CALL POT		;Devuelve en AX la potencia: (primer)^segundo
	ADD SP, 2
	STOP

POT:	PUSH BP
	MOV BP, SP
	PUSH BX		; BX = exponente
	PUSH CX		; CX = base

	MOV BX, [BP+EXP]
	MOV CX, [BP+BASE]

	CMP BX, 0
	JZ CERO
	CMP BX, 1
	JP OTRO
	
	MOV AX, CX
	JMP FIN_POT

OTRO:	SUB BX, 1
	PUSH BX
	PUSH [BP+BASE]
	CALL POT
	ADD SP, 2

	MUL AX, CX
	JMP FIN_POT

CERO:	MOV AX, 1

FIN_POT: POP CX
	POP BX
	MOV SP, BP
	POP BP
	RET
