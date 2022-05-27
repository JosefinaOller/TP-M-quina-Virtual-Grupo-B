;DIRECCIONAMIENTO INDIRECTO
\\DATA 100 
\\EXTRA 0 
\\STACK 0
ant		EQU 	-1
sig		EQU		1
texto 		EQU "HOLA"
;inicializo los registros
		mov 	Eax, 1
		mov		Ebx, 5
        mov     Ecx, 2     
		mov Eex, texto   
;relleno la memoria de 1 a 10
otro:	cmp		Eax, 7
		jz		sigue
		mov		[Eax], Eax
		add		Eax, 1
		jmp 	otro
;recupero registros y multiplico x10
sigue:	mul 	[ECX+ant],10
		mul 	[ECx],10
		mul 	[Ecx+sig],10
		mul 	[Ebx-1],10
		mul 	[EBx],10
		mul 	[EBX+1],10
        mov     [EBX+2],[Ecx-sig]
;imprime todo 
		mov 	Eax, 1        
		mov		Edx, 1
		mov		Ecx, 7
		sys %2
		stop