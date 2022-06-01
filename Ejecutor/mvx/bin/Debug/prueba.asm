mov AH, %08 ; Obtener los parámetros del disco
mov DL, 0 ; Unidad de disco 0
sys %D ; Indica que se realiza la lectura