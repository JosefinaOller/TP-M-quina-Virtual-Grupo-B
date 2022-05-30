#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memoria.h"
#include "funciones.h"



void MOV(Memoria *memoria, OperandosYFlags op)
{
    int aux;

    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = op.operandoB[0] & 0xFF;
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = op.operandoB[0] & 0xFF;
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + (aux * 0x100);
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = op.operandoB[0] & 0xFFFF;
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        memoria->RAM[op.segmento.ds + op.operandoA[4]] = op.operandoB[0];
    }
    else if (op.operandoA[1] == 3)
    {
        if (op.segmento.actualA == 0)  //DS
        {
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = op.operandoB[0];
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            memoria->RAM[op.segmento.es + op.operandoA[4]] = op.operandoB[0];
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = op.operandoB[0];
        }
    }

}

void ADD(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] + op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = (op.operandoA[0] + op.operandoB[0]) & 0xFF;
            cc = aux;
            if ( (0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] + (op.operandoB[0] * 0x100) ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] + op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] + op.operandoB[0];
        cc = aux;
        memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  memoria->RAM[op.segmento.ds + op.operandoA[4]] + op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  memoria->RAM[op.segmento.es + op.operandoA[4]] + op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux =  memoria->RAM[op.segmento.ss + op.operandoA[4]] + op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if ( cc<0 )
        memoria->VectorDeRegistros[8] = 0x80000000;
    else if ( cc==0 )
        memoria->VectorDeRegistros[8] = 0x00000001;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void SUB(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] - op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = (op.operandoA[0] - op.operandoB[0]) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] - (op.operandoB[0] * 0x100) ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] - op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] - op.operandoB[0];
        cc = aux;
        memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  memoria->RAM[op.segmento.ds + op.operandoA[4]] - op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  memoria->RAM[op.segmento.es + op.operandoA[4]] - op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux =  memoria->RAM[op.segmento.ss + op.operandoA[4]] - op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if ( cc<0 )
        memoria->VectorDeRegistros[8] = 0x80000000;
    else if ( cc==0 )
        memoria->VectorDeRegistros[8] = 0x00000001;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void SWAP(Memoria *memoria, OperandosYFlags op)
{
    int aux;

    if ((op.operandoA[1] != 0) && (op.operandoB[1] != 0)) //No hay operandos inmediatos
    {
        if ((op.operandoA[1] == 1) && (op.operandoB[1] == 1)) //Ambos tipo de registro
        {
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];

                if (op.operandoB[2] == 0) //4 bytes
                    memoria->VectorDeRegistros[op.operandoB[3]] = op.operandoA[0];
                else if (op.operandoB[2] == 1) //4to byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFF;
                }
                else if (op.operandoB[2] == 2) //3er byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= ((op.operandoA[0] * 0x100) & 0xFF00);
                }
                else if (op.operandoB[2] == 3) //Ultimos 2 bytes
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFFFF;
                }
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = op.operandoA[0] & 0xFF;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + (op.operandoB[0] & 0xFF);

                if (op.operandoB[2] == 0) //4 bytes
                    memoria->VectorDeRegistros[op.operandoB[3]] = aux;
                else if (op.operandoB[2] == 1) //4to byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= aux;
                }
                else if (op.operandoB[2] == 2) //3er byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= ((aux * 0x100) & 0xFF00);
                }
                else if (op.operandoB[2] == 3) //Ultimos 2 bytes
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFFFF;
                }
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = op.operandoA[0] & 0xFF00;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + ((op.operandoB[0] * 0x100 ) & 0xFF00);

                if (op.operandoB[2] == 0) //4 bytes
                    memoria->VectorDeRegistros[op.operandoB[3]] = aux / 0x100;
                else if (op.operandoB[2] == 1) //4to byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= (aux / 0x100);
                }
                else if (op.operandoB[2] == 2) //3er byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= (aux & 0xFF00);
                }
                else if (op.operandoB[2] == 3) //Ultimos 2 bytes
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= (aux / 0x100) & 0xFFFF;
                }
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A
                aux = op.operandoA[0] & 0xFFFF;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + (op.operandoB[0] & 0xFFFF);

                if (op.operandoB[2] == 0) //4 bytes
                    memoria->VectorDeRegistros[op.operandoB[3]] = aux;
                else if (op.operandoB[2] == 1) //4to byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= (aux & 0xFF);
                }
                else if (op.operandoB[2] == 2) //3er byte
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= ((aux&0xFF) * 0x100);
                }
                else if (op.operandoB[2] == 3) //Ultimos 2 bytes
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= aux;
                }
            }
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 2)) //Ambos son de tipo Directo
        {
            memoria->RAM[op.operandoA[4] + op.segmento.ds] = op.operandoB[0];
            memoria->RAM[op.operandoB[4] + op.segmento.ds] = op.operandoA[0];
        }
        else if ((op.operandoA[1] == 3) && (op.operandoB[1] == 3)) //Ambos son de tipo indirecto
        {
            memoria->RAM[op.operandoA[4] + (memoria->VectorDeRegistros[op.segmento.actualA] & 0xFFFF)] = op.operandoB[0];
            memoria->RAM[op.operandoB[4] + (memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF)] = op.operandoA[0];
        }
        else if ((op.operandoA[1] == 1) && (op.operandoB[1] == 2)) //A tipo de registro y B tipo directo
        {
            //Si el operando A es de registro
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
                memoria->RAM[op.operandoB[4] + op.segmento.ds] = op.operandoA[0];
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoA[3]]+= op.operandoB[0] & 0xFF;
                memoria->RAM[op.operandoB[4] + op.segmento.ds] = op.operandoA[0] & 0xFF;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoA[3]] += (op.operandoB[0] & 0xFF) * 0x100;
                memoria->RAM[op.operandoB[4] + op.segmento.ds] = ( op.operandoA[0] & 0xFF00) / 0x100;
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoA[3]] += op.operandoB[0] & 0xFFFF;
                memoria->RAM[op.operandoB[4] + op.segmento.ds] = op.operandoA[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 1) && (op.operandoB[1] == 3)) //A tipo registro y B indirecto
        {
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
                memoria->RAM[op.operandoB[4] + (memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF)] = op.operandoA[0];
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoA[3]]+= op.operandoB[0] & 0xFF;
                memoria->RAM[op.operandoB[4] + (memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF)] = op.operandoA[0] & 0xFF;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoA[3]] += (op.operandoB[0] & 0xFF) * 0x100;
                memoria->RAM[op.operandoB[4] + (memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF)] = ( op.operandoA[0] & 0xFF00) / 0x100;
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoA[3]] += op.operandoB[0] & 0xFFFF;
                memoria->RAM[op.operandoB[4] + (memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF)] = op.operandoA[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 1))//A tipo directo y B tipo de registro
        {
            memoria->RAM[op.operandoA[4] + op.segmento.ds] = op.operandoB[0];

            if (op.operandoB[2] == 0) //4 bytes
                memoria->VectorDeRegistros[op.operandoB[3]] = op.operandoA[0];
            else if (op.operandoB[2] == 1) //4to byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFF;
            }
            else if (op.operandoB[2] == 2) //3er byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoB[3]]+= ((op.operandoA[0] * 0x100) & 0xFF00);
            }
            else if (op.operandoB[2] == 3) //Ultimos 2 bytes
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 3) && (op.operandoB[1] == 1))//A tipo indirecto y B tipo de registro
        {
            memoria->RAM[op.operandoA[4] + (memoria->VectorDeRegistros[op.segmento.actualA] & 0xFFFF)] = op.operandoB[0];

            if (op.operandoB[2] == 0) //4 bytes
                memoria->VectorDeRegistros[op.operandoB[3]] = op.operandoA[0];
            else if (op.operandoB[2] == 1) //4to byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFF;
            }
            else if (op.operandoB[2] == 2) //3er byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoB[3]]+= ((op.operandoA[0] * 0x100) & 0xFF00);
            }
            else if (op.operandoB[2] == 3) //Ultimos 2 bytes
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 3))//A tipo directo y B tipo indirecto
        {
            memoria->RAM[op.operandoA[4] + op.segmento.ds] = op.operandoB[0];
            memoria->RAM[op.operandoB[4] + (memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF)] = op.operandoA[0];
        }
        else if ((op.operandoA[1] == 3) && (op.operandoB[1] == 2))//A tipo indirecto y B tipo directo
        {
            memoria->RAM[op.operandoA[4] + (memoria->VectorDeRegistros[op.segmento.actualA] & 0xFFFF)] = op.operandoB[0];
            memoria->RAM[op.operandoB[4] + op.segmento.ds] = op.operandoA[0];
        }
    }
}

void MUL(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] * op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = ( op.operandoA[0] * op.operandoB[0] ) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] * op.operandoB[0] ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] * op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }

    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] * op.operandoB[0];
        cc = aux;
        memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  memoria->RAM[op.segmento.ds + op.operandoA[4]] * op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  memoria->RAM[op.segmento.es + op.operandoA[4]] * op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux =  memoria->RAM[op.segmento.ss + op.operandoA[4]] * op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if ( cc<0 )
        memoria->VectorDeRegistros[8] = 0x80000000;
    else if ( cc==0 )
        memoria->VectorDeRegistros[8] = 0x00000001;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void DIV(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    int valorInicial;
    int cc;
    //Si el operando B es 0, no se realiza el DIV y se omite la instrucción
    if (op.operandoB[0] != 0)
    {
        if (op.operandoA[1] == 1)
        {
            //Si el operando A es de registro
            valorInicial = memoria->VectorDeRegistros[op.operandoA[3]];
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                aux = op.operandoA[0] / op.operandoB[0];
                cc = aux;
                memoria->VectorDeRegistros[op.operandoA[3]] = aux;
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = ( op.operandoA[0] / op.operandoB[0] ) & 0xFF;
                cc = aux;
                if ((0x80 & aux) != 0)
                {
                    cc = aux << 24;
                    cc>>=24;
                }
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFF00) + aux;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = ( op.operandoA[0] / op.operandoB[0] ) & 0xFF00;
                cc = aux;
                if ((0x8000 & aux) != 0)
                {
                    cc = aux << 16;
                    cc>>=16;
                }
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A
                aux = ( op.operandoA[0] / op.operandoB[0] ) & 0xFFFF;
                cc = aux;
                if ((0x8000 & aux) != 0)
                {
                    cc = aux << 16;
                    cc>>=16;
                }
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
            }
            memoria->VectorDeRegistros[9] = valorInicial % op.operandoB[0];
        }
        else if (op.operandoA[1] == 2)
        {
            //Si el operando A es directo

            valorInicial = memoria->RAM[op.segmento.ds + op.operandoA[4]];
            aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] / op.operandoB[0];
            cc = aux;
            memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;
            memoria->VectorDeRegistros[9] = valorInicial % op.operandoB[0];
        }
        else if (op.operandoA[1] == 3)
        {
            //Indirecto
            if (op.segmento.actualA == 0)  //DS
            {
                valorInicial = memoria->RAM[op.segmento.ds + op.operandoA[4]];

                aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] / op.operandoB[0];
                cc = aux;
                memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
            }
            else if (op.segmento.actualA == 2)    //ES
            {
                valorInicial = memoria->RAM[op.segmento.es + op.operandoA[4]];

                aux = memoria->RAM[op.segmento.es + op.operandoA[4]] / op.operandoB[0];
                cc = aux;
                memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
            }
            else if (op.segmento.actualA == 1)     //SS
            {
                valorInicial = memoria->RAM[op.segmento.es + op.operandoA[4]];

                aux = memoria->RAM[op.segmento.es + op.operandoA[4]] / op.operandoB[0];
                cc = aux;
                memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
            }

            memoria->VectorDeRegistros[9] = valorInicial % op.operandoB[0];
        }

        if ( cc<0 )
            memoria->VectorDeRegistros[8] = 0x80000000;
        else if ( cc==0 )
            memoria->VectorDeRegistros[8] = 0x00000001;
        else
            memoria->VectorDeRegistros[8] = 0;
    }

}

void CMP(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] - op.operandoB[0];
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = ( op.operandoA[0] - op.operandoB[0] ) & 0xFF;
            aux<<=24;
            aux>>=24;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] - (op.operandoB[0] * 0x100) ) & 0xFF00;
            aux<<=16;
            aux>>=16;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] - op.operandoB[0] ) & 0xFFFF;
            aux<<=16;
            aux>>=16;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] - op.operandoB[0];
    }
    else if (op.operandoA[1] == 0)
    {
        //Si el operando A es inmediato
        aux = op.operandoA[0] - op.operandoB[0];
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] - op.operandoB[0];
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux = memoria->RAM[op.segmento.es + op.operandoA[4]] - op.operandoB[0];
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux = memoria->RAM[op.segmento.ss + op.operandoA[4]] - op.operandoB[0];
        }
    }

    if (aux == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if (aux < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void SHL(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] << op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = ( op.operandoA[0]  << op.operandoB[0]) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] << op.operandoB[0] ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] << op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }

    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] << op.operandoB[0];
        cc = aux;
        memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  memoria->RAM[op.segmento.ds + op.operandoA[4]] << op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  memoria->RAM[op.segmento.es + op.operandoA[4]] << op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux =  memoria->RAM[op.segmento.ss + op.operandoA[4]] << op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }


    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if (cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

void SHR(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro

        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] >> op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = (op.operandoA[0] >> op.operandoB[0]) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] >> op.operandoB[0] ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            aux = ( op.operandoA[0] >> op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }

    }
    else if (op.operandoA[1] == 2)
    {
        //Si el operando A es directo
        aux = memoria->RAM[op.segmento.ds + op.operandoA[4]] >> op.operandoB[0];
        cc = aux;
        memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  memoria->RAM[op.segmento.ds + op.operandoA[4]] >> op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  memoria->RAM[op.segmento.es + op.operandoA[4]] >> op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux =  memoria->RAM[op.segmento.ss + op.operandoA[4]] >> op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if (cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

void AND(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] & op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = (op.operandoA[0] & op.operandoB[0]) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] & (op.operandoB[0] * 0x100) ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] & op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Directo
        aux = op.operandoA[0] & op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4] + op.segmento.ds] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  op.operandoA[0] & op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  op.operandoA[0] & op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux = op.operandoA[0] & op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

void OR(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] | op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = (op.operandoA[0] | op.operandoB[0]) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] | (op.operandoB[0] * 0x100) ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] | op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Directo
        aux = op.operandoA[0] | op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4] + op.segmento.ds] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  op.operandoA[0] | op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  op.operandoA[0] | op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux = op.operandoA[0] | op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

void XOR(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    int cc;
    if (op.operandoA[1] == 1)
    {
        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            aux = op.operandoA[0] ^ op.operandoB[0];
            cc = aux;
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            aux = (op.operandoA[0] ^ op.operandoB[0]) & 0xFF;
            cc = aux;
            if ((0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            aux = ( op.operandoA[0] ^ (op.operandoB[0] * 0x100) ) & 0xFF00;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + aux;
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            aux = ( op.operandoA[0] ^ op.operandoB[0] ) & 0xFFFF;
            cc = aux;
            if ((0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Directo
        aux = op.operandoA[0] ^ op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4] + op.segmento.ds] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            aux =  op.operandoA[0] ^ op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            aux =  op.operandoA[0] ^ op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            aux = op.operandoA[0] ^ op.operandoB[0];
            cc = aux;
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

void SLEN(Memoria *memoria, OperandosYFlags op)
{
    int aux, length=0;

    if (!(op.operandoA[1]==0 || op.operandoB[1]==0 || op.operandoB[1]==1))
    {
        //Primero se calcula la longitud del string
        aux = memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF) + op.operandoB[4]];
        while (aux!=0)
        {
            length++;
            op.operandoB[4]++;
            aux = memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF) + op.operandoB[4]];
        }

        //Luego se almacena la longitud

        if (op.operandoA[1] == 1)
        {
            //Si el operando A es de registro
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = length;
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = length & 0xFF;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = length & 0xFF;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + (aux * 0x100);
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A
                aux = length & 0xFFFF;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
            }
        }
        else if (op.operandoA[1] == 2)
        {
            //Si el operando A es directo
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = length;
        }
        else if (op.operandoA[1] == 3)
        {
            //Si A es indirecto
            if (op.segmento.actualA == 0)  //DS
            {
                memoria->RAM[op.segmento.ds + op.operandoA[4]] = length;
            }
            else if (op.segmento.actualA == 2)    //ES
            {
                memoria->RAM[op.segmento.es + op.operandoA[4]] = length;
            }
            else if (op.segmento.actualA == 1)     //SS
            {
                memoria->RAM[op.segmento.ss + op.operandoA[4]] = length;
            }
        }
    }
}

void SMOV(Memoria *memoria, OperandosYFlags op)
{
    int aux;

    if (!(op.operandoA[1]==0 || op.operandoA[1]==1 || op.operandoB[1]==0 || op.operandoB[1]==1))
    {
        aux = memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF) + op.operandoB[4]];
        while (aux!=0)
        {
            memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualA] & 0xFFFF) + op.operandoA[4]] = aux;
            op.operandoB[4]++;
            op.operandoA[4]++;
            aux = memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF) + op.operandoB[4]];
        }
        memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualA] & 0xFFFF) + op.operandoA[4]] = 0;
    }

}

void SCMP(Memoria *memoria, OperandosYFlags op)
{
    int cc=0,aux;

    if (!(op.operandoA[1]==0 || op.operandoA[1]==1 || op.operandoB[1]==0 || op.operandoB[1]==1))
    {
        while (cc==0)
        {
            aux = memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualA] & 0xFFFF) + op.operandoA[4]]
            - memoria->RAM[(memoria->VectorDeRegistros[op.segmento.actualB] & 0xFFFF) + op.operandoB[4]];

            if (aux<0)
                cc = -1;
            else if (aux>0)
                cc = 1;
            op.operandoA[4]++;
            op.operandoB[4]++;
        }

        if (cc == 0)
            memoria->VectorDeRegistros[8] = 0x1;
        else if(cc < 0)
            memoria->VectorDeRegistros[8] = 0x80000000;
        else
            memoria->VectorDeRegistros[8] = 0;
    }

}

void SYS(Memoria *memoria, OperandosYFlags op)
{
    void (*FuncionesSYS[7])(Memoria *,OperandosYFlags);

    FuncionesSYS[0]=&SYS1;
    FuncionesSYS[1]=&SYS2;
    FuncionesSYS[2]=&SYSF;
    FuncionesSYS[3]=&SYS3;
    FuncionesSYS[4]=&SYS4;
    FuncionesSYS[5]=&SYS7;
    FuncionesSYS[6]=&SYSD;

    switch (op.operandoA[0])
    {
    case 0x1:
        FuncionesSYS[0](memoria,op);
        break;
    case 0x2:
        FuncionesSYS[1](memoria,op);
        break;
    case 0x3:
        FuncionesSYS[3](memoria,op);
        break;
    case 0x4:
        FuncionesSYS[4](memoria,op);
        break;
    case 0x7:
        FuncionesSYS[5](memoria,op);
        break;
    case 0xD:
        FuncionesSYS[6](memoria,op);
        break;
    case 0xF:
        FuncionesSYS[2](memoria,op);
        break;
    }
}

void SYS1(Memoria *memoria, OperandosYFlags op)
{
    int edx = memoria->VectorDeRegistros[13] & 0xFFFF;
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;
    int prompt = ( ax & 0x0800 ) / 0x100;
    int inputMode = ( ax & 0x0100 ) / 0x100;
    int tipoDato;
    char num[16];
    int seg, finSeg;
    int i,j;
    int aux;
    int tope;

    aux = memoria->VectorDeRegistros[13] / 0x10000;
    if (aux == 0)
    {
        seg = op.segmento.ds;
        finSeg = op.segmento.es;
    }
    else if (aux == 1)
    {
        seg = op.segmento.ss;
        finSeg = op.segmento.finSS;
    }
    else if (aux == 2)
    {
        seg = op.segmento.es;
        finSeg = op.segmento.ss;
    }
    else if (aux == 3)
    {
        seg = op.segmento.cs;
        finSeg = op.segmento.ds;
    }

    i = edx + seg;
    tope = i+cx;


    if (inputMode == 1)
    {

        j = 0;
        char sAux[255];

        if (prompt == 0)
            printf("[%04d]: ", i);

        fgets(sAux,cx+1,stdin);
        while ((i<finSeg) && (j<strlen(sAux)) && (i <= tope))
        {
            memoria->RAM[i] = sAux[j];
            i++;
            j++;
        }
        if (i>=finSeg)
        {
            memoria->error=1;
            printf("Segmentation Fault");
        }
    }
    else
    {
        tipoDato = (ax & 0xF);

        while ((i<finSeg) && (i < tope))
        {
            if (prompt == 0)
                printf("[%04d]: ", i);
            scanf("%s",num);
            if (tipoDato == 0x1)
            {
                QuitaCaracter(num,'\'',strlen(num));
                QuitaCaracter(num,'#',strlen(num));
                memoria->RAM[i] = atoi(num);
            }
            else if (tipoDato == 0x4)
            {
                QuitaCaracter(num,'@',strlen(num));
                sscanf(num,"%o",&(memoria->RAM[i]));
            }
            else if (tipoDato == 0x8)
            {
                QuitaCaracter(num,'%',strlen(num));
                sscanf(num,"%X",&(memoria->RAM[i]));
            }
            i++;
        }
    }
}

void SYS2(Memoria *memoria, OperandosYFlags op)
{
    int edx = memoria->VectorDeRegistros[13] & 0xFFFF;
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;
    int prompt = ( ax & 0x0800 ) / 0x100;
    int endline = ( ax & 0x0100 ) / 0x100;
    int tipoDato = ax & 0xFF;
    int i;
    int aux;
    int seg;
    int tope;

    aux = memoria->VectorDeRegistros[13] / 0x10000;
    if (aux == 0)
        seg = op.segmento.ds;
    else if (aux == 1)
        seg = op.segmento.ss;
    else if (aux == 2)
        seg = op.segmento.es;
    else if (aux == 3)
        seg = op.segmento.cs;

    i = edx + seg;
    tope = i+cx;

    while (i<tope)
    {
        if (prompt == 0)
            printf("[%04d]: ",i);
        if ( (tipoDato & 0x10) == 0x10)
        {
            unsigned char caracter = memoria->RAM[i] & 0xFF;
            if (caracter<32 || caracter>126)
                printf("%c ",'.');
            else
                printf("%c ",caracter);
        }
        if ( (tipoDato & 0x8) == 0x8 )
            printf("%c%X ",'%',memoria->RAM[i]);
        if ( (tipoDato & 0x4) == 0x4 )
            printf("%c%o ",'@',memoria->RAM[i]);
        if ( (tipoDato & 0x1) == 0x1 )
            printf("%d ",memoria->RAM[i]);
        if ( endline == 0 )
            printf("\n");

        i++;
    }
    printf("\n");
}

void SYS3(Memoria *memoria, OperandosYFlags op)
{
    int i,j;
    int seg, finSeg;
    int aux;
    int cadLength;
    char cad[150];

    int edx = memoria->VectorDeRegistros[13] & 0xFFFF;
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;

    //Veo segmento de memoria donde se almacena el string
    aux = memoria->VectorDeRegistros[13] / 0x10000;
    if (aux == 0)
    {
        seg = op.segmento.ds;
        finSeg = op.segmento.es;
    }
    else if (aux == 1)
    {
        seg = op.segmento.ss;
        finSeg = op.segmento.finSS;
    }
    else if (aux == 2)
    {
        seg = op.segmento.es;
        finSeg = op.segmento.ss;
    }
    else if (aux == 3)
    {
        seg = op.segmento.cs;
        finSeg = op.segmento.ds;
    }

    i = edx + seg; //Me muevo por ram
    j = 0; //Me muevo en la cadena


    if ((ax && 0x800) == 0)
        printf("[%04d]: ",i);
    strcpy(cad,fgets(cad,cx+1,stdin));
    cadLength = strlen(cad);

    while ( (i<finSeg) && (i<(edx + seg + cx)) && (j<cadLength) )
    {
        memoria->RAM[i] = cad[j];
        i++;
        j++;
    }

    if (i>=finSeg)
    {
        memoria->error=1;
        printf("Segmentation Fault");
    }
    else if (j>=cadLength)
    {
        memoria->RAM[i-1] = 0;
    }
    else
        memoria->RAM[i] = 0;
}

void SYS4(Memoria *memoria, OperandosYFlags op)
{
    int i;
    int seg;
    int aux;

    int edx = memoria->VectorDeRegistros[13] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;

    //Veo segmento de memoria donde se almacena el string
    aux = memoria->VectorDeRegistros[13] / 0x10000;
    if (aux == 0)
        seg = op.segmento.ds;
    else if (aux == 1)
        seg = op.segmento.ss;
    else if (aux == 2)
        seg = op.segmento.es;
    else if (aux == 3)
        seg = op.segmento.cs;

    i = edx + seg; //Me muevo por ram

    if ((ax & 0x800) == 0)
        printf("[%04d]: ",i);

    while ( memoria->RAM[i] != 0 )
        printf("%c",memoria->RAM[i++]);

    if ( (ax & 0x100) == 0 )
        printf("\n");

}

void SYS7(Memoria *memoria, OperandosYFlags op)
{
    system("cls");
}

void SYSD(Memoria *memoria, OperandosYFlags op)
{
    int ah = (memoria->VectorDeRegistros[10] & 0xFF00) / 0x100;
    int al = memoria->VectorDeRegistros[10] & 0xFF;
    int ch = (memoria->VectorDeRegistros[12] & 0xFF00) / 0x100;
    int cl = memoria->VectorDeRegistros[12] & 0xFF;
    int dh = (memoria->VectorDeRegistros[13] & 0xFF00) / 0x100;
    int dl = (memoria->VectorDeRegistros[13] & 0xFF);
    //int ebx = memoria->VectorDeRegistros[11];
    int seg = memoria->VectorDeRegistros[11] / 0x10000;
    int offset = memoria->VectorDeRegistros[11] & 0xFFFF;
    int buffer[128];


    unsigned char cantCilindros = memoria->discos.info[dl].cantCilindros;
    unsigned char cantSectores = memoria->discos.info[dl].cantSectores;
    unsigned char cantCabezas = memoria->discos.info[dl].cantCabezas;
    unsigned int tamSector = memoria->discos.info[dl].tamSector;

    int pos = 512 + (ch * cantCilindros * cantSectores * tamSector) + (cl * cantSectores * tamSector) + (dh * tamSector);
    int posMax = cantCabezas * cantCilindros * cantSectores * 512;
    int posSegAct;
    int tamReal;
    int tamRealFinal = pos + (al * 512);

    if ( !(ah==0 || ah==0x02 || ah==0x03 || ah==0x08) ){ //Función Inválida
        memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0100;
        memoria->discos.info[dl].ultimoEstado = 0x01;
    }
    else { //Función válida
        if (dl >= memoria->discos.cant){ //No existe el disco
            printf("CANT:%d\n",memoria->discos.cant);
            printf("DL:%d\n",dl);
            memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x3100;
            memoria->discos.info[dl].ultimoEstado = 0x31;
        }
        else{ //Existe el disco
            if ( ah == 0 ){ //Consulta ultimo estado
                memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + (memoria->discos.info[dl].ultimoEstado * 0x100);
            }
            else if ( ah == 0x08 ){ //Obtener parámetros del disco
                memoria->VectorDeRegistros[10] = memoria->VectorDeRegistros[10] & 0xFFFF00FF; // AH = 00
                memoria->VectorDeRegistros[12] = (memoria->VectorDeRegistros[12] & 0xFFFF0000) + (cantCilindros * 0x100) +  cantCabezas; // CH Y CL
                memoria->VectorDeRegistros[13] = (memoria->VectorDeRegistros[13] & 0xFFFF00FF) + (cantSectores * 0x100); // DH
                printf("%08X:\n",memoria->VectorDeRegistros[10]);
                printf("%08X:\n",memoria->VectorDeRegistros[12]);
                printf("%08X:\n",memoria->VectorDeRegistros[13]);
            }
            else if (cl > cantCabezas){ //Numero invalido de cabezas
                memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000C00;
                memoria->discos.info[dl].ultimoEstado = 0xC;
            }
            else if (ch > cantCilindros){ //Numero invalido de cilindros
                memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000B00;
                memoria->discos.info[dl].ultimoEstado = 0xB;
            }
            else if (dh > cantSectores){ //Numero invalido de sectores
                memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000D00;
                memoria->discos.info[dl].ultimoEstado = 0xD;
            }
            else if ( ah == 0x02 ){ //Leer del disco
                if ((seg == 0) && (((offset+op.segmento.ds) + al*512)>op.segmento.es)){ //Error de lectura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000400;
                    memoria->discos.info[dl].ultimoEstado = 0x4;

                }
                else if ((seg == 2) && (((offset+op.segmento.es) + al*512)>op.segmento.ss)){ //Error de lectura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000400;
                    memoria->discos.info[dl].ultimoEstado = 0x4;
                }
                else{ //No es error de lectura -> Operación exitosa
                    int cero = 0;
                    int transferidos = 0;

                    //printf("PosACopiar: %d\n",posSegAct);
                    //Primero se valida que haya datos en disco, sino se rellena con 0's

                    fseek(memoria->discos.info[dl].arch,0,SEEK_END);
                    tamReal = ftell(memoria->discos.info[dl].arch);

                    while (tamReal < tamRealFinal){
                        fwrite(&cero,sizeof(int),1,memoria->discos.info[dl].arch);
                        tamReal++;
                    }

                    //Se lee hasta el final del disco o la cantidad de sectores indicados
                    //También se cuentan la cantidad de sectores leídos/transferidos

                    fseek(memoria->discos.info[dl].arch,pos,SEEK_SET);


                    if (seg==0)
                        posSegAct = op.segmento.ds + offset;
                    else if (seg==2)
                        posSegAct = op.segmento.es + offset;

                    while (pos<posMax && pos<tamRealFinal){
                        fread(&buffer,sizeof(buffer),1,memoria->discos.info[dl].arch);

                        for (int i=0; i<128; i++){
                            memoria->RAM[posSegAct] = buffer[i];
                            posSegAct++;
                        }
                        transferidos+=1; //Se transfirió un sector
                        pos+=sizeof(buffer);
                    }

                    if (pos>=posMax)
                        memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFFFF00) + (transferidos & 0xFF);
                    memoria->VectorDeRegistros[10] =  memoria->VectorDeRegistros[10] & 0xFFFF00FF;
                    memoria->discos.info[dl].ultimoEstado = 0x00;
                }
            }
            else if ( ah == 0x03 ){ //Escribir en disco
                if ((seg == 0) && (((offset+op.segmento.ds) + al*512)>op.segmento.es)){ //Error de escritura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0000CC00;
                    memoria->discos.info[dl].ultimoEstado = 0xCC;
                }
                else if ((seg == 2) && (((offset+op.segmento.es) + al*512)>op.segmento.ss)){ //Error de escritura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0000CC00;
                    memoria->discos.info[dl].ultimoEstado = 0xCC;
                }
                else if (tamRealFinal > posMax){ //Falla en la operación
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0000FF00;
                    memoria->discos.info[dl].ultimoEstado = 0xFF;
                }
                else{ // Procede a escribir -> Operación exitosa

                    fseek(memoria->discos.info[dl].arch,pos,SEEK_SET);

                    if (seg==0)
                        posSegAct = op.segmento.ds + offset;
                    else if (seg==2)
                        posSegAct = op.segmento.es + offset;

                    while (pos<tamRealFinal){
                        for (int i=0; i<128; i++){
                            buffer[i] = memoria->RAM[posSegAct];
                            posSegAct++;
                        }

                        fwrite(&buffer,sizeof(buffer),1,memoria->discos.info[dl].arch);
                        pos+=sizeof(buffer);
                    }

                    memoria->VectorDeRegistros[10] =  memoria->VectorDeRegistros[10] & 0xFFFF00FF;
                    memoria->discos.info[dl].ultimoEstado = 0x00;
                }
            }
        }
    }
}

void SYSF(Memoria *memoria, OperandosYFlags op)
{
    char sAux[255];
    int ip;
    int cod;
    VectorFunciones vecF;
    unsigned int dirMemoria;

    if (op.flags[0]==1)
    {
        iniciaVectorFunciones(vecF);


        if ( (0 <= (memoria->VectorDeRegistros[5] & 0xFFFF)) && ((memoria->VectorDeRegistros[5] & 0xFFFF)<(memoria->VectorDeRegistros[0]&0xFFFF)) && ((memoria->VectorDeRegistros[5]&0xFFFF)<op.cantInstrucciones) && memoria->error==0)
        {
            ip = memoria->VectorDeRegistros[5] & 0xFFFF;
            memoria->VectorDeRegistros[5]++;
            cod = decodificaCodigo(memoria->RAM[ip]);
            decodificaOperandos(memoria,cod,memoria->RAM[ip],&op);

            vecF[cod](memoria,op);
        }


        if (op.flags[1]==1)
            system("cls");

        if (op.flags[2]==1)
            disassembler(*memoria, op);

        printf("[%04d] cmd: ",(memoria->VectorDeRegistros[5]&0xFFFF) - 1);

        scanf("%[^\n]s%*c",sAux);

        while ( (0 <= (memoria->VectorDeRegistros[5] & 0xFFFF)) && ((memoria->VectorDeRegistros[5] & 0xFFFF)<=(memoria->VectorDeRegistros[0]&0xFFFF)) && (strcmp(sAux,"r")!=0) && ((memoria->VectorDeRegistros[5]&0xFFFF)<op.cantInstrucciones) && memoria->error==0)
        {
            if (strcmp(sAux,"p")==0)
            {
                if ((memoria->VectorDeRegistros[5] & 0xFFFF) != (memoria->VectorDeRegistros[0]&0xFFFF))
                {
                    ip = (memoria->VectorDeRegistros[5] & 0xFFFF);
                    memoria->VectorDeRegistros[5]++;
                    cod = decodificaCodigo(memoria->RAM[ip]);
                    decodificaOperandos(memoria,cod,memoria->RAM[ip],&op);
                    vecF[cod](memoria,op);
                }

                if (op.flags[1]==1)
                    system("cls");

                if (op.flags[2]==1)
                    disassembler(*memoria, op);

            }
            else if ( cuentaChars(sAux,' ',strlen(sAux)) == 0 )
            {
                dirMemoria = atoi(sAux);
                if (dirMemoria<4096)
                    printf("[%04d]: %04X %04X %d\n",dirMemoria,(memoria->RAM[dirMemoria] >> 16)&0xFFFF, memoria->RAM[dirMemoria]&0xFFFF,memoria->RAM[dirMemoria] );
            }
            else if ( cuentaChars(sAux,' ',strlen(sAux)) == 1 )
            {
                char *token;

                token = strtok(sAux," ");
                dirMemoria = atoi(token);
                token = strtok(NULL," ");

                while (dirMemoria < (atoi(token)+1) && dirMemoria<4096 && atoi(token)<4096)
                {
                    printf("[%04d]: %04X %04X %d\n",dirMemoria,(memoria->RAM[dirMemoria] / 0x10000)&0xFFFF, memoria->RAM[dirMemoria]&0xFFFF, memoria->RAM[dirMemoria]);
                    dirMemoria++;
                }
            }
            printf("[%04d] cmd: ",(memoria->VectorDeRegistros[5]&0xFFFF) - 1);
            fflush(stdin);
            scanf("%[^\n]%*c",sAux);
        }
        fflush(stdin);
    }
}

void JMP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JZ(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] == 0x1)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] == 0)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JN(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] == 0x80000000)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JNZ(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8]!= 0x1)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JNP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] != 0)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JNN(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] != 0x80000000)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void LDL(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[9] = (memoria->VectorDeRegistros[9] & 0xFFFF0000) + (op.operandoA[0] & 0xFFFF) ;
}

void LDH(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[9] = ((op.operandoA[0] & 0xFFFF) * 0x10000) + (memoria->VectorDeRegistros[9] & 0xFFFF);
}

void RND(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[9] = rand() % (op.operandoA[0]+1);
}

void NOT(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    int cc;

    aux = ~op.operandoA[0];


    if (op.operandoA[1] == 1)
    {
        //De registro

        //Si el operando A es de registro
        if (op.operandoA[2] == 0)
        {
            //Si el operando A es de 4 bytes
            memoria->VectorDeRegistros[op.operandoA[3]] = aux;
            cc = aux;
        }
        else if (op.operandoA[2] == 1)
        {
            //Si utilizamos el 4to byte del operando A
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + (aux & 0xFF);
            cc = (aux & 0xFF);
            if ( (0x80 & aux) != 0)
            {
                cc = aux << 24;
                cc>>=24;
            }
        }
        else if (op.operandoA[2] == 2)
        {
            //Si utilizamos el 3er byte del operando A
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + (aux & 0xFF00);
            cc = (aux & 0xFF00);
            if ( (0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
        }
        else if (op.operandoA[2] == 3)
        {
            //Si utilizamos los 2 ultimos bytes del operando A
            memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + (aux & 0xFFFF);
            cc = (aux & 0xFFFF);
            if ( (0x8000 & aux) != 0)
            {
                cc = aux << 16;
                cc>>=16;
            }
        }
    }
    else if (op.operandoA[1] == 2)
    {
        //Directo
        memoria->RAM[op.operandoA[4] + op.segmento.ds] = aux;
        cc = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        if (op.segmento.actualA == 0)  //DS
        {
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 2)    //ES
        {
            memoria->RAM[op.segmento.es + op.operandoA[4]] = aux;
        }
        else if (op.segmento.actualA == 1)     //SS
        {
            memoria->RAM[op.segmento.ss + op.operandoA[4]] = aux;
        }
        cc = aux;
    }


    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void PUSH(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    memoria->VectorDeRegistros[6]--;
    aux = memoria->VectorDeRegistros[6]&0xFFFF;
    if (aux >= op.segmento.ss)
        memoria->RAM[memoria->VectorDeRegistros[6]&0xFFFF] = op.operandoA[0];
    else
    {
        memoria->error=1;
        printf("Stack Overflow\n");
    }
}

void POP(Memoria *memoria, OperandosYFlags op)
{
    int aux = (memoria->VectorDeRegistros[6]&0xFFFF) + 1;

    if (aux >= op.segmento.finSS)
    {
        memoria->error=1;
        printf("Stack Underflow");
    }
    else
    {
        if (op.operandoA[1] == 1)
        {
            //Si el operando A es de registro
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)];
                memoria->VectorDeRegistros[6]++;
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)] & 0xFF;
                memoria->VectorDeRegistros[6]++;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)] & 0xFF;
                memoria->VectorDeRegistros[6]++;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + (aux * 0x100);
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A
                aux = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)] & 0xFFFF;
                memoria->VectorDeRegistros[6]++;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
            }
        }
        else if (op.operandoA[1] == 2)
        {
            //Si el operando A es directo
            memoria->RAM[op.segmento.ds + op.operandoA[4]] = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)];
            memoria->VectorDeRegistros[6]++;
        }
        else if (op.operandoA[1] == 3)
        {
            if (op.segmento.actualA == 0)  //DS
            {
                memoria->RAM[op.segmento.ds + op.operandoA[4]] = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)];
                memoria->VectorDeRegistros[6]++;
            }
            else if (op.segmento.actualA == 2)    //ES
            {
                memoria->RAM[op.segmento.es + op.operandoA[4]] = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)];
                memoria->VectorDeRegistros[6]++;
            }
            else if (op.segmento.actualA == 1)     //SS
            {
                memoria->RAM[op.segmento.ss + op.operandoA[4]] = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)];
                memoria->VectorDeRegistros[6]++;
            }
        }
    }
}

void CALL(Memoria *memoria, OperandosYFlags op)
{
    int aux;
    memoria->VectorDeRegistros[6]--;
    aux = (memoria->VectorDeRegistros[6]&0xFFFF);
    if (aux >= op.segmento.ss)
    {
        memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)] = memoria->VectorDeRegistros[5]&0xFFFF;
        if ((op.operandoA[0] < op.segmento.ds) && (op.operandoA[0]<op.cantInstrucciones))
            memoria->VectorDeRegistros[5] = (memoria->VectorDeRegistros[5] & 0xFFFF0000) + (op.operandoA[0]&0xFFFF);
    }
    else
    {
        memoria->error=1;
        printf("Stack Overflow");
    }
}

void RET(Memoria *memoria, OperandosYFlags op)
{
    int aux = (memoria->VectorDeRegistros[6]&0xFFFF) + 1;

    if (aux >= op.segmento.finSS)
    {
        memoria->error=1;
        printf("Stack Underflow");
    }
    else
    {
        aux = memoria->RAM[(memoria->VectorDeRegistros[6]&0xFFFF)];
        memoria->VectorDeRegistros[6]++;
        if ((aux < op.segmento.ds) && (aux<op.cantInstrucciones))
            memoria->VectorDeRegistros[5] = aux;
    }
}

void STOP(Memoria *memoria, OperandosYFlags op)
{
    exit(-1);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void imprimeMnemonico(int cod)
{
    char aux[6];
    switch(cod)
    {
    case 0x00:
        strcpy(aux,"MOV");
        break;
    case 0x01:
        strcpy(aux,"ADD");
        break;
    case 0x02:
        strcpy(aux,"SUB");
        break;
    case 0x03:
        strcpy(aux,"SWAP");
        break;
    case 0x04:
        strcpy(aux,"MUL");
        break;
    case 0x05:
        strcpy(aux,"DIV");
        break;
    case 0x06:
        strcpy(aux,"CMP");
        break;
    case 0x07:
        strcpy(aux,"SHL");
        break;
    case 0x08:
        strcpy(aux,"SHR");
        break;
    case 0x09:
        strcpy(aux,"AND");
        break;
    case 0x0A:
        strcpy(aux,"OR");
        break;
    case 0x0B:
        strcpy(aux,"XOR");
        break;
    case 0x0C:
        strcpy(aux,"SLEN");
        break;
    case 0x0D:
        strcpy(aux,"SMOV");
        break;
    case 0x0E:
        strcpy(aux,"SCMP");
        break;
    case 0xF0:
        strcpy(aux,"SYS");
        break;
    case 0xF1:
        strcpy(aux,"JMP");
        break;
    case 0xF2:
        strcpy(aux,"JZ");
        break;
    case 0xF3:
        strcpy(aux,"JP");
        break;
    case 0xF4:
        strcpy(aux,"JN");
        break;
    case 0xF5:
        strcpy(aux,"JNZ");
        break;
    case 0xF6:
        strcpy(aux,"JNP");
        break;
    case 0xF7:
        strcpy(aux,"JNN");
        break;
    case 0xF8:
        strcpy(aux,"LDL");
        break;
    case 0xF9:
        strcpy(aux,"LDH");
        break;
    case 0xFA:
        strcpy(aux,"RND");
        break;
    case 0xFB:
        strcpy(aux,"NOT");
        break;
    case 0xFC:
        strcpy(aux,"PUSH");
        break;
    case 0xFD:
        strcpy(aux,"POP");
        break;
    case 0xFE:
        strcpy(aux,"CALL");
        break;
    case 0xFF0:
        strcpy(aux,"RET");
        break;
    case 0xFF1:
        strcpy(aux,"STOP");
        break;
    }
    printf("%6s",aux);
}

void stringRegistro(int op[],char aux[])
{
    if (op[3]<10)
    {
        switch (op[3])
        {
        case 0:
            strcpy(aux,"DS");
            break;
        case 1:
            strcpy(aux,"SS");
            break;
        case 2:
            strcpy(aux,"ES");
            break;
        case 3:
            strcpy(aux,"CS");
            break;
        case 4:
            strcpy(aux,"HP");
            break;
        case 5:
            strcpy(aux,"IP");
            break;
        case 6:
            strcpy(aux,"SP");
            break;
        case 7:
            strcpy(aux,"BP");
            break;
        case 8:
            strcpy(aux,"CC");
            break;
        case 9:
            strcpy(aux,"AC");
            break;
        }
    }
    else
    {
        if (op[3] == 10)
        {
            switch (op[2])
            {
            case 0:
                strcpy(aux,"EAX");
                break;
            case 1:
                strcpy(aux,"AL");
                break;
            case 2:
                strcpy(aux,"AH");
                break;
            case 3:
                strcpy(aux,"AX");
                break;
            }
        }
        else if (op[3]==11)
        {
            switch (op[2])
            {
            case 0:
                strcpy(aux,"EBX");
                break;
            case 1:
                strcpy(aux,"BL");
                break;
            case 2:
                strcpy(aux,"BH");
                break;
            case 3:
                strcpy(aux,"BX");
                break;
            }
        }
        else if (op[3]==12)
        {
            switch (op[2])
            {
            case 0:
                strcpy(aux,"ECX");
                break;
            case 1:
                strcpy(aux,"CL");
                break;
            case 2:
                strcpy(aux,"CH");
                break;
            case 3:
                strcpy(aux,"CX");
                break;
            }
        }
        else if (op[3]==13)
        {
            switch (op[2])
            {
            case 0:
                strcpy(aux,"EDX");
                break;
            case 1:
                strcpy(aux,"DL");
                break;
            case 2:
                strcpy(aux,"DH");
                break;
            case 3:
                strcpy(aux,"DX");
                break;
            }
        }
        else if (op[3]==14)
        {
            switch (op[2])
            {
            case 0:
                strcpy(aux,"EEX");
                break;
            case 1:
                strcpy(aux,"EL");
                break;
            case 2:
                strcpy(aux,"EH");
                break;
            case 3:
                strcpy(aux,"EX");
                break;
            }
        }
        else if (op[3]==15)
        {
            switch (op[2])
            {
            case 0:
                strcpy(aux,"EFX");
                break;
            case 1:
                strcpy(aux,"FL");
                break;
            case 2:
                strcpy(aux,"FH");
                break;
            case 3:
                strcpy(aux,"FX");
                break;
            }
        }
    }
}

void imprimeOperandos(OperandosYFlags op,int cod,int instruccion)
{
    char aux[10], offset[10];
    int dir;

    if (cod<0xF)  //2 operandos
    {
        //Primer operando
        if (op.operandoA[1]==0)
        {
            //Inmediato
            printf("%10d, ",op.operandoA[0]);
        }
        else if (op.operandoA[1]==1)
        {
            //De registro
            stringRegistro(op.operandoA,aux);
            printf("%10s, ",aux);
        }
        else if (op.operandoA[1]==2)
        {
            //Directo
            dir = (instruccion & 0xFFF000)/0x1000;
            printf("%10c%d], ",'[',dir);
        }
        else if (op.operandoA[1]==3)
        {
            //Indirecto
            stringRegistro(op.operandoA,aux);
            strcat(aux, "+");
            itoa(op.operandoA[5], offset, 10);
            strcat(aux, offset);
            printf("%10c%s], ",'[',aux);
        }

        //Segundo operando
        if (op.operandoB[1]==0)
        {
            //Inmediato
            printf("%d",op.operandoB[0]);
        }
        else if (op.operandoB[1]==1)
        {
            //De registro
            stringRegistro(op.operandoB,aux);
            printf("%s",aux);
        }
        else if (op.operandoB[1]==2)
        {
            //Directo
            dir = (instruccion & 0xFFF);
            printf("[%d]",dir);
        }
        else if (op.operandoB[1]==3)
        {
            //Indirecto
            stringRegistro(op.operandoB,aux);
            strcat(aux, "+");
            itoa(op.operandoB[5], offset, 10);
            strcat(aux, offset);
            printf("[%s]",aux);
        }

    }
    else if (cod<0xFF)    //1 operando
    {
        //printf("\nTipo de operando A: %d\n",op.operandoA[1]);
        if (op.operandoA[1]==0)
        {
            //Inmediato
            printf("%10d",op.operandoA[0]);
        }
        else if (op.operandoA[1]==1)
        {
            //De registro
            stringRegistro(op.operandoA,aux);
            printf("%10s",aux);
        }
        else if (op.operandoA[1]==2)
        {
            //Directo
            dir = (instruccion & 0xFFFF);
            printf("%10c%d]",'[',dir);
        }
        else if (op.operandoA[1]==3)
        {
            //Indirecto
            stringRegistro(op.operandoA,aux);
            strcat(aux, "+");
            itoa(op.operandoA[5], offset, 10);
            strcat(aux, offset);
            printf("%10c%s]",'[',aux);
        }

    } //No imprime nada con 0 operandos
}

void imprimeEstadoRegistros(int vecR[])
{
    printf(" DS:%12d | SS:%12d | ES:%12d | CS:%12d |\n",vecR[0]&0xFFFF,vecR[1]&0xFFFF, vecR[2]&0xFFFF, vecR[3]&0xFFFF);
    printf(" HP:%12d | IP:%12d | SP:%12d | BP:%12d |\n",vecR[4]&0xFFFF,(vecR[5]-1)&0xFFFF,vecR[6]&0xFFFF,vecR[7]&0xFFFF);
    printf(" CC:%12d | AC:%12d |EAX:%12d |EBX:%12d |\n",vecR[8],vecR[9],vecR[10],vecR[11]);
    printf("ECX:%12d |EDX:%12d |EEX:%12d |EFX:%12d |\n",vecR[12],vecR[13],vecR[14],vecR[15]);
}

void disassembler(Memoria memoria, OperandosYFlags op)
{
    int ipAct = (memoria.VectorDeRegistros[5]&0xFFFF) - 1;
    int cod;
    int i;


    printf("Codigo:\n");

    for (i=ipAct-5; i<ipAct; i++)
    {
        if (i>=0)
        {
            printf(" [%04d]: ",i);
            printf("%02X ",(memoria.RAM[i] & 0xFF000000)/0x01000000);
            printf("%02X ",(memoria.RAM[i] & 0xFF0000)/0x00010000);
            printf("%02X ",(memoria.RAM[i] & 0xFF00)/0x00000100);
            printf("%02X ",(memoria.RAM[i] & 0xFF));
            printf("%4d: ",i+1);
            cod = decodificaCodigo(memoria.RAM[i]);
            decodificaOperandos(&memoria,cod,memoria.RAM[i],&op);
            imprimeMnemonico(cod);
            imprimeOperandos(op,cod,memoria.RAM[i]);
            printf("\n");
        }
        else
        {
            printf(" [----]: -- -- -- --\n");
        }
    }

    i=ipAct;
    printf(">[%04d]: ",i);
    printf("%02X ",(memoria.RAM[i] & 0xFF000000)/0x01000000);
    printf("%02X ",(memoria.RAM[i] & 0xFF0000)/0x00010000);
    printf("%02X ",(memoria.RAM[i] & 0xFF00)/0x00000100);
    printf("%02X ",(memoria.RAM[i] & 0xFF));
    printf("%4d: ",i+1);
    cod = decodificaCodigo(memoria.RAM[i]);
    decodificaOperandos(&memoria,cod,memoria.RAM[i],&op);
    imprimeMnemonico(cod);
    imprimeOperandos(op,cod,memoria.RAM[i]);
    printf("\n");

    for (int i=ipAct+1; i<ipAct+5; i++)
    {
        printf(" [%04d]: ",i);
        printf("%02X ",(memoria.RAM[i] & 0xFF000000)/0x01000000);
        printf("%02X ",(memoria.RAM[i] & 0xFF0000)/0x00010000);
        printf("%02X ",(memoria.RAM[i] & 0xFF00)/0x00000100);
        printf("%02X ",(memoria.RAM[i] & 0xFF));
        if (i<(memoria.VectorDeRegistros[0]&0xFFFF) && (i<op.cantInstrucciones))
        {
            printf("%4d: ",i+1);
            cod = decodificaCodigo(memoria.RAM[i]);
            decodificaOperandos(&memoria,cod,memoria.RAM[i],&op);
            imprimeMnemonico(cod);
            imprimeOperandos(op,cod,memoria.RAM[i]);
        }
        printf("\n");

    }

    printf("Registros:\n");

    imprimeEstadoRegistros(memoria.VectorDeRegistros);
}

void setDisco(int nro, Memoria *memoria)
{
    char nombreDisco[20];
    char aux[20];
    FILE *arch;
    DiskHead header;
    int r1, r2;
    time_t hora;
    struct tm *t;
    char strHora[80];

    itoa(nro,aux,10);
    strcpy(nombreDisco,"disco");
    strcat(nombreDisco,aux);
    strcat(nombreDisco,".vdd");

    arch = fopen(nombreDisco,"r+");
    if (arch!=NULL){
        memoria->discos.info[nro].arch = arch;
        memoria->discos.info[nro].nroUnidad = nro;
        fseek(arch,33, SEEK_SET);
        fread(&memoria->discos.info[nro].cantCilindros,sizeof(unsigned char),1,arch);
        fread(&memoria->discos.info[nro].cantCabezas,sizeof(unsigned char),1,arch);
        fread(&memoria->discos.info[nro].cantSectores,sizeof(unsigned char),1,arch);
        fread(&memoria->discos.info[nro].tamSector,sizeof(unsigned int),1,arch);
    }
    else
    {
        memoria->discos.info[nro].arch = fopen(nombreDisco,"w+");
        if (memoria->discos.info[nro].arch!=NULL)
        {

            header.idTipo = 0x56444430;

            header.version = 1;

            srand (rand() % 256);
            r1 = rand() % RAND_MAX;
            r2 = rand() % RAND_MAX;
            header.GUID1 = (r1*0x10000) + r2;
            srand (rand() % 150);
            r1 = rand() % RAND_MAX;
            r2 = rand() % RAND_MAX;
            header.GUID2 = (r1*0x10000) + r2;

            hora = time(0);
            t = localtime(&hora);
            strftime(strHora, sizeof(strHora), "%Y%m%d%H%M%S", t);

            header.fechaCreacion = (strHora[0]&0xF)<<28 | (strHora[1]&0xF)<<24 |
                (strHora[2]&0xF)<<20 | (strHora[3]&0xF)<<16 | (strHora[4]&0xF)<<12 |
                (strHora[5]&0xF)<<8 | (strHora[6]&0xF)<<4 | (strHora[7]&0xF);
            header.horaCreacion = (strHora[8]&0xF)<<28 | (strHora[9]&0xF)<<24 |
                (strHora[10]&0xF)<<20 | (strHora[11]&0xF)<<16 | (strHora[12]&0xF)<<12 |
                (strHora[13]&0xF)<<8 | (strHora[14]&0xF)<<4 | (strHora[15]&0xF);

            header.tipo = 1;
            header.cantCilindros = 128;
            header.cantCabezas = 128;
            header.cantSectores = 128;
            header.tamSector = 512;

            for (int i=0; i<118; i++)
                header.relleno[i]=0;

            fwrite(&header,sizeof(DiskHead),1,memoria->discos.info[nro].arch);

            memoria->discos.info[nro].nroUnidad = nro;
            memoria->discos.info[nro].cantCilindros = header.cantCilindros;
            memoria->discos.info[nro].cantCabezas = header.cantCabezas;
            memoria->discos.info[nro].cantSectores = header.cantSectores;
            memoria->discos.info[nro].tamSector = header.tamSector;
        }
    }
}

void inicializaDiscos(Memoria *memoria, int argc,char *argv[]){
    memoria->discos.cant=0;
    for (int i=0; i<255; i++){
        memoria->discos.info[i].arch = NULL;
        memoria->discos.info[i].ultimoEstado = 0;
    }

    for(int i=2; i<argc; i++){
        if( strstr(argv[i],"vdd") != NULL){
            setDisco(memoria->discos.cant,memoria);
            memoria->discos.cant++;
        }
    }
}


void inicializaFlags(OperandosYFlags *op,int argc,char *argv[]){
    for(int i=0; i<3; i++)
        op->flags[i]=0;

    for(int i=2; i<argc; i++)
    {
        if(!strcmp(argv[i],"-b"))
            op->flags[0]=1;
        else if(!strcmp(argv[i],"-c"))
            op->flags[1]=1;
        else if(!strcmp(argv[i],"-d"))
            op->flags[2]=1;
    }
}

void iniciaVectorFunciones(VectorFunciones vecF)
{
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
    vecF[0x0C]=&SLEN;
    vecF[0x0D]=&SMOV;
    vecF[0x0E]=&SCMP;

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
    vecF[0xFC]=&PUSH;
    vecF[0xFD]=&POP;
    vecF[0xFE]=&CALL;

    vecF[0xFF0]=&RET;
    vecF[0xFF1]=&STOP;
}

int verificoHeader(Header header)
{
    int ret=0;

    // MV-2: 0x4D562D32
    // V.22: 0x562E3232

    if ((header.bloque[0] == 0x4D562D32) && (header.bloque[5] == 0x562E3232))
        ret = 1;

    return ret;
}

int seteoSegmentos(Memoria *mem,Header header,OperandosYFlags *op)
{
    int segSize = header.bloque[1] + header.bloque[2] + header.bloque[3] + header.bloque[4];
    int ret = 0;

    if (sizeof(*mem)>=segSize)
    {
        mem->VectorDeRegistros[3] = header.bloque[4] * 0x10000;                                                          //CS
        mem->VectorDeRegistros[0] = header.bloque[1] * 0x10000 + header.bloque[4];                                       //DS
        mem->VectorDeRegistros[2] = header.bloque[3] * 0x10000 + (header.bloque[4] + header.bloque[1]);                  //ES
        mem->VectorDeRegistros[1] = (header.bloque[2] * 0x10000) + ( (mem->VectorDeRegistros[2]&0xFFFF) + header.bloque[3]);  //SS

        op->segmento.ds = mem->VectorDeRegistros[0]&0xFFFF;
        op->segmento.es = mem->VectorDeRegistros[2]&0xFFFF;
        op->segmento.ss = mem->VectorDeRegistros[1]&0xFFFF;
        op->segmento.finSS = (mem->VectorDeRegistros[1]&0xFFFF) + (mem->VectorDeRegistros[1]/0x10000);

        ret = 1;
    }

    return ret;
}

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

void decodificaOperandos(Memoria *memoria, int codigo, int instruccion, OperandosYFlags *op)
{
    if (codigo < 15)
    {
        //Tiene 2 operandos

        //Averiguo los tipos
        op->operandoA[1] = (instruccion & 0x0C000000) >> 26;
        op->operandoB[1] = (instruccion & 0x03000000) / 0x01000000;

        //--------------------------------------------------------------------------------------------
        //Operando B

        if (op->operandoB[1] == 0)
        {
            //Inmediato
            op->operandoB[0] = instruccion & 0xFFF;
            if ( (op->operandoB[0] & 0x800) != 0 )
            {
                op->operandoB[0]<<=20;
                op->operandoB[0]>>=20;
            }
        }
        else if ( op->operandoB[1] == 1 )
        {
            //Registro
            op->operandoB[2] = (instruccion & 0x30) / 0x10;
            op->operandoB[3] = instruccion & 0xF;

            if (op->operandoB[2] == 0)
                //Registro de 4 bytes
                op->operandoB[0] = memoria->VectorDeRegistros[op->operandoB[3]];
            else if (op->operandoB[2] == 1)
            {

                //4to byte del registro
                op->operandoB[0] = memoria->VectorDeRegistros[op->operandoB[3]] & 0xFF;
                if ( (op->operandoB[0] & 0x80) != 0 )
                {
                    op->operandoB[0]<<=24;
                    op->operandoB[0]>>=24;
                }
            }
            else if (op->operandoB[2] == 2)
            {

                //3er byte del registro
                //En este caso particular, el operando B queda corrido hacia la derecha
                op->operandoB[0] = (memoria->VectorDeRegistros[op->operandoB[3]] & 0xFF00) / 0x100;
                if ( (op->operandoB[0] & 0x80) != 0 )
                {
                    op->operandoB[0]<<=24;
                    op->operandoB[0]>>=24;
                }
            }
            else if (op->operandoB[2] == 3)
            {

                //Registro de 2 bytes
                op->operandoB[0] = memoria->VectorDeRegistros[op->operandoB[3]] & 0xFFFF;
                if ( (op->operandoB[0] & 0x8000) != 0 )
                {
                    op->operandoB[0]<<=16;
                    op->operandoB[0]>>=16;
                }
            }
        }
        else if (op->operandoB[1] == 2) //Directo
        {
            op->segmento.actualB = 0;

            op->operandoB[4] = (instruccion & 0xFFF) ;

            if ((op->operandoB[4]+op->segmento.ds)>=(memoria->VectorDeRegistros[2]&0xFFFF))
            {
                memoria->error = 1;
                printf("Segmentation Fault\n");
            }
            else
                op->operandoB[0] = memoria->RAM[op->operandoB[4] + op->segmento.ds];

        }
        else if (op->operandoB[1] == 3)
        {
            //Indirecto

            op->operandoB[5] = (instruccion & 0xFF0);
            op->operandoB[5]<<=20;
            op->operandoB[5]>>=24;

            //Registro
            op->operandoB[2] = 0;   //4 bytes
            op->operandoB[3] = instruccion & 0xF; //REG
            op->operandoB[4] = (memoria->VectorDeRegistros[op->operandoB[3]] & 0xFFFF) + op->operandoB[5]; // [REG+OFFSET]

            //Segmento
            op->segmento.actualB = (memoria->VectorDeRegistros[op->operandoB[3]] / 0x10000) & 0xF;

            if (op->segmento.actualB == 0)  //DS
            {
                if ((op->operandoB[4] + op->segmento.ds <= op->segmento.ds) || (op->operandoB[4] + op->segmento.ds >= op->segmento.es))
                    memoria->error = 1;
            }
            else if (op->segmento.actualB == 1)    //SS
            {
                if (op->operandoB[3]==7){
                    if (op->operandoB[4]>=op->segmento.finSS)
                        memoria->error = 1;
                } else if ((op->operandoB[4] + op->segmento.ss <= op->segmento.ss) || (op->operandoB[4] + op->segmento.ss >= op->segmento.finSS)){
                    memoria->error = 1;
                }
            }
            else if (op->segmento.actualB == 2)    //ES
            {
                if ((op->operandoB[4] + op->segmento.es <= op->segmento.es) || (op->operandoB[4] + op->segmento.es >= op->segmento.ss))
                    memoria->error = 1;
            }
            else if (op->segmento.actualB == 3){    //CS
                if (op->operandoB[4] + op->segmento.cs < op->segmento.cs || (op->operandoB[4] + op->segmento.cs >= op->segmento.ds))
                    memoria->error = 1;
            }

            if (memoria->error==1)
                printf("Segmentation Fault\n");
            else if (op->operandoB[3]==7)
                op->operandoB[0] = memoria->RAM[op->operandoB[4]];
            else
                op->operandoB[0] = memoria->RAM[(memoria->VectorDeRegistros[op->segmento.actualB]&0xFFFF) + op->operandoB[4]];
        }

        //--------------------------------------------------------------------------------------------
        //Operando A

        if (memoria->error!=1)
        {
            if (op->operandoA[1] == 0)
            {
                //Inmediato
                op->operandoA[0] = (instruccion & 0xFFF000) / 0x1000;
                if ( (op->operandoA[0] & 0x800) != 0 )
                {
                    op->operandoA[0]<<=20;
                    op->operandoA[0]>>=20;
                }
            }
            else if(op->operandoA[1] == 1)
            {
                //Registro
                op->operandoA[3] = (instruccion & 0xF000) / 0x1000;
                op->operandoA[2] = (instruccion & 0x30000) / 0x10000;

                if (op->operandoA[2] == 0)
                    //Registro de 4 bytes
                    op->operandoA[0] = memoria->VectorDeRegistros[op->operandoA[3]];
                else if(op->operandoA[2] == 1)
                {

                    //4to byte del registro
                    op->operandoA[0] = memoria->VectorDeRegistros[op->operandoA[3]] & 0xFF;
                    if ( (op->operandoA[0] & 0x80) != 0 )
                    {
                        op->operandoA[0]<<=24;
                        op->operandoA[0]>>=24;
                    }
                }
                else if (op->operandoA[2] == 2)
                {

                    //3er byte del registro
                    op->operandoA[0] = memoria->VectorDeRegistros[op->operandoA[3]] & 0xFF00;
                    if ( (op->operandoA[0] & 0x80) != 0 )
                    {
                        op->operandoA[0]<<=24;
                        op->operandoA[0]>>=24;
                    }
                }
                else if (op->operandoA[2] == 3)
                {

                    //Registro de 2 bytes
                    op->operandoA[0] = (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF);
                    if ( (op->operandoA[0] & 0x8000) != 0 )
                    {
                        op->operandoA[0]<<=16;
                        op->operandoA[0]>>=16;
                    }
                }
            }
            else if (op->operandoA[1] == 2)
            {
                // Tipo de operando A: directo
                op->operandoA[4] = (instruccion & 0xFFF000) / 0x1000 ;

                op->segmento.actualA = 0;

                if ((op->operandoA[4]+(op->segmento.actualA))>=(memoria->VectorDeRegistros[2]&0xFFFF))
                {
                    memoria->error = 1;
                    printf("Segmentation Fault\n");
                }
                else
                    op->operandoA[0] = memoria->RAM[op->operandoA[4] + op->segmento.ds];
            }
            else if (op->operandoA[1] == 3)
            {
                //Indirecto

                op->operandoA[5] = (instruccion & 0xFF0000);
                op->operandoA[5]<<=8;
                op->operandoA[5]>>=24;

                //Registro
                op->operandoA[2] = 0;   //4 bytes
                op->operandoA[3] = (instruccion & 0xF000) / 0x1000; //REG
                op->operandoA[4] = (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5]; // [REG+OFFSET]

                //Segmento
                op->segmento.actualA = (memoria->VectorDeRegistros[op->operandoA[3]] / 0x10000) & 0xF;

                if (op->segmento.actualA == 0)  //DS
                {
                    if ((op->operandoA[4] + op->segmento.ds <= op->segmento.ds) || (op->operandoA[4] + op->segmento.ds >= op->segmento.es))
                        memoria->error = 1;
                }
                else if (op->segmento.actualA == 1)    //SS
                {
                    if (op->operandoA[3]==7){
                        if (op->operandoA[4]>=op->segmento.finSS)
                            memoria->error=1;
                    } else if ((op->operandoA[4] + op->segmento.ss <= op->segmento.ss) || (op->operandoA[4] + op->segmento.ss >= op->segmento.finSS)){
                            memoria->error = 1;
                    }
                }
                else if (op->segmento.actualA == 2)    //ES
                {
                    if ((op->operandoA[4] + op->segmento.es <= op->segmento.es) || (op->operandoA[4] + op->segmento.es >= op->segmento.ss))
                        memoria->error = 1;
                }

                if (memoria->error==1)
                    printf("Segmentation Fault\n");
                else if (op->operandoA[3]==7)
                    op->operandoB[0] = memoria->RAM[op->operandoA[4]];
                else
                    op->operandoA[0] = memoria->RAM[(memoria->VectorDeRegistros[op->segmento.actualA]&0xFFFF) + op->operandoA[4]];
            }
        }
    }
    else if (codigo < 0xFF)
    {
        //Tiene 1 operando

        //Averiguo el tipo de operando
        op->operandoA[1] = (instruccion & 0x00C00000) / 0x100000;
        op->operandoA[1]>>=2;

        //Analizo operando
        if (op->operandoA[1] == 0) //Inmediato
        {
            op->operandoA[0] = instruccion & 0xFFFF;

            if ( (op->operandoA[0] & 0x8000) != 0 )
            {
                op->operandoA[0]<<=16;
                op->operandoA[0]>>=16;
            }
        }
        else if (op->operandoA[1] == 1)
        {
            //De registro
            op->operandoA[2] = (instruccion & 0x30) / 0x10;
            op->operandoA[3] = instruccion & 0xF;

            if (op->operandoA[2]  == 0)
                //Registro de 4 bytes
                op->operandoA[0] = memoria->VectorDeRegistros[op->operandoA[3]];
            else if (op->operandoA[2] == 1)
            {
                //4to byte del registro
                op->operandoA[0] = memoria->VectorDeRegistros[op->operandoA[3]] & 0xFF;
                if ( (op->operandoA[0] & 0x80) != 0 )
                {
                    op->operandoA[0]<<=24;
                    op->operandoA[0]>>=24;
                }
            }
            else if (op->operandoA[2] == 2)
            {
                //3er byte del registro
                ////En este caso particular, el operando A queda corrido hacia la derecha
                op->operandoA[0] = (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFF00) / 0x10;
                if ( (op->operandoA[0] & 0x80) != 0 )
                {
                    op->operandoA[0]<<=24;
                    op->operandoA[0]>>=24;
                }
            }
            else if (op->operandoA[2]  == 3)
            {
                //Registro de 2 bytes
                op->operandoA[0] = memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF;
                if ( (op->operandoA[0] & 0x8000) != 0 )
                {
                    op->operandoA[0]<<=16;
                    op->operandoA[0]>>=16;
                }
            }
        }
        else if (op->operandoA[1] == 2)  //Directo
        {
            op->operandoA[4] = (instruccion & 0xFFFF);

            op->segmento.actualA = 0;

            if ((op->operandoA[4]+op->segmento.ds)>=(memoria->VectorDeRegistros[2]&0xFFFF))
            {
                memoria->error = 1;
                printf("Segmentation Fault\n");
            }
            else
                op->operandoA[0] = memoria->RAM[op->operandoA[4] + op->segmento.ds];
        }
        else if (op->operandoA[1] == 3)
        {
            //Indirecto

            op->operandoA[5] = (instruccion & 0xFF0);
            op->operandoA[5]<<=20;
            op->operandoA[5]>>=24;

            //Registro
            op->operandoA[2] = 0;   //4 bytes
            op->operandoA[3] = instruccion & 0xF; //REG
            op->operandoA[4] = (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5]; // [REG+OFFSET]

            //Segmento
            op->segmento.actualA = (memoria->VectorDeRegistros[op->operandoA[3]] / 0x10000) & 0xF;

            if (op->segmento.actualA == 0)  //DS
            {
                if ((op->operandoA[4] + op->segmento.ds <= op->segmento.ds) || (op->operandoA[4] + op->segmento.ds >= op->segmento.es))
                    memoria->error = 1;
            }
            else if (op->segmento.actualA == 1)    //SS
            {
                if (op->operandoA[3]==7){
                    if (op->operandoA[4]>=op->segmento.finSS)
                        memoria->error=1;
                } else if ((op->operandoA[4] + op->segmento.ss <= op->segmento.ss) || (op->operandoA[4] + op->segmento.ss >= op->segmento.finSS)){
                            memoria->error = 1;
                }
                /*if ((op->operandoA[4] + op->segmento.ss <= op->segmento.ss) || (op->operandoA[4] + op->segmento.ss >= op->segmento.finSS))
                    memoria->error = 1;*/
            }
            else if (op->segmento.actualA == 2)    //ES
            {
                if ((op->operandoA[4] + op->segmento.es <= op->segmento.es) || (op->operandoA[4] + op->segmento.es >= op->segmento.ss))
                    memoria->error = 1;
            }

            if (memoria->error==1)
                printf("Segmentation Fault\n");
            else if (op->operandoA[3]==7)
                op->operandoA[0] = memoria->RAM[op->operandoA[4]];
            else
                op->operandoA[0] = memoria->RAM[(memoria->VectorDeRegistros[op->segmento.actualA]&0xFFFF) + op->operandoA[4]];
        }
    }
}

void QuitaCaracter(char cadena[],char caracter,int longitud)
{
    char aux[longitud];
    int i,j=0;

    for (i = 0; i < longitud; i++)
    {
        if (cadena[i] != caracter)
        {
            aux[j] = cadena[i];
            j++;
        }
    }
    aux[j] = '\0';
    strcpy(cadena,aux);
}

int cuentaChars(char cadena[], char caracter,int longitud)
{
    int cont=0;

    for (int i = 0; i < longitud; i++)
        if (cadena[i] == caracter)
            cont++;

    return cont;
}

void inicializaRegistros(Memoria *mem,OperandosYFlags op)
{
    for (int i=8; i<16; i++)
        mem->VectorDeRegistros[i]=0;

    mem->VectorDeRegistros[4] = 0x00020000; //HP
    mem->VectorDeRegistros[5] = 0x00030000; //IP
    mem->VectorDeRegistros[6] = 0x00010000 + ((op.segmento.finSS) & 0xFFFF); //SP
    mem->VectorDeRegistros[7] = 0x00010000; //BP

}
