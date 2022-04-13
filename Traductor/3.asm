; Leo entradas. ej: 101,AA,FF,CAE

        mov     eax  ,   %008
;       mov     eax  ,   %001
        mov     ecx  ,   1
        mov     edx  ,   0
        sys     1 

; Inicializo         
        mov     eax  ,   32
        xor     [1] ,   [1]

; Calculos 

sigue:  mov     eex  ,   [0]
        shl     [0] ,   1
        shr     eex  ,   31
        and     eex  ,   1
        mul     [1] ,   10
        add     [1] ,   eex
        sub     eax  ,   1
        jnz     sigue

; Muestro resultados        

        mov     eax  ,   %001
        mov     ecx  ,   1
        mov     edx  ,   1
        sys     2
        stop
        mov     [1] ,   1 ; ojo esto no. 
        sys     2
        stop