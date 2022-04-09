#include <string.h>
#include "prototipos.h"

void cargaMnemonico(char etiq[],int codigo,int cantOp, Mnemonico *x){
    strcpy((*x).etiqueta,etiq);
    (*x).cantOp=cantOp;
    (*x).codigo=codigo;
}

void cargaLinea(char lab[],char mnem[],char argA[], char ArgB[], char come[], Linea *linea){
    strcpy((*linea).label,lab);
    strcpy((*linea).comentario,come);
    strcpy((*linea).argA,argA);
    strcpy((*linea).argB,ArgB);
    strcpy((*linea).mnem,mnem);
}

