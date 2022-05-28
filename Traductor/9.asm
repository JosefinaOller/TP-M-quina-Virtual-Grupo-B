\\DATA 5000
;Inicializa HEAP
        mov     [EBX], 1
;Escribe mensaje al usuario
msg     equ     "Escriba palabras seguidas de ENTER (en blanco para terminar)"
        mov     Eax, %100
        mov     Ebx, 1
        mov     Ecx, -1  
        mov     Edx, msg
        sys     %4 
        mov     Ecx, 1 
;Lee una palabra en DS
ini:    mov     Eax, %110
        mov     Ebx, 2
        mov     Ecx, -1
        mov     Edx, 0
        sys     %3
        slen    Ecx, [EAX]
        cmp     Ecx, 0           ;Si está vacía... 
        jz      finlee          ;...termina la lectura 
        mov     Edx, [EBX]      ;Sino, muevo el HEAP a DX        
        add     [BX], cx      ;Incremento el HEAP para reservar la memoria 
        add     [BX], 1       ;Uno más por el \0
        jmp     ini
finlee: mov dx, 1               ;Inicializa 1 para comenzar el recorrido 
        mov cx, [EBX]          
        sub cx, 1               ;Posiciona a cx en el último \0
next:   cmp dx, cx              ;Termina cuando DX llega al último \0
        jz  fin
may:    and [EDX], %DF        ;Pasa a MAYUSCULAS
        cmp [EDX], 0          ;Si encuentra un \0 ...
        jnz sig         
        mov [EDX], ' '        ;... pone un espacio para contactenar 
sig:    add dx, 1               
        jmp next
; Muestra cadena concatenada y pasada a mayusculas
fin:    SYS %F
        mov     Eax, %100
        mov     Ebx, 3
        mov     Ecx, -1  
        mov     Edx, 1
        sys     %4 
        stop 


        



