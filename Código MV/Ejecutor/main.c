#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "memoria.h"
#include "funciones.h"

typedef void (*VectorFunciones[255])(Memoria*);

//instruccion  memoria->RAM[memoria->VectorRegistros[4]]
//operando 1   memoria->RAM[memoria->VectorRegistros[4]+1]
//operando 2   memoria->RAM[memoria->VectorRegistros[4]+2]

int main(int argc, char *argv[])
{
    Memoria memoria;
    VectorFunciones vecF;
    FILE *arch=fopen(argv[1],"rb");

    srand (getpid());   //Cambia la semilla en cada ejecución del programa para la instrucción random

    vecF[0x00]=&MOV;
    vecF[0x01]=&ADD;
    vecF[0x02]=&SUB;
    vecF[0x03]=&SWAP;
    vecF[0x04]=&MUL;
    vecF[0x05]=&DIV;
    vecF[0x06]=&CMP;
    vecF[0x07]=&SHL;
    vecF[0x08]=&SHR;
    vecF[0x09]=&AND;
    vecF[0x0A]=&OR;
    vecF[0x0B]=&XOR;

    vecF[0xF0]=&SYS;
    vecF[0xF1]=&JMP;
    vecF[0xF2]=&JZ;
    vecF[0xF3]=&JP;
    vecF[0xF4]=&JN;
    vecF[0xF5]=&JNZ;
    vecF[0xF6]=&JNP;
    vecF[0xF7]=&JNN;
    vecF[0xF8]=&LDL;
    vecF[0xF9]=&LDH;
    vecF[0xFA]=&RND;
    vecF[0xFB]=&NOT;

    vecF[0xFF1]=&STOP;

    fread(&memoria,sizeof(Memoria),1,arch);
    fclose(arch);

    system("cls");

    return 0;
}

void MOV(Memoria *memoria){}

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


