\\DATA 200
ldh 2 ; Código del ES
ldl 64 ; offset del ES
mov EBX, AC ; Dirección del buffer de lectura
mov AH, %02 ; Leer del disco
mov AL, 1 ; 3 sectores a leer
mov CH, 0 ; Cilindro 2
mov CL, 0 ; Cabeza 8
mov DH, 0 ; Sector 10
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la lectura
stop
