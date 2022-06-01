\\DATA 2000
\\EXTRA 2000
\\STACK 2000

ldh 2 ; Código del ES
ldl 0 ; offset del ES
mov EBX, AC ; Dirección del buffer de lectura
mov AH, %03 ; Escribir del disco
mov AL, 3 ; 3 sectores a leer
mov CH, 0 ; Cilindro 0
mov CL, 0 ; Cabeza 0
mov DH, 1 ; Sector 1
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la escritura
STOP