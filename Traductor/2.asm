    mov eax,1
    mov [0],eax
    mov ebx,[0]
    sub eax,1
    mov [1],eax
    mov eex,[1]
    sub eax,1
    mov [2],eax
    mov eex,[2]
    mul eax,%ffff
    mov [3],eax
    mov efx,[3]
    ldh 65535
    ldl 65535
    mov [4],ac
    shr ac,32
    mov ecx, 1
    shl ecx, 31
    shr ecx, 31    
    mov [5], ecx
    sys %F 
    stop