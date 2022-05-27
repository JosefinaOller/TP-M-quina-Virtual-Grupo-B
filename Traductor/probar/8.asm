\\DATA 10 
\\EXTRA -1
\\STACK 0
dmin    EQU 0
dmax    EQU 1
line    EQU "-----------------------------------------------------------------------"
mmin    EQU "El minimo es: "
mmax    EQU "El maximo es: "
        SYS %F
        MOV     EAX, %100
        MOV     EBX, 1
        MOV     ECX, -1
        MOV     EDX, line
        SYS     %4 
        MOV     EAX, %110
        MOV     EBX, 1
        MOV     ECX, -1
        MOV     EDX, mmin
        SYS     %4
        MOV     [dmin],[EBX+dmin]
        MOV     [EBX+dmin],0
        MOV     EAX, %101
        MOV     EBX, 2
        MOV     ECX, 1
        MOV     EDX, dmin
        SYS     %2
        MOV     EAX, %100
        MOV     EBX, 1
        MOV     ECX, -1
        MOV     EDX, line
        SYS     %4 
        MOV     EAX, %110
        MOV     EBX, 1
        MOV     ECX, -1
        MOV     EDX, mmax
        SYS     %4
        MOV     [dmax],[EBX+dmax]
        MOV     [EBX+dmax],0
        MOV     EAX, %101
        MOV     EBX, 2
        MOV     ECX, 1
        MOV     EDX, dmax
        SYS     %2
        MOV     EAX, %100
        MOV     EBX, 1
        MOV     ECX, -1
        MOV     EDX, line
        SYS     %4 
        STOP
        
        