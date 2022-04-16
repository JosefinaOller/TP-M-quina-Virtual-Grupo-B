#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = op.operandoB[0];
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
        aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] + op.operandoB[0];
        cc = aux;
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;
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
        aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] - op.operandoB[0];
        cc = aux;
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;
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
                aux = op.operandoA[0];
                memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
                if (op.operandoB[2] == 2)
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= ((aux * 0x100) & 0xFF00);
                }
                else
                    memoria->VectorDeRegistros[op.operandoB[3]] = aux;
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = (op.operandoA[0] & 0xFF);
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + (op.operandoB[0] & 0xFF);
                if (op.operandoB[2] == 2)
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= ((aux * 0x100) & 0xFF00);
                }
                else
                    memoria->VectorDeRegistros[op.operandoB[3]] = (memoria->VectorDeRegistros[op.operandoB[3]] & 0xFFFFFF00) + aux;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = (op.operandoA[0] & 0xFF00);
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + ((op.operandoB[0] * 0x100 ) & 0xFF00);
                if (op.operandoB[2] == 2)
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= aux;
                }
                else
                    memoria->VectorDeRegistros[op.operandoB[3]] = (aux / 0x100);
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A
                aux = (op.operandoA[0] & 0xFFFF);
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + (op.operandoB[0] & 0xFFFF);
                if (op.operandoB[2] == 2)
                {
                    memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                    memoria->VectorDeRegistros[op.operandoB[3]]+= ((aux * 0x100) & 0xFF00);
                }
                else
                    memoria->VectorDeRegistros[op.operandoB[3]] = (memoria->VectorDeRegistros[op.operandoB[3]] & 0xFFFF0000) + aux;
            }
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 2)) //Ambos son de tipo Directo
        {
            aux = memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]] = memoria->RAM[op.operandoB[4] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoB[4] + memoria->VectorDeRegistros[0]] = aux;
        }
        else if ((op.operandoA[1] == 1) && (op.operandoB[1] == 2)) //A tipo de registro y B tipo directo
        {
            //Si el operando A es de registro
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                aux = op.operandoA[0];
                memoria->VectorDeRegistros[op.operandoA[3]] = memoria->RAM[op.operandoB[4]+memoria->VectorDeRegistros[0]];
                memoria->RAM[op.operandoB[4] + memoria->VectorDeRegistros[0]] = aux;
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = (op.operandoA[0] & 0xFF);
                memoria->VectorDeRegistros[op.operandoA[3]]&=0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoA[3]]+=memoria->RAM[op.operandoB[4]+memoria->VectorDeRegistros[0]] & 0xFF;
                memoria->RAM[op.operandoB[4] + memoria->VectorDeRegistros[0]] = aux;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = ( op.operandoA[0] & 0xFF00) / 0x100;
                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoA[3]] += (memoria->RAM[op.operandoB[4]+memoria->VectorDeRegistros[0]] & 0xFF) * 0x100;
                memoria->RAM[op.operandoB[4] + memoria->VectorDeRegistros[0]] = aux;
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A

                aux = (op.operandoA[0] & 0xFFFF);
                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoA[3]] += memoria->RAM[op.operandoB[4]+memoria->VectorDeRegistros[0]] & 0xFFFF;
                memoria->RAM[op.operandoB[4] + memoria->VectorDeRegistros[0]] = aux;
            }
        }
        else //A tipo directo y B tipo de registro
        {
            aux = memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]];
            memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]] = op.operandoB[0];
            if (op.operandoB[2] == 2)
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoB[3]]+= ((aux * 0x100) & 0xFF00);
            }
            else
                memoria->VectorDeRegistros[op.operandoB[3]] = aux;
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
        aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] * op.operandoB[0];
        cc = aux;
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;

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

            valorInicial = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]];
            aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] / op.operandoB[0];
            cc = aux;
            memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;
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
        aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] - op.operandoB[0];
    }
    else if (op.operandoA[1] == 0)
    {
        //Si el operando A es inmediato
        aux = op.operandoA[0] - op.operandoB[0];
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
        aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] << op.operandoB[0];
        cc = aux;
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;

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
        aux = memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] >> op.operandoB[0];
        cc = aux;
        memoria->RAM[memoria->VectorDeRegistros[0] + op.operandoA[4]] = aux;

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
        memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]] = aux;

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
        memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]] = aux;

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
        memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]] = aux;

    }
    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

void SYS(Memoria *memoria, OperandosYFlags op)
{
    void (*FuncionesSYS[3])(Memoria *,OperandosYFlags);

    FuncionesSYS[0]=&SYS1;
    FuncionesSYS[1]=&SYS2;
    FuncionesSYS[2]=&SYSF;

    if ( op.operandoA[0] == 0x1 )  //SYS READ
    {
        FuncionesSYS[0](memoria,op);
    }
    else if (op.operandoA[0] == 0x2)    //SYS WRITE
    {
        FuncionesSYS[1](memoria,op);
    }
    else if (op.operandoA[0] == 0xF)    //SYS BREAKPOINT
    {
        FuncionesSYS[2](memoria,op);
    }
}

void SYS1(Memoria *memoria, OperandosYFlags op)
{
    int edx = memoria->VectorDeRegistros[13];
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;
    int prompt = ( ax & 0x0800 ) / 0x100;
    int inputMode = ( ax & 0x0100 ) / 0x100;
    int tipoDato;
    char num[16];

    if (inputMode == 1)
    {
        int i = edx + memoria->VectorDeRegistros[0];
        int j = 0;
        char sAux[255];

        if (prompt == 0)
            printf("[%04d]: ", memoria->VectorDeRegistros[5]-1);

        scanf("%[^\n]%*c",sAux);

        while ( (j<strlen(sAux)) && (i < (edx + cx + memoria->VectorDeRegistros[0] - 1)))
        {
            //En la condición va un -1 ya que reserva el espacio para el /0 al final
            memoria->RAM[i] = sAux[j];
            i++;
            j++;
        }
        memoria->RAM[i] = '\0';

    }
    else
    {
        tipoDato = (ax & 0xF);

        if (tipoDato == 0x1 )  //Decimal
        {
            for (int i = (edx + memoria->VectorDeRegistros[0]); i< (edx + cx + memoria->VectorDeRegistros[0]); i++)
            {
                if (prompt == 0)
                    printf("[%04d]: ", memoria->VectorDeRegistros[5]-1);
                scanf("%s",num);
                QuitaCaracter(num,'\'',strlen(num));
                QuitaCaracter(num,'#',strlen(num));
                memoria->RAM[i] = atoi(num);
            }

        }
        else if (tipoDato == 0x4)    //Octal
        {
            for (int i = (edx + memoria->VectorDeRegistros[0]); i< (edx + cx + memoria->VectorDeRegistros[0]); i++)
            {
                if (prompt == 0)
                    printf("[%04d]: ", memoria->VectorDeRegistros[5]-1);
                scanf("%s",num);
                QuitaCaracter(num,'@',strlen(num));
                sscanf(num,"%o",&(memoria->RAM[i]));
            }

        }
        else if (tipoDato == 0x8)    //Hexadecimal
        {
            for (int i = (edx + memoria->VectorDeRegistros[0]); i< (edx + cx + memoria->VectorDeRegistros[0]); i++)
            {
                if (prompt == 0)
                    printf("[%04d]: ", memoria->VectorDeRegistros[5]-1);
                scanf("%s",num);
                QuitaCaracter(num,'%',strlen(num));
                sscanf(num,"%X",&(memoria->RAM[i]));
            }
        }
    }
}

void SYS2(Memoria *memoria, OperandosYFlags op)
{
    int edx = memoria->VectorDeRegistros[13];
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;
    int prompt = ( ax & 0x0800 ) / 0x100;
    int endline = ( ax & 0x0100 ) / 0x100;
    int tipoDato = ax & 0xFF;

    for (int i=(edx + memoria->VectorDeRegistros[0]); i<(edx + cx+ memoria->VectorDeRegistros[0]); i++)
    {
        if (prompt == 0)
            printf("[%04d]: ",i - memoria->VectorDeRegistros[0]);
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
    }
    printf("\n");
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


        if ( (0 <= memoria->VectorDeRegistros[5]) && (memoria->VectorDeRegistros[5]<memoria->VectorDeRegistros[0]))
        {
            ip = memoria->VectorDeRegistros[5];
            memoria->VectorDeRegistros[5]++;
            cod = decodificaCodigo(memoria->RAM[ip]);
            decodificaOperandos(*memoria,cod,memoria->RAM[ip],&op);
            vecF[cod](memoria,op);
        }

        if (op.flags[1]==1)
            system("cls");

        if (op.flags[2]==1)
            disassembler(*memoria, op);

        printf("[%04d]: ",memoria->VectorDeRegistros[5] - 1);

        scanf("%[^\n]s%*c",sAux);

        while ( (0 <= memoria->VectorDeRegistros[5]) && (memoria->VectorDeRegistros[5]<=memoria->VectorDeRegistros[0]) && (strcmp(sAux,"r")!=0) )
        {
            if (strcmp(sAux,"p")==0)
            {
                if (memoria->VectorDeRegistros[5] != memoria->VectorDeRegistros[0])
                {
                    ip = memoria->VectorDeRegistros[5];
                    memoria->VectorDeRegistros[5]++;
                    cod = decodificaCodigo(memoria->RAM[ip]);
                    decodificaOperandos(*memoria,cod,memoria->RAM[ip],&op);
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
            printf("[%04d]: ",memoria->VectorDeRegistros[5] - 1);
            fflush(stdin);
            scanf("%[^\n]%*c",sAux);
        }
        fflush(stdin);
    }
}

void JMP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
        memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JZ(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
        if (memoria->VectorDeRegistros[8] == 0x1)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
        if (memoria->VectorDeRegistros[8] == 0)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JN(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
        if (memoria->VectorDeRegistros[8] == 0x80000000)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JNZ(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
        if (memoria->VectorDeRegistros[8]!= 0x1)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JNP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
        if (memoria->VectorDeRegistros[8] != 0)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

void JNN(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < memoria->VectorDeRegistros[0])
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
    //La descripción de la instrucción en la hoja está mal
    //RND debe almacenar en AC un valor aleatorio entre 0 y el valor del operando

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
        memoria->RAM[op.operandoA[4] + memoria->VectorDeRegistros[0]] = aux;
        cc = aux;
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

void STOP(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[5] = 0x7FFFFFFF;
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
        case 5:
            strcpy(aux,"IP");
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
    char aux[6];
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

    } //No imprime nada con 0 operandos
}

void imprimeEstadoRegistros(int vecR[])
{
    printf(" DS:%12d | %15c | %15c | %15c |\n",vecR[0],' ',' ',' ');
    printf(" %15c | IP:%12d | %15c | %15c |\n",' ',vecR[5]-1,' ',' ');
    printf(" CC:%12d | AC:%12d |EAX:%12d |EBX:%12d |\n",vecR[8],vecR[9],vecR[10],vecR[11]);
    printf("ECX:%12d |EDX:%12d |EEX:%12d |EFX:%12d |\n",vecR[12],vecR[13],vecR[14],vecR[15]);
}

void disassembler(Memoria memoria, OperandosYFlags op)
{
    int ipAct = memoria.VectorDeRegistros[5] - 1;
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
            decodificaOperandos(memoria,cod,memoria.RAM[i],&op);
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
    decodificaOperandos(memoria,cod,memoria.RAM[i],&op);
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
        if (i<memoria.VectorDeRegistros[0])
        {
            printf("%4d: ",i+1);
            cod = decodificaCodigo(memoria.RAM[i]);
            decodificaOperandos(memoria,cod,memoria.RAM[i],&op);
            imprimeMnemonico(cod);
            imprimeOperandos(op,cod,memoria.RAM[i]);
        }
        printf("\n");

    }

    printf("Registros:\n");

    imprimeEstadoRegistros(memoria.VectorDeRegistros);
}

void inicializaFlags(OperandosYFlags *op,int argc,char *argv[])
{

    for(int i=0; i<4; i++)
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
}

int verificoHeader(Header header)
{
    int ret=0;

    // MV-1: 0x4D562D31
    // V.22: 0x562E3232

    if ((header.bloque[0] == 0x4D562D31) && (header.bloque[5] == 0x562E3232))
        ret = 1;

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

void decodificaOperandos(Memoria memoria, int codigo, int instruccion, OperandosYFlags *op)
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
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]];
            else if (op->operandoB[2] == 1)
                //4to byte del registro
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]] & 0xFF;
            else if (op->operandoB[2] == 2)
                //3er byte del registro
                //En este caso particular, el operando B queda corrido hacia la derecha
                op->operandoB[0] = (memoria.VectorDeRegistros[op->operandoB[3]] & 0xFF00) / 0x100;
            else if (op->operandoB[2] == 3)
                //Registro de 2 bytes
                op->operandoB[0] = memoria.VectorDeRegistros[op->operandoB[3]] & 0xFFFF;
        }
        else if (op->operandoB[1] == 2) //Directo
        {
            op->operandoB[0] = memoria.RAM[ (instruccion & 0xFFF) + memoria.VectorDeRegistros[0]];
            op->operandoB[4] = (instruccion & 0xFFF);
        }

        //--------------------------------------------------------------------------------------------
        //Operando A

        if (op->operandoA[1] == 0)
            //Inmediato
            op->operandoA[0] = (instruccion & 0xFFF000) / 0x1000;
        else if(op->operandoA[1] == 1)
        {
            //Registro
            op->operandoA[3] = (instruccion & 0xF000) / 0x1000;
            op->operandoA[2] = (instruccion & 0x30000) / 0x10000;

            if (op->operandoA[2] == 0)
                //Registro de 4 bytes
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]];
            else if(op->operandoA[2] == 1)
                //4to byte del registro
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xFF;
            else if (op->operandoA[2] == 2)
                //3er byte del registro
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xFF00;
            else if (op->operandoA[2] == 3)
                //Registro de 2 bytes
                op->operandoA[0] = (memoria.VectorDeRegistros[op->operandoA[3]] & 0xFFFF);
        }
        else if (op->operandoA[1] == 2)
        {

            // Tipo de operando A: directo
            op->operandoA[0] = memoria.RAM[((instruccion & 0xFFF000) / 0x1000)+ memoria.VectorDeRegistros[0]];
            op->operandoA[4] = (instruccion & 0xFFF000) / 0x1000;
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
        }
        else if (op->operandoA[1] == 1)
        {
            //De registro
            op->operandoA[2] = (instruccion & 0x30) / 0x10;
            op->operandoA[3] = instruccion & 0xF;

            if (op->operandoA[2]  == 0)
                //Registro de 4 bytes
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]];
            else if (op->operandoA[2] == 1)
                //4to byte del registro
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xFF;
            else if (op->operandoA[2] == 2)
                //3er byte del registro
                ////En este caso particular, el operando A queda corrido hacia la derecha
                op->operandoA[0] = (memoria.VectorDeRegistros[op->operandoA[3]] & 0xFF00) / 0x10;
            else if (op->operandoA[2]  == 3)
                //Registro de 2 bytes
                op->operandoA[0] = memoria.VectorDeRegistros[op->operandoA[3]] & 0xFFFF;

        }
        else if (op->operandoA[1] == 2)  //Directo
        {
            op->operandoA[0] = memoria.RAM[(instruccion & 0xFFFF) + memoria.VectorDeRegistros[0]];
            op->operandoA[4] = (instruccion & 0xFFFF);
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


