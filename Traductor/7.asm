\\DATA 0 
\\EXTRA -1
\\STACK 0
MIN     EQU %80000000
MAX     EQU %7FFFFFFF
UNO     EQU 1
dmin    EQU 0
dmax    EQU 1
start   EQU 2
        mov EDx, start
        mov ECx, [Dx]
        add ECx, Dx
        mov [EBX+dmin], MAX
        mov [EBX+dmax], MIN
INI:    add EDx, UNO
        cmp EDx, ECx
        jp  fin
        cmp [EBX+dmin], [EDx]
        jn  sig
        mov [EBX+dmin], [EDx]
SIG:    cmp [EBX+dmax], [EDx]
        jp  ini
        mov [EBX+dmax], [EDx]
        jmp ini
FIN:    Stop 