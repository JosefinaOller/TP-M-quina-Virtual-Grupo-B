#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoria.h"
#include "funciones.h"

//    0x03000000; tipo operando B
//    0x0C000000; tipo operando A

//int aux = (0x0BFFEFAC & 0x0C000000) / 0x01000000;


void MOV(Memoria *memoria, Operandos op){
    if (op.operandoA[1] == 1) //De registro
        memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
    else if (op.operandoA[1] == 2) //Directo
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[0]] = op.operandoB[0];
}

void ADD(Memoria *memoria) {}

void SUB(Memoria *memoria) {}

void SWAP(Memoria *memoria, Operandos op) {
    int aux;

    if ((op.operandoA[1] != 0) && (op.operandoB[1] != 0)) //No es inmediato
    {
        if ((op.operandoA[1] == 1) && (op.operandoB[1] == 1)) //Ambos tipo de registro
        {
            aux = memoria->VectorDeRegistros[op.operandoA[0]];
            memoria->VectorDeRegistros[op.operandoA[0]] = memoria->VectorDeRegistros[op.operandoB[0]];
            memoria->VectorDeRegistros[op.operandoB[0]]= aux;
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 2)) //Ambos son de tipo Directo
        {
            aux = memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]] = memoria->RAM[op.operandoB[0] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoB[0] + memoria->VectorDeRegistros[0]] = aux;
        }
        else if ((op.operandoA[1] == 1) && (op.operandoB[1] == 2)) //A tipo de registro y B tipo directo
        {
            aux = memoria->VectorDeRegistros[op.operandoA[0]];
            memoria->VectorDeRegistros[op.operandoA[0]] = memoria->RAM[op.operandoB[0] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoB[0] + memoria->VectorDeRegistros[0]] = aux;
        }
        else //A tipo directo y B tipo de registro
        {
            aux = memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]] = memoria->VectorDeRegistros[op.operandoB[0]];
            memoria->VectorDeRegistros[op.operandoB[0]] = aux;
        }
    }
}

void MUL(Memoria *memoria) {}

void DIV(Memoria *memoria) {}

void CMP(Memoria *memoria) {}

void SHL(Memoria *memoria) {}

void SHR(Memoria *memoria) {}

void AND(Memoria *memoria, Operandos op) {
    int aux;

    aux = op.operandoA[0] & op.operandoB[0];

    if (op.operandoA[1] == 0) //A tipo inmediato
        memoria->VectorDeRegistros[op.operandoA[0]] = aux;
    else if (op.operandoA[1] == 1) //A tipo Directo
        memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]] = aux;

    if (aux == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(aux < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void OR(Memoria *memoria, Operandos op) {
    int aux;

    aux = op.operandoA[0] | op.operandoB[0];

    if (op.operandoA[1] == 0) //A tipo inmediato
        memoria->VectorDeRegistros[op.operandoA[0]] = aux;
    else if (op.operandoA[1] == 1) //A tipo Directo
        memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]] = aux;

    if (aux == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(aux < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void XOR(Memoria *memoria, Operandos op) {
    int aux;

    aux = op.operandoA[0] ^ op.operandoB[0];

    if (op.operandoA[1] == 0) //A tipo inmediato
        memoria->VectorDeRegistros[op.operandoA[0]] = aux;
    else if (op.operandoA[1] == 1) //A tipo Directo
        memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]] = aux;

    if (aux == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(aux < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void SYS(Memoria *memoria) {}

void JMP(Memoria *memoria) {}

void JZ(Memoria *memoria) {}

void JP(Memoria *memoria) {}

void JN(Memoria *memoria) {}

void JNZ(Memoria *memoria) {}

void JNP(Memoria *memoria) {}

void JNN(Memoria *memoria) {}

void LDL(Memoria *memoria, Operandos op) {
    memoria->VectorDeRegistros[9] = (op.operandoA[0] & 0x3) + (memoria->VectorDeRegistros[9] & 0xFFFFFFF3);
}

void LDH(Memoria *memoria, Operandos op) {
    memoria->VectorDeRegistros[9] = ((op.operandoA[0] & 0x3) << 30) + (memoria->VectorDeRegistros[9] & 0x3FFFFFFF);
}

void RND(Memoria *memoria) {}

void NOT(Memoria *memoria, Operandos op) {
    int aux;

    aux = ~op.operandoA[0];

    if (op.operandoA[1] == 0) //A tipo inmediato
        memoria->VectorDeRegistros[op.operandoA[0]] = aux;
    else if (op.operandoA[1] == 1) //A tipo Directo
        memoria->RAM[op.operandoA[0] + memoria->VectorDeRegistros[0]] = aux;

    if (aux == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(aux < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void STOP(Memoria *memoria) {}

//-------------------------------
/*
int verificoHeader(Header header){
    int ret=1;

    if ()

    return ret;
}*/

int decodificaCodigo(int instruccion)
{
    unsigned int cod = instruccion;
    //Se utiliza unsigned int para propagar 0's hacia la derecha
    if ( !((cod / 0x01000000) == 0xFF ))
    {
        if (!( (cod / 0x10000000) == 0xF ))
        {
            //Tiene 2 operandos
            cod /= 0x10000000;
        }
        else
            //Tiene 1 operando
            cod /= 0x01000000;
    }
    else
        //No tiene operandos
        cod /= 0x00100000;

    return cod;
}


void decodificaOperandos(Memoria memoria, int codigo, int instruccion, Operandos *op)
{
    if (codigo < 0xF)
    {
        //Tiene 2 operandos

        //Averiguo los tipos

        op->operandoA[1] = (instruccion & 0x0C000000) >> 26;
        op->operandoB[1] = (instruccion & 0x03000000) / 0x01000000;

        //--------------------------------------------------------------------------------------------
        //Operando B

        if (op->operandoB[1] == 0) //Inmediato
            op->operandoB[0] = instruccion & 0xFFF;

        else if ( op->operandoB[1] == 1 ){ //Registro
            op->operandoB[2] = (instruccion & 0x30) / 0x10;
            op->operandoB[3] = instruccion & 0xF;

            if (op->operandoB[2] == 0)
                //Registro de 4 bytes
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]];

            else if (op->operandoB[2] == 1)

                //4to byte del registro
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]] & 0xF;


            else if (op->operandoB[2] == 2)
                //3er byte del registro
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]] & 0xF0;

            else if (op->operandoB[2] == 3)
                //Registro de 2 bytes
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]] & 0xFF;
        }

        else if (op->operandoB[1] == 2) //Directo
            op->operandoB[0] = memoria.RAM[ (instruccion & 0xFFF) + memoria.VectorDeRegistros[0]];

        //--------------------------------------------------------------------------------------------
        //Operando A

        if (op->operandoA[1] == 0) //Tipo de operando A: inmediato
            op->operandoA[0] = (instruccion & 0xFFF000) / 0x1000;
        else if(op->operandoA[1] == 1) //Tipo de operando A: de registro
        {
            op->operandoA[3] = (instruccion & 0xF000) / 0x1000;
            op->operandoA[2] = (instruccion & 0x30000) / 0x10000;

            if (op->operandoA[2] == 0)
                //Registro de 4 bytes
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]];

            else if(op->operandoA[2] == 1)
                //4to byte del registro
                op->operandoA[0] = (memoria.VectorDeRegistros[op->operandoA[3]] & 0xF);

            else if (op->operandoA[2] == 2)
                //3er byte del registro
                op->operandoA[0] = (memoria.VectorDeRegistros[op->operandoA[3]] & 0xF0);

            else if (op->operandoA[2] == 3)
                //Registro de 2 bytes
                op->operandoA[0] = (memoria.VectorDeRegistros[op->operandoA[3]] & 0xFF);
        }
        else if (op->operandoA[1] == 2) // Tipo de operando A: directo
            op->operandoA[0] = memoria.RAM[((instruccion & 0xFFF000) / 0x1000)+ memoria.VectorDeRegistros[0]];
    }
    else if (codigo < 0xFF)
    {
        //Tiene 1 operando

        //Averiguo el tipo de operando

        op->operandoA[1] = (instruccion & 0xC00000) / 0x100000;

        //Analizo operando
        if (op->operandoA[1] == 0) //Inmediato
            op->operandoA[0] = instruccion & 0xFFFF;
        else if (op->operandoA[1] == 1) //De registro
        {
            op->operandoA[2] = (instruccion & 0x30) / 0x10;
            op->operandoA[3] = instruccion & 0xF;

            if (op->operandoA[2]  == 0)
                //Registro de 4 bytes
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]];
            else if (op->operandoA[2] == 1)
                //4to byte del registro
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xF;
            else if (op->operandoA[2] == 2)
                //3er byte del registro
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xF0;
            else if (op->operandoA[2]  == 3)
                //Registro de 2 bytes
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xFF;
        }
        else if (op->operandoA[2] == 2) //Directo
            op->operandoA[0] = memoria.RAM[(instruccion & 0xFFFF) + memoria.VectorDeRegistros[0]];

    }
}


