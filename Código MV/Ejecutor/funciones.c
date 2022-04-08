#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "memoria.h"
#include "funciones.h"

//    0x03000000; tipo operando B
//    0x0C000000; tipo operando A

    //int aux = (0x0BFFEFAC & 0x0C000000) / 0x01000000;


void MOV(Memoria *memoria){
}

void ADD(Memoria *memoria){}

void SUB(Memoria *memoria){}

void SWAP(Memoria *memoria){}

void MUL(Memoria *memoria){}

void DIV(Memoria *memoria){}

void CMP(Memoria *memoria){}

void SHL(Memoria *memoria){}

void SHR(Memoria *memoria){}

void AND(Memoria *memoria){}

void OR(Memoria *memoria){}

void XOR(Memoria *memoria){}

void SYS(Memoria *memoria){}

void JMP(Memoria *memoria){}

void JZ(Memoria *memoria){}

void JP(Memoria *memoria){}

void JN(Memoria *memoria){}

void JNZ(Memoria *memoria){}

void JNP(Memoria *memoria){}

void JNN(Memoria *memoria){}

void LDL(Memoria *memoria){}

void LDH(Memoria *memoria){}

void RND(Memoria *memoria){}

void NOT(Memoria *memoria){}

void STOP(Memoria *memoria){}

//-------------------------------

int decodificaCodigo(int instruccion){
    unsigned int cod = instruccion;
    //Se utiliza unsigned int para propagar 0's hacia la derecha

    if ( !((cod / 0x01000000) == 0xFF )){
        if (!( (cod / 0x10000000) == 0xF )){
            //Tiene 2 operandos
            cod /= 0x10000000;
        } else
            //Tiene 1 operando
            cod /= 0x01000000;
    } else
        //No tiene operandos
        cod /= 0x00100000;

    return cod;
}

/*
void decodificaOperandos(Memoria memoria, int codigo, int instruccion, int *tipoA, int *tipoRegA, int *tipoB, int *tipoRegB, int *opA, int *opB){
    *tipoA = instruccion & 0x0C000000 / 0x01000000;
    *tipoB = instruccion & 0x03000000 / 0x01000000;

    int regAux;

    if (*tipoB == 0)   //Tipo de operando: inmediato
        *opB = instruccion & 0xFFF;

    else if ( *tipoB == 1 ){   //Tipo de operando: de registro

        *tipoRegB = (instruccion & 0x30) / 0x10;
        regAux = instruccion & 0xF;
        if (tipoRegB == 0)
            *opB = memoria.VectorDeRegistros[regAux];
        else if (tipoRegB == 1)
            *opB = memoria.VectorDeRegistros[regAux] &

    }
}*/


