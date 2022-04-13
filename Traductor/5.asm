; Bueno, ya hemos terminado o casi...
        mov     [0], %62
        mov     [1], @165
        mov     [2], 101
        mov     [3], #110
        mov     [4], 'o'
        mov     eex, ' '
        not     eex
CMP1:   cmp     0,0
        jnz     CMP2
COM1:   and     [0], eex
CMP2:   cmp     15,%10
        jnn     COM3
        jn      COM2
        jmp     COM4
COM2:   and     [1], eex
CMP3:   cmp     %20,' 
        jz      COM3
        jmp     CMP3
COM3:   and     [2], eex
CMP4:   cmp     '(',')'
        jp     COM5
COM4:   and     [3], eex
CMP5:   cmp     1,2
        jnp     COM5
        jmp     CMP5
COM5:   and     [4], eex
        mov     EAX, %110
        mov     ECX, 5
        mov     EDX, 0
        sys     2
    