    xor     [%FF], [%FF]
    add     [%FF], %F 
    mov     eax, %8
    mov     ecx, 1
    mov     edx, %FF
    mov     efx, 31
CMP1:   cmp     efx, 0 
        jn      FIN 
        mov     eex, efx
        sub     efx, 1
        div     eex, 4
        cmp     ac, 0 
        jz      line
        or    eax, %100
SYS2:    sys     2
    shl     [%FF], 1
    jmp     CMP1
line: and    eax, %EFF
    jmp     SYS2
FIN: stop    

