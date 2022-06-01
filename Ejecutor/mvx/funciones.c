#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "memoria.h"
#include "funciones.h"


/*Función MOV: Se averigua el tipo de operando al que se le va a asignar un valor
y luego se le asigna el valor*/
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
        memoria->RAM[op.operandoA[4]] = op.operandoB[0];
    }
    else if (op.operandoA[1] == 3)
    {
        //Operando A Indirecto
        memoria->RAM[op.operandoA[4]] = op.operandoB[0];
    }

}

/*Función ADD: Se averigua el tipo de operando al que se le va a sumar un valor
y luego se le suma el valor, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        aux = memoria->RAM[op.operandoA[4]] + op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = memoria->RAM[op.operandoA[4]] + op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if ( cc<0 )
        memoria->VectorDeRegistros[8] = 0x80000000;
    else if ( cc==0 )
        memoria->VectorDeRegistros[8] = 0x00000001;
    else
        memoria->VectorDeRegistros[8] = 0;
}

/*Función SUB: Se averigua el tipo de operando al que se le va a restar un valor
y luego se le resta el valor, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        aux = memoria->RAM[op.operandoA[4]] - op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = memoria->RAM[op.operandoA[4]] - op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if ( cc<0 )
        memoria->VectorDeRegistros[8] = 0x80000000;
    else if ( cc==0 )
        memoria->VectorDeRegistros[8] = 0x00000001;
    else
        memoria->VectorDeRegistros[8] = 0;
}

/*Función SWAP: Se averigua el tipo de ambos operandos. Luego se intercambian los valores acorde a los tipos*/
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
            memoria->RAM[op.operandoA[4]] = op.operandoB[0];
            memoria->RAM[op.operandoB[4]] = op.operandoA[0];
        }
        else if ((op.operandoA[1] == 3) && (op.operandoB[1] == 3)) //Ambos son de tipo indirecto
        {
            memoria->RAM[op.operandoA[4]] = op.operandoB[0];
            memoria->RAM[op.operandoB[4]] = op.operandoA[0];
        }
        else if ((op.operandoA[1] == 1) && (op.operandoB[1] == 2)) //A tipo de registro y B tipo directo
        {
            //Si el operando A es de registro
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
                memoria->RAM[op.operandoB[4]] = op.operandoA[0];
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoA[3]]+= op.operandoB[0] & 0xFF;
                memoria->RAM[op.operandoB[4]] = op.operandoA[0] & 0xFF;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoA[3]] += (op.operandoB[0] & 0xFF) * 0x100;
                memoria->RAM[op.operandoB[4]] = (op.operandoA[0] & 0xFF00) / 0x100;
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoA[3]] += op.operandoB[0] & 0xFFFF;
                memoria->RAM[op.operandoB[4]] = op.operandoA[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 1) && (op.operandoB[1] == 3)) //A tipo registro y B indirecto
        {
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = op.operandoB[0];
                memoria->RAM[op.operandoB[4]] = op.operandoA[0];
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoA[3]]+= op.operandoB[0] & 0xFF;
                memoria->RAM[op.operandoB[4]] = op.operandoA[0] & 0xFF;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoA[3]] += (op.operandoB[0] & 0xFF) * 0x100;
                memoria->RAM[op.operandoB[4]] = (op.operandoA[0] & 0xFF00) / 0x100;
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A

                memoria->VectorDeRegistros[op.operandoA[3]] &= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoA[3]] += op.operandoB[0] & 0xFFFF;
                memoria->RAM[op.operandoB[4]] = op.operandoA[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 1))//A tipo directo y B tipo de registro
        {
            if (op.operandoB[2] == 0) //4 bytes
            {
                memoria->VectorDeRegistros[op.operandoB[3]] = op.operandoA[0];
                memoria->RAM[op.operandoA[4]] = op.operandoB[0];
            }
            else if (op.operandoB[2] == 1) //4to byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFF;
                memoria->RAM[op.operandoA[4]] = op.operandoB[0] & 0xFF;

            }
            else if (op.operandoB[2] == 2) //3er byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoB[3]]+= ((op.operandoA[0] * 0x100) & 0xFF00);
                memoria->RAM[op.operandoA[4]] = op.operandoB[0] & 0xFF;
            }
            else if (op.operandoB[2] == 3) //Ultimos 2 bytes
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFFFF;
                memoria->RAM[op.operandoA[4]] = op.operandoB[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 3) && (op.operandoB[1] == 1))//A tipo indirecto y B tipo de registro
        {
            if (op.operandoB[2] == 0) //4 bytes
            {
                memoria->VectorDeRegistros[op.operandoB[3]] = op.operandoA[0];
                memoria->RAM[op.operandoA[4]] = op.operandoB[0];
            }
            else if (op.operandoB[2] == 1) //4to byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFFFF00;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFF;
                memoria->RAM[op.operandoA[4]] = op.operandoB[0] & 0xFF;

            }
            else if (op.operandoB[2] == 2) //3er byte
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF00FF;
                memoria->VectorDeRegistros[op.operandoB[3]]+= ((op.operandoA[0] * 0x100) & 0xFF00);
                memoria->RAM[op.operandoA[4]] = op.operandoB[0] & 0xFF;
            }
            else if (op.operandoB[2] == 3) //Ultimos 2 bytes
            {
                memoria->VectorDeRegistros[op.operandoB[3]]&= 0xFFFF0000;
                memoria->VectorDeRegistros[op.operandoB[3]]+= op.operandoA[0] & 0xFFFF;
                memoria->RAM[op.operandoA[4]] = op.operandoB[0] & 0xFFFF;
            }
        }
        else if ((op.operandoA[1] == 2) && (op.operandoB[1] == 3))//A tipo directo y B tipo indirecto
        {
            memoria->RAM[op.operandoA[4]] = op.operandoB[0];
            memoria->RAM[op.operandoB[4]] = op.operandoA[0];
        }
        else if ((op.operandoA[1] == 3) && (op.operandoB[1] == 2))//A tipo indirecto y B tipo directo
        {
            memoria->RAM[op.operandoA[4]] = op.operandoB[0];
            memoria->RAM[op.operandoB[4]] = op.operandoA[0];
        }
    }
}

/*Función MUL: Se averigua el tipo de operando al que se le va a multiplicar un valor
y luego se le multiplica el valor, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        aux = memoria->RAM[op.operandoA[4]] * op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = memoria->RAM[op.operandoA[4]] * op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if ( cc<0 )
        memoria->VectorDeRegistros[8] = 0x80000000;
    else if ( cc==0 )
        memoria->VectorDeRegistros[8] = 0x00000001;
    else
        memoria->VectorDeRegistros[8] = 0;
}

/*Función DIV: Se averigua el tipo de operando al que se le va a dividir un valor
y luego se le divide el valor, realizando un corrimiento por si el valor es negativo y debe propagar el signo pero sólo
SI ESTE ES DISTINTO DE 0. Caso contrario se omite la operación.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado.
Y utiliza otro auxiliar "valorInicial" para modificar el AC con el resto de la división*/
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
            valorInicial = memoria->RAM[op.operandoA[4]];
            aux = memoria->RAM[op.operandoA[4]] / op.operandoB[0];
            cc = aux;
            memoria->RAM[op.operandoA[4]] = aux;
            memoria->VectorDeRegistros[9] = valorInicial % op.operandoB[0];
        }
        else if (op.operandoA[1] == 3)
        {
            //Indirecto
            valorInicial = memoria->RAM[op.operandoA[4]];
            aux = memoria->RAM[op.operandoA[4]] / op.operandoB[0];
            cc = aux;
            memoria->RAM[op.operandoA[4]] = aux;
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

/*Función CMP: Se averigua el tipo de operando al que se le va a comparar un valor
y luego se resta el valor en un auxiliar, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
void CMP(Memoria *memoria, OperandosYFlags op)
{
    int aux=0;
    if (op.operandoA[1] == 0)
    {
        //Si el operando A es inmediato
        aux = op.operandoA[0] - op.operandoB[0];
    }
    else if (op.operandoA[1] == 1)
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
        aux = memoria->RAM[op.operandoA[4]] - op.operandoB[0];
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = memoria->RAM[op.operandoA[4]] - op.operandoB[0];
    }

    if (aux == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if (aux < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

/*Función SHL: Se averigua el tipo de operando al que se le va a realizar un corrimiento
y luego se le corre el valor, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        aux = memoria->RAM[op.operandoA[4]] << op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = memoria->RAM[op.operandoA[4]] << op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if (cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

/*Función SHR: Se averigua el tipo de operando al que se le va a realizar un corrimiento
y luego se le corre el valor, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        aux = memoria->RAM[op.operandoA[4]] >> op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = memoria->RAM[op.operandoA[4]] >> op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if (cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

/*Función AND: Se averigua el tipo de operando al que se le va a realizar un AND
y luego se realiza la operación, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        memoria->RAM[op.operandoA[4]] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = op.operandoA[0] & op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

/*Función OR: Se averigua el tipo de operando al que se le va a realizar un OR
y luego se realiza la operación, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        memoria->RAM[op.operandoA[4]] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = op.operandoA[0] | op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;


}

/*Función XOR: Se averigua el tipo de operando al que se le va a realizar un XOR
y luego se realiza la operación, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        memoria->RAM[op.operandoA[4]] = aux;

    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        aux = op.operandoA[0] ^ op.operandoB[0];
        cc = aux;
        memoria->RAM[op.operandoA[4]] = aux;
    }
    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;

}

/*Función SLEN: Se averigua el tipo de operando, en caso de no ser correctos no se realiza la operación.
Se calcula la longitud del string y luego acorde al tipo de operando se almacena la longitud.*/
void SLEN(Memoria *memoria, OperandosYFlags op)
{
    int aux, length=0;

    if (!(op.operandoA[1]==0 || op.operandoB[1]==0 || op.operandoB[1]==1))
    {
        //Primero se calcula la longitud del string
        aux = memoria->RAM[op.operandoB[4]];
        while (aux!=0)
        {
            length++;
            op.operandoB[4]++;
            aux = memoria->RAM[op.operandoB[4]];
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
            memoria->RAM[op.operandoA[4]] = length;
        }
        else if (op.operandoA[1] == 3)
        {
            //Si A es indirecto
            memoria->RAM[op.operandoA[4]] = length;
        }
    }
}

/*Función SMOV: Se averigua el tipo de operando, en caso de no ser correctos no se realiza la operación.
Se obtienen en op[4] las direcciones de comienzo y destino del string y a partir de ellas
se pasan uno a uno los valores. Una vez que termina se almacena el 0 (fin de cadena) en la siguiente posicion del destino*/
void SMOV(Memoria *memoria, OperandosYFlags op)
{
    int aux;

    if (!(op.operandoA[1]==0 || op.operandoA[1]==1 || op.operandoB[1]==0 || op.operandoB[1]==1))
    {
        aux = memoria->RAM[op.operandoB[4]];
        while (aux!=0)
        {
            memoria->RAM[op.operandoA[4]] = aux;
            op.operandoB[4]++;
            op.operandoA[4]++;
            aux = memoria->RAM[op.operandoB[4]];
        }
        memoria->RAM[op.operandoA[4]] = 0;
    }
}

/*Función SCMP: Se averigua el tipo de operando, en caso de no ser correctos no se realiza la operación.
Se obtienen en op[4] las direcciones de comienzo de ambos strings y a partir de ellas
se comparan uno a uno los valores. Si uno de ellos es mayor que otro, esto se refleja en el CC*/
void SCMP(Memoria *memoria, OperandosYFlags op)
{
    int cc=0,aux;

    if (!(op.operandoA[1]==0 || op.operandoA[1]==1 || op.operandoB[1]==0 || op.operandoB[1]==1))
    {
        while ((cc==0) && (memoria->RAM[op.operandoA[4]]!=0) && (memoria->RAM[op.operandoB[4]]!=0) )
        {
            aux = memoria->RAM[op.operandoA[4]] - memoria->RAM[op.operandoB[4]];

            if (aux<0)
                cc = -1;
            else if (aux>0)
                cc = 1;
            op.operandoA[4]++;
            op.operandoB[4]++;
        }

        if (memoria->RAM[op.operandoA[4]]==0 && memoria->RAM[op.operandoB[4]]==0)
            cc = 0;
        else if (memoria->RAM[op.operandoA[4]]==0)
            cc = 1;
        else if (memoria->RAM[op.operandoB[4]]==0)
            cc = -1;

        if (cc == 0)
            memoria->VectorDeRegistros[8] = 0x1;
        else if(cc < 0)
            memoria->VectorDeRegistros[8] = 0x80000000;
        else
            memoria->VectorDeRegistros[8] = 0;
    }

}

/*Funcion SYS: Llama los distintos SYS acorde al valor del operando*/
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

/*Funcion SYS1: Leer especificación en TP*/
void SYS1(Memoria *memoria, OperandosYFlags op)
{
    //Se almacenan las variables utilizadas en la especificación.
    //Se utiliza un prompt para ver si se imprime la dirección de memoria por pantalla
    //Se utiliza inputMode para saber el tipo de ingreso de datos

    int edx;
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;
    int prompt = ( ax & 0x0800 ) / 0x100;
    int inputMode = ( ax & 0x0100 ) / 0x100;
    int tipoDato;
    char num[16];
    int seg, finSeg;
    int j;
    int nroSegmento;
    int tope;

    //Se averigua el tipo de segmento y acorde a ello se almacena dónde comienza éste
    nroSegmento = memoria->VectorDeRegistros[13] / 0x10000;

    if (nroSegmento == 0)
    {
        seg = op.segmento.ds;
        finSeg = op.segmento.es;
    }
    else if (nroSegmento == 1)
    {
        seg = op.segmento.ss;
        finSeg = op.segmento.finSS;
    }
    else if (nroSegmento == 2)
    {
        seg = op.segmento.es;
        finSeg = op.segmento.ss;
    }
    else if (nroSegmento == 3)
    {
        seg = op.segmento.cs;
        finSeg = op.segmento.ds;
    }

    //Se asigna la posición de comienzo de escritura
    edx = seg + (memoria->VectorDeRegistros[13] & 0xFFFF);
    tope = edx + cx;


    if (inputMode == 1)
    {
        //Formato de ingreso hasta enter
        j = 0;
        char sAux[255];

        if (prompt == 0)
            printf("[%04d]: ", edx);

        fgets(sAux,cx+1,stdin);
        while ((edx<finSeg) && (j<strlen(sAux)) && (edx < tope))
        {
            memoria->RAM[edx] = sAux[j];
            edx++;
            j++;
        }
        if (edx>=finSeg)
            memoria->error=1;
    }
    else
    {
        //Formato de ingreso por cantidad de valores

        //Se averigua el tipo de dato
        tipoDato = (ax & 0xF);

        //Acorde al tipo de dato, escribe el dato como decimal, hexa, octal.
        while ((edx<finSeg) && (edx < tope))
        {
            if (prompt == 0)
                printf("[%04d]: ", edx);
            scanf("%s",num);
            if (tipoDato == 0x1)
            {
                QuitaCaracter(num,'\'',strlen(num));
                QuitaCaracter(num,'#',strlen(num));
                memoria->RAM[edx] = atoi(num);
            }
            else if (tipoDato == 0x4)
            {
                QuitaCaracter(num,'@',strlen(num));
                sscanf(num,"%o",&(memoria->RAM[edx]));
            }
            else if (tipoDato == 0x8)
            {
                QuitaCaracter(num,'%',strlen(num));
                sscanf(num,"%X",&(memoria->RAM[edx]));
            }
            edx++;
        }
        //Si se ingresa a un segmento de memoria fuera del límite del actual, habrá error de segmentación
        if (edx>=finSeg)
            memoria->error=1;
    }
}

/*Funcion SYS2: Leer especificación en TP*/
void SYS2(Memoria *memoria, OperandosYFlags op)
{

    //Se almacenan las variables utilizadas en la especificación.
    //Se utiliza un prompt para ver si se imprime la dirección de memoria por pantalla

    int edx;
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;
    int prompt = ( ax & 0x0800 ) / 0x100;
    int endline = ( ax & 0x0100 ) / 0x100;
    int tipoDato = ax & 0xFF;
    int nroSegmento;
    int seg;
    int tope;

    //Se averigua el tipo de segmento y acorde a ello se almacena dónde comienza éste
    nroSegmento = memoria->VectorDeRegistros[13] / 0x10000;
    if (nroSegmento == 0)
        seg = op.segmento.ds;
    else if (nroSegmento == 1)
        seg = op.segmento.ss;
    else if (nroSegmento == 2)
        seg = op.segmento.es;
    else if (nroSegmento == 3)
        seg = op.segmento.cs;

    //Se asigna la posición de comienzo de escritura
    edx = seg + (memoria->VectorDeRegistros[13] & 0xFFFF);
    tope = edx+cx;

    //Acorde al tipo de dato, se muestra el dato como decimal, hexa, octal, caracter o todos los señalados.
    while ((edx<tope) && (edx<op.segmento.finSS))
    {
        if (prompt == 0)
            printf("[%04d]: ",edx);
        if ( (tipoDato & 0x10) == 0x10)
        {
            unsigned char caracter = memoria->RAM[edx] & 0xFF;
            if (caracter<32 || caracter>126)
                printf("%c ",'.');
            else
                printf("%c ",caracter);
        }
        if ( (tipoDato & 0x8) == 0x8 )
            printf("%c%X ",'%',memoria->RAM[edx]);
        if ( (tipoDato & 0x4) == 0x4 )
            printf("%c%o ",'@',memoria->RAM[edx]);
        if ( (tipoDato & 0x1) == 0x1 )
            printf("%d ",memoria->RAM[edx]);
        if ( endline == 0 )
            printf("\n");

        edx++;
    }
    //Si se intentó acceder a una posición fuera de la memoria, hay un error de segmentación
    if (edx>=op.segmento.finSS)
        memoria->error = 1;
    printf("\n");
}

/*Funcion SYS3: Leer especificación en TP*/
void SYS3(Memoria *memoria, OperandosYFlags op)
{
    int j;
    int seg, finSeg;
    int nroSegmento;
    int cadLength;
    char cad[150];
    int tope;

    int edx;
    int cx = memoria->VectorDeRegistros[12] & 0xFFFF;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;

    //Veo segmento de memoria donde se almacena el string
    nroSegmento = memoria->VectorDeRegistros[13] / 0x10000;
    if (nroSegmento == 0)
    {
        seg = op.segmento.ds;
        finSeg = op.segmento.es;
    }
    else if (nroSegmento == 1)
    {
        seg = op.segmento.ss;
        finSeg = op.segmento.finSS;
    }
    else if (nroSegmento == 2)
    {
        seg = op.segmento.es;
        finSeg = op.segmento.ss;
    }
    else if (nroSegmento == 3)
    {
        seg = op.segmento.cs;
        finSeg = op.segmento.ds;
    }

    //Posicion donde se comienza a almacenar el string
    edx = seg + (memoria->VectorDeRegistros[13] & 0xFFFF);
    j = 0; //Me muevo en la cadena
    tope = edx + cx;

    //Se muestra el prompt
    if ((ax && 0x800) == 0)
        printf("[%04d]: ",edx);
    strcpy(cad,fgets(cad,cx+1,stdin));
    cadLength = strlen(cad);

    //Se almacena el string caracter a caracter
    while ( (edx<finSeg) && (edx<tope) && (j<cadLength) )
    {
        memoria->RAM[edx] = cad[j];
        edx++;
        j++;
    }
    //Si se acaba el segmento, hay error de segmentación.
    if (edx>=finSeg)
        memoria->error=1;
    else if (j>=cadLength)
        memoria->RAM[edx-1] = 0;
    else if (edx>=tope)
        memoria->RAM[edx] = 0;
}

/*Funcion SYS4: Leer especificación en TP*/
void SYS4(Memoria *memoria, OperandosYFlags op)
{
    int seg;
    int nroSegmento;
    int edx;
    int ax = memoria->VectorDeRegistros[10] & 0xFFFF;

    //Veo segmento de memoria donde se almacena el string
    nroSegmento = memoria->VectorDeRegistros[13] / 0x10000;
    if (nroSegmento == 0)
        seg = op.segmento.ds;
    else if (nroSegmento == 1)
        seg = op.segmento.ss;
    else if (nroSegmento == 2)
        seg = op.segmento.es;
    else if (nroSegmento == 3)
        seg = op.segmento.cs;

    //Posicion desde la cual se empieza a leer
    edx = seg + (memoria->VectorDeRegistros[13] & 0xFFFF);

    //Se muestra o no el prompt
    if ((ax & 0x800) == 0)
        printf("[%04d]: ",edx);

    //Se muestra el string hasta encontrar un fin de cadena o error de segmentación
    while ( (edx < op.segmento.finSS) && (memoria->RAM[edx] != 0) )
        printf("%c",memoria->RAM[edx++]);

    if (edx>= op.segmento.finSS)
        memoria->error=1;

    //Escribe o no \n al final de la muestra
    if ( (ax & 0x100) == 0 )
        printf("\n");

}

/*Funcion SYS7: Leer especificación en TP*/
void SYS7(Memoria *memoria, OperandosYFlags op)
{
    //Limpia la pantalla
    system("cls");
}

/*Funcion SYSD: Leer especificación en TP*/
void SYSD(Memoria *memoria, OperandosYFlags op)
{
    //Se inicializan las variables necesarias
    int ah = (memoria->VectorDeRegistros[10] & 0xFF00) / 0x100;
    int al = memoria->VectorDeRegistros[10] & 0xFF;
    int ch = (memoria->VectorDeRegistros[12] & 0xFF00) / 0x100;
    int cl = memoria->VectorDeRegistros[12] & 0xFF;
    int dh = (memoria->VectorDeRegistros[13] & 0xFF00) / 0x100;
    int dl = (memoria->VectorDeRegistros[13] & 0xFF);
    int seg = (memoria->VectorDeRegistros[11] / 0x10000)&0xFFFF;
    int offset = memoria->VectorDeRegistros[11] & 0xFFFF;
    int buffer[128];

    //Se obtienen los parámetros del disco
    unsigned char cantCilindros = memoria->discos.info[dl].cantCilindros;
    unsigned char cantSectores = memoria->discos.info[dl].cantSectores;
    unsigned char cantCabezas = memoria->discos.info[dl].cantCabezas;
    unsigned int tamSector = memoria->discos.info[dl].tamSector;

    //Se calcula la posición inicial donde se comienza a leer o escribir
    int pos = 512 + (ch * cantCilindros * cantSectores * tamSector) + (cl * cantSectores * tamSector) + (dh * tamSector);
    //Se calcula la posición límite física del disco
    int posMax = cantCabezas * cantCilindros * cantSectores * 512;
    int posSegAct;
    int tamReal;
    //Se calcula la posición límite virtual del disco
    int tamRealFinal = pos + (al * 512);

    if ( !(ah==0 || ah==0x02 || ah==0x03 || ah==0x08) ){ //Función Inválida
        memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0100;
        memoria->discos.info[dl].ultimoEstado = 0x01;
    }
    else { //Función válida
        if (dl >= memoria->discos.cant){ //No existe el disco
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
                if ((seg == 0) && (((offset+op.segmento.ds) + al*128)>op.segmento.es)){ //Error de lectura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000400;
                    memoria->discos.info[dl].ultimoEstado = 0x4;

                }
                else if ((seg == 2) && (((offset+op.segmento.es) + al*128)>op.segmento.ss)){ //Error de lectura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x00000400;
                    memoria->discos.info[dl].ultimoEstado = 0x4;
                }
                else{ //No es error de lectura -> Operación exitosa
                    int cero = 0;
                    int transferidos = 0;

                    //Primero se valida que haya datos en disco, sino se rellena con 0's

                    fseek(memoria->discos.info[dl].arch,0,SEEK_END);
                    tamReal = ftell(memoria->discos.info[dl].arch);

                    while (tamReal <= tamRealFinal){
                        fwrite(&cero,sizeof(int),1,memoria->discos.info[dl].arch);
                        tamReal+=4;
                    }

                    //Se lee hasta el final del disco o la cantidad de sectores indicados
                    //También se cuentan la cantidad de sectores leídos/transferidos


                    //Nos posicionamos en donde debemos comenzar a leer
                    fseek(memoria->discos.info[dl].arch,pos,SEEK_SET);

                    //Se calcula la posición de memoria donde se comienzan a escribir los datos
                    if (seg==0)
                        posSegAct = op.segmento.ds + offset;
                    else if (seg==2)
                        posSegAct = op.segmento.es + offset;

                    //Mientras no termine de leer datos del disco o llegue a su límite
                    while ( (pos<posMax) && (pos<tamRealFinal)){
                        fread(&buffer,sizeof(buffer),1,memoria->discos.info[dl].arch);

                        for (int i=0; i<128; i++){
                            memoria->RAM[posSegAct] = buffer[i];
                            posSegAct++;
                        }

                        transferidos+=1; //Se transfirió un sector
                        pos+=sizeof(buffer);
                    }


                    //En caso de llegar a su límite, indico la cantidad de sectores transferidos
                    if (pos>=posMax)
                        memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFFFF00) + (transferidos & 0xFF);

                    memoria->VectorDeRegistros[10] =  memoria->VectorDeRegistros[10] & 0xFFFF00FF;
                    memoria->discos.info[dl].ultimoEstado = 0x00;
                }
            }
            else if ( ah == 0x03 ){ //Escribir en disco
                if ((seg == 0) && (((offset+op.segmento.ds) + al*128)>op.segmento.es)){ //Error de escritura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0000CC00;
                    memoria->discos.info[dl].ultimoEstado = 0xCC;
                }
                else if ((seg == 2) && (((offset+op.segmento.es) + al*128)>op.segmento.ss)){ //Error de escritura
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0000CC00;
                    memoria->discos.info[dl].ultimoEstado = 0xCC;
                }
                else if (tamRealFinal > posMax){ //Falla en la operación
                    memoria->VectorDeRegistros[10] = (memoria->VectorDeRegistros[10] & 0xFFFF00FF) + 0x0000FF00;
                    memoria->discos.info[dl].ultimoEstado = 0xFF;
                }
                else{ // Procede a escribir -> Operación exitosa

                    //Nos posicionamos en la posición donde se comienza a escribir
                    fseek(memoria->discos.info[dl].arch,pos,SEEK_SET);

                    //Se calcula la dirección de memoria de la cual se extraen datos
                    if (seg==0)
                        posSegAct = op.segmento.ds + offset;
                    else if (seg==2)
                        posSegAct = op.segmento.es + offset;

                    //Mientras queden datos por transferir de memoria a disco se escribe en el disco
                    while (pos<tamRealFinal){

                        for (int i=0; i<128; i++){
                            buffer[i] = memoria->RAM[posSegAct];
                            posSegAct++;
                        }

                        fseek(memoria->discos.info[dl].arch,pos,SEEK_SET);
                        fwrite(&buffer,sizeof(int),1,memoria->discos.info[dl].arch);
                        pos+=sizeof(buffer);
                    }
                    memoria->VectorDeRegistros[10] =  memoria->VectorDeRegistros[10] & 0xFFFF00FF;
                    memoria->discos.info[dl].ultimoEstado = 0x00;

                }
            }
        }
    }
}

/*Funcion SYSF: Leer especificación en TP*/
void SYSF(Memoria *memoria, OperandosYFlags op)
{

    char sAux[255]; //Valor ingresado por consola
    int ip; //Ip actual real
    int ipEjecutado; //Ip utilizado para mostrar en el dissasembler
    int ipAux; //Ip auxiliar utilizado para que el Ejecutado no pierda su valor
    int cod; //Codigo de operacion
    VectorFunciones vecF;
    unsigned int dirMemoria; //Direccion de memoria a imprimir

    //-b
    if (op.flags[0]==1)
    {
        iniciaVectorFunciones(vecF);

        ip = memoria->VectorDeRegistros[5] & 0xFFFF;
        ipEjecutado = ip;

        //Si se cumplen las condiciones y no hay error, se ejecuta la instruccion
        if ( (0 <= ip) && (ip < op.segmento.ds) && (ip < op.cantInstrucciones) && memoria->error==0)
        {
            memoria->VectorDeRegistros[5]++;
            //Se decodifica el código de operación
            cod = decodificaCodigo(memoria->RAM[ip]);
            //Se decodifican los operandos
            decodificaOperandos(memoria,cod,memoria->RAM[ip],&op);
            //Se ejecuta la función
            vecF[cod](memoria,op);
            //Se guarda el nuevo ip
            ip = memoria->VectorDeRegistros[5] & 0xFFFF;
        }

        //Comprobación de error
        if (memoria->error!=0){
            printf("\nError: %s\n",memoria->msjError[memoria->error].detalle);
            exit(-1);
        }

        //-c
        if (op.flags[1]==1)
            system("cls");

        //-d
        if (op.flags[2]==1)
            disassembler(memoria, op, ipEjecutado);

        printf("[%04d] cmd: ",ipEjecutado);

        ipAux = ipEjecutado;
        ipEjecutado = ip;

        //Siguiente operación  (p, r, valor)
        scanf("%[^\n]s%*c",sAux);

        //Se sigue utilizando el SYSF hasta ingresar R o que ocurra un error o que se terminen las instrucciones
        while ( (0 <= ip) && (ip < op.segmento.ds) && (strcmp(sAux,"r")!=0) && (ip<op.cantInstrucciones) && memoria->error==0)
        {
            if (strcmp(sAux,"p")==0)
            {
                memoria->VectorDeRegistros[5]++;
                cod = decodificaCodigo(memoria->RAM[ip]);
                decodificaOperandos(memoria,cod,memoria->RAM[ip],&op);
                vecF[cod](memoria,op);
                ip = memoria->VectorDeRegistros[5] & 0xFFFF;

                fflush(stdin);
                if (memoria->error!=0){
                    printf("\nError: %s\n",memoria->msjError[memoria->error].detalle);
                    exit(-1);
                }

                if (op.flags[1]==1)
                    system("cls");

                if (op.flags[2]==1)
                    disassembler(memoria, op, ipEjecutado);

                ipAux = ipEjecutado;

            }
            else if ( cuentaChars(sAux,' ',strlen(sAux)) == 0 )
            {
                //Se imprime la direccion de memoria ingresada en sAux
                dirMemoria = atoi(sAux);
                if (dirMemoria<4096)
                    printf("[%04d]: %04X %04X %d\n",dirMemoria,(memoria->RAM[dirMemoria] >> 16)&0xFFFF, memoria->RAM[dirMemoria]&0xFFFF,memoria->RAM[dirMemoria] );
                ipEjecutado = ipAux;
            }
            else if ( cuentaChars(sAux,' ',strlen(sAux)) == 1 )
            {
                //Se imprime el rango de direcciones de memoria ingresadas en sAux
                char *token;

                token = strtok(sAux," ");
                dirMemoria = atoi(token);
                token = strtok(NULL," ");

                while (dirMemoria < (atoi(token)+1) && dirMemoria<4096 && atoi(token)<4096)
                {
                    printf("[%04d]: %04X %04X %d\n",dirMemoria,(memoria->RAM[dirMemoria] / 0x10000)&0xFFFF, memoria->RAM[dirMemoria]&0xFFFF, memoria->RAM[dirMemoria]);
                    dirMemoria++;
                }
                ipEjecutado = ipAux;
            }

            printf("[%04d] cmd: ",ipEjecutado);
            ipEjecutado = ip;
            fflush(stdin);
            scanf("%[^\n]%*c",sAux);
        }

        if (memoria->error!=0){
            printf("\nError: %s\n",memoria->msjError[memoria->error].detalle);
            exit(-1);
        }

        fflush(stdin);
    }
}

//Función JMP: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JMP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función JZ: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JZ(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] == 0x1)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función JP: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] == 0)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función JN: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JN(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] == 0x80000000)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función JNZ: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JNZ(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8]!= 0x1)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función JNP: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JNP(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] != 0)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función JNN: Se comprueban las condiciones del salto y caso satisfactorio, se cambia el IP
void JNN(Memoria *memoria, OperandosYFlags op)
{
    if (op.operandoA[0] < op.cantInstrucciones)
        if (memoria->VectorDeRegistros[8] != 0x80000000)
            memoria->VectorDeRegistros[5] = op.operandoA[0];
}

//Función LDL: Se carga la parte baja del AC
void LDL(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[9] = (memoria->VectorDeRegistros[9] & 0xFFFF0000) + (op.operandoA[0] & 0xFFFF) ;
}

//Función LDL: Se carga la parte alta del AC
void LDH(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[9] = ((op.operandoA[0] & 0xFFFF) * 0x10000) + (memoria->VectorDeRegistros[9] & 0xFFFF);
}

//Función RND: Se calcula un random entre 0 y el operando. Se almacena en AC
void RND(Memoria *memoria, OperandosYFlags op)
{
    memoria->VectorDeRegistros[9] = rand() % (op.operandoA[0]+1);
}

/*Función NOT: Se averigua el tipo de operando al que se le va a realizar un NOT
y luego se realiza la operación, realizando un corrimiento por si el valor es negativo y debe propagar el signo.
Además se almacena el resultado en un auxiliar para luego cambiar el cc acorde al resultado*/
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
        memoria->RAM[op.operandoA[4]] = aux;
        cc = aux;
    }
    else if (op.operandoA[1] == 3)
    {
        //Indirecto
        memoria->RAM[op.operandoA[4]] = aux;
        cc = aux;
    }

    if (cc == 0)
        memoria->VectorDeRegistros[8] = 0x1;
    else if(cc < 0)
        memoria->VectorDeRegistros[8] = 0x80000000;
    else
        memoria->VectorDeRegistros[8] = 0;
}

/*Función PUSH: Se ingresa un dato a la pila*/
void PUSH(Memoria *memoria, OperandosYFlags op)
{
    int posSP;

    //Se decrementa el SP
    memoria->VectorDeRegistros[6]--;
    //Se calcula la posición a la que se ingresa un dato
    posSP = op.segmento.ss + (memoria->VectorDeRegistros[6]&0xFFFF);

    //Si la posición pertenece al stack segment se almacena, sino hay stack overflow
    if (posSP > op.segmento.ss){
        memoria->RAM[posSP] = op.operandoA[0];
    }
    else
        memoria->error=2;
}

/*Función POP: Se obtiene un dato de la pila*/
void POP(Memoria *memoria, OperandosYFlags op)
{
    int aux = op.segmento.ss + (memoria->VectorDeRegistros[6]&0xFFFF) + 1;   //SP+1
    //Se averigua la posición de la cual se toma un valor
    int posSP = op.segmento.ss + (memoria->VectorDeRegistros[6]&0xFFFF);

    //Se averigua que no haya Stack Underflow
    if (aux >= op.segmento.finSS)
        memoria->error=3;
    else
    {
        //Se averigua el tipo de operando y se almacena el valor en el operando. Además se incrementa el SP
        if (op.operandoA[1] == 1)
        {
            //Si el operando A es de registro
            if (op.operandoA[2] == 0)
            {
                //Si el operando A es de 4 bytes
                memoria->VectorDeRegistros[op.operandoA[3]] = memoria->RAM[posSP];
                memoria->VectorDeRegistros[6]++;
            }
            else if (op.operandoA[2] == 1)
            {
                //Si utilizamos el 4to byte del operando A
                aux = memoria->RAM[posSP] & 0xFF;
                memoria->VectorDeRegistros[6]++;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFFFF00) + aux;
            }
            else if (op.operandoA[2] == 2)
            {
                //Si utilizamos el 3er byte del operando A
                aux = memoria->RAM[posSP] & 0xFF;
                memoria->VectorDeRegistros[6]++;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF00FF) + (aux * 0x100);
            }
            else if (op.operandoA[2] == 3)
            {
                //Si utilizamos los 2 ultimos bytes del operando A
                aux = memoria->RAM[posSP] & 0xFFFF;
                memoria->VectorDeRegistros[6]++;
                memoria->VectorDeRegistros[op.operandoA[3]] = (memoria->VectorDeRegistros[op.operandoA[3]] & 0xFFFF0000) + aux;
            }
        }
        else if (op.operandoA[1] == 2)
        {
            //Si el operando A es directo
            memoria->RAM[op.operandoA[4]] = memoria->RAM[posSP];
            memoria->VectorDeRegistros[6]++;
        }
        else if (op.operandoA[1] == 3)
        {
            memoria->RAM[op.operandoA[4]] = memoria->RAM[posSP];
            memoria->VectorDeRegistros[6]++;
        }
    }
}

/*Función CALL: Se llama a un procedimiento*/
void CALL(Memoria *memoria, OperandosYFlags op)
{
    int posSP;
    int posDestino;

    //Se decrementa el SP
    memoria->VectorDeRegistros[6]--;
    //Se averigua la posición relativa del SP al Stack segment
    posSP = op.segmento.ss + (memoria->VectorDeRegistros[6]&0xFFFF);

    //Si no hay stack overflow, se almacena el valor del IP al que retornar luego
    if (posSP > op.segmento.ss){
        memoria->RAM[posSP] = memoria->VectorDeRegistros[5]&0xFFFF;
        posDestino = op.operandoA[0];

        //Si el IP es válido se almacena, sino hay segmentation fault
        if (posDestino>=op.cantInstrucciones)
            memoria->error=1;
        else
            memoria->VectorDeRegistros[5] = (memoria->VectorDeRegistros[5]&0xFFFF0000) + posDestino;
    } else
        memoria->error=2;
}

/*Función RET: Se retorna de un procedimiento*/
void RET(Memoria *memoria, OperandosYFlags op)
{
    //Se aumenta el SP
    int aux = op.segmento.ss + (memoria->VectorDeRegistros[6]&0xFFFF) + 1;   //SP+1
    //Se calcula la posición relativa actual
    int posSP = op.segmento.ss + (memoria->VectorDeRegistros[6]&0xFFFF);

    //Si no hay stack underflow, se cambia el IP a la posición obtenida de la pila
    if (aux >= op.segmento.finSS)
        memoria->error=3;
    else
    {
        aux = memoria->RAM[posSP];
        memoria->VectorDeRegistros[6]++;
        if (aux < op.cantInstrucciones)
            memoria->VectorDeRegistros[5] = (memoria->VectorDeRegistros[5] & 0xFFFF0000) + aux;
        else
            memoria->error=1;
    }
}

/*Función STOP: detiene la ejecución del programa*/
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

//Función que acorde al código de operación, imprime un mnemónico*/
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

//Función que imprime un operando en consola
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

//Función encargada de formatear y llamar la impresión de operandos por consola
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
            printf("%8c%s], ",'[',aux);
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

//Función encargada de imprimir los registros actuales por consola
void imprimeEstadoRegistros(int vecR[], OperandosYFlags op)
{
    printf(" DS:%12d | SS:%12d | ES:%12d | CS:%12d |\n",vecR[0]&0xFFFF,vecR[1]&0xFFFF, vecR[2]&0xFFFF, vecR[3]&0xFFFF);
    printf(" HP:%12d | IP:%12d | SP:%12d | BP:%12d |\n",vecR[4],(vecR[5]-1)&0xFFFF,(vecR[6]&0xFFFF) + op.segmento.ss,(vecR[7]&0xFFFF)+op.segmento.ss );
    printf(" CC:%12d | AC:%12d |EAX:%12d |EBX:%12d |\n",vecR[8],vecR[9],vecR[10],vecR[11]);
    printf("ECX:%12d |EDX:%12d |EEX:%12d |EFX:%12d |\n",vecR[12],vecR[13],vecR[14],vecR[15]);
}

//Función encargada de la impresión en consola
void disassembler(Memoria *memoria, OperandosYFlags op, int ipEjecutado)
{
    int cod;
    int i;

    printf("Codigo:\n");

    for (i=ipEjecutado-2; i<ipEjecutado; i++)
    {
        if (i>=0)
        {
            printf(" [%04d]: ",i);
            printf("%02X ",(memoria->RAM[i] & 0xFF000000)/0x01000000);
            printf("%02X ",(memoria->RAM[i] & 0xFF0000)/0x00010000);
            printf("%02X ",(memoria->RAM[i] & 0xFF00)/0x00000100);
            printf("%02X ",(memoria->RAM[i] & 0xFF));
            printf("%4d: ",i+1);
            cod = decodificaCodigo(memoria->RAM[i]);
            decodificaOperandos(memoria,cod,memoria->RAM[i],&op);
            memoria->error=0;
            imprimeMnemonico(cod);
            imprimeOperandos(op,cod,memoria->RAM[i]);
            printf("\n");
        }
        else
        {
            printf(" [----]: -- -- -- --\n");
        }
    }

    i=ipEjecutado;
    printf(">[%04d]: ",i);
    printf("%02X ",(memoria->RAM[i] & 0xFF000000)/0x01000000);
    printf("%02X ",(memoria->RAM[i] & 0xFF0000)/0x00010000);
    printf("%02X ",(memoria->RAM[i] & 0xFF00)/0x00000100);
    printf("%02X ",(memoria->RAM[i] & 0xFF));
    printf("%4d: ",i+1);
    cod = decodificaCodigo(memoria->RAM[i]);
    decodificaOperandos(memoria,cod,memoria->RAM[i],&op);
    memoria->error=0;
    imprimeMnemonico(cod);
    imprimeOperandos(op,cod,memoria->RAM[i]);
    printf("\n");

    for (int i=ipEjecutado+1; i<ipEjecutado+3; i++)
    {
        printf(" [%04d]: ",i);
        printf("%02X ",(memoria->RAM[i] & 0xFF000000)/0x01000000);
        printf("%02X ",(memoria->RAM[i] & 0xFF0000)/0x00010000);
        printf("%02X ",(memoria->RAM[i] & 0xFF00)/0x00000100);
        printf("%02X ",(memoria->RAM[i] & 0xFF));
        if (i<op.segmento.ds && (i<op.cantInstrucciones))
        {
            printf("%4d: ",i+1);
            cod = decodificaCodigo(memoria->RAM[i]);
            decodificaOperandos(memoria,cod,memoria->RAM[i],&op);
            memoria->error=0;
            imprimeMnemonico(cod);
            imprimeOperandos(op,cod,memoria->RAM[i]);
        }
        printf("\n");

    }

    printf("Registros:\n");

    imprimeEstadoRegistros(memoria->VectorDeRegistros,op);
}

//Función encargada de cargar o escribir discos nuevos
void setDisco(int nro, Memoria *memoria,char nombreD[])
{
    FILE *arch;
    DiskHead header;
    int r1, r2;
    time_t hora;
    struct tm *t;
    char strHora[80];
    char auxTamSector;

    arch = fopen(nombreD,"r+");
    if (arch!=NULL){
        //Si el disco ya existe, se obtienen sus parámetros
        memoria->discos.info[nro].arch = arch;
        memoria->discos.info[nro].nroUnidad = nro;
        fseek(arch,33, SEEK_SET);
        fread(&memoria->discos.info[nro].cantCilindros,sizeof(unsigned char),1,arch);
        fread(&memoria->discos.info[nro].cantCabezas,sizeof(unsigned char),1,arch);
        fread(&memoria->discos.info[nro].cantSectores,sizeof(unsigned char),1,arch);
        memoria->discos.info[nro].tamSector = 0;

        fread(&auxTamSector,sizeof(unsigned char),1,arch);
        memoria->discos.info[nro].tamSector+= (auxTamSector<<24);

        fread(&auxTamSector,sizeof(unsigned char),1,arch);
        memoria->discos.info[nro].tamSector+= auxTamSector<<16;

        fread(&auxTamSector,sizeof(unsigned char),1,arch);
        memoria->discos.info[nro].tamSector+= (auxTamSector<<8);

        fread(&auxTamSector,sizeof(unsigned char),1,arch);
        memoria->discos.info[nro].tamSector+= (auxTamSector);

    }
    else
    {
        //Si el disco no existe, se generan los parámetros por defecto y se escribe
        int auxEscritura;
        memoria->discos.info[nro].arch = fopen(nombreD,"w+");
        if (memoria->discos.info[nro].arch!=NULL)
        {

            header.idTipo = 0x30444456;

            header.version = 0x01000000;

            srand (rand() % 256);
            r1 = rand() % RAND_MAX;
            r2 = rand() % RAND_MAX;
            header.GUID1 = (r1*0x100000000) + r2;
            srand (rand() % 150);
            r1 = rand() % RAND_MAX;
            r2 = rand() % RAND_MAX;
            header.GUID2 = (r1*0x100000000) + r2;

            hora = time(0);
            t = localtime(&hora);
            strftime(strHora, sizeof(strHora), "%Y%m%d%H%M%S", t);

            header.fechaCreacion = 0;
            header.fechaCreacion+= (strHora[6]&0xF)*10 + (strHora[7]&0xF);
            header.fechaCreacion<<=8;
            header.fechaCreacion += (strHora[4]&0xF)*10 + (strHora[5]&0xF);
            header.fechaCreacion<<=16;
            auxEscritura=0;
            auxEscritura+= (strHora[0]&0xF)*1000 + (strHora[1]&0xF)*100 + (strHora[2]&0xF)*10 + (strHora[3]&0xF);
            auxEscritura = ((auxEscritura&0xFF)<<8) + ((auxEscritura&0xFF00)>>8);
            header.fechaCreacion+=auxEscritura;

            header.horaCreacion = 0;
            header.horaCreacion+= (strHora[14]&0xF)*10 + (strHora[15]&0xF);
            header.horaCreacion<<=8;
            header.horaCreacion += (strHora[12]&0xF)*10 + (strHora[13]&0xF);
            header.horaCreacion<<=16;
            auxEscritura=0;
            auxEscritura+= (strHora[8]&0xF)*1000 + (strHora[9]&0xF)*100 + (strHora[10]&0xF)*10 + (strHora[11]&0xF);
            auxEscritura = ((auxEscritura&0xFF)<<8) + ((auxEscritura&0xFF00)>>8);
            header.horaCreacion+=auxEscritura;

            header.tipo = 1;
            header.cantCilindros = 128;
            header.cantCabezas = 128;
            header.cantSectores = 128;
            auxEscritura = 512;
            header.tamSector = ((auxEscritura&0xFF)<<24) + ((auxEscritura&0xFF00)<<8) + ((auxEscritura&0xFF0000)>>8) + ((auxEscritura&0xFF000000)>>24);

            for (int i=0; i<118; i++)
                header.relleno[i]=0;

            fwrite(&header,sizeof(DiskHead),1,memoria->discos.info[nro].arch);

            memoria->discos.info[nro].nroUnidad = nro;
            memoria->discos.info[nro].cantCilindros = header.cantCilindros;
            memoria->discos.info[nro].cantCabezas = header.cantCabezas;
            memoria->discos.info[nro].cantSectores = header.cantSectores;
            memoria->discos.info[nro].tamSector = auxEscritura;
        }
    }
}

/*
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
*/

//Función que se encarga de inicializar los discos en la memoria
void inicializaDiscos(Memoria *memoria, int argc,char *argv[]){
    memoria->discos.cant=0;
    for (int i=0; i<255; i++){
        memoria->discos.info[i].arch = NULL;
        memoria->discos.info[i].ultimoEstado = 0;
    }

    //Llama a la creación o lectura de discos a memoria
    for(int i=2; i<argc; i++){
        if( strstr(argv[i],"vdd") != NULL){
            setDisco(memoria->discos.cant,memoria,argv[i]);
            memoria->discos.cant++;
        }
    }
}

//Función que se encarga de inicializar los flags en memoria
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

//Función que se encarga de inicializar las funciones en un vector
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

//Función que verifica que el header sea de la MV2
int verificoHeader(Header header)
{
    int ret=0;

    // MV-2: 0x4D562D32
    // V.22: 0x562E3232

    if ((header.bloque[0] == 0x4D562D32) && (header.bloque[5] == 0x562E3232))
        ret = 1;

    return ret;
}

//Función que setea los segmentos relativamente a los demás en la memoria
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

//Función que decodifica el código de operación
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

//Función que decodifica y almacena partes de los operandos
//Sus valores se explican en funciones.h
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

            if (op->operandoB[2] == 0){
                //Registro de 4 bytes
                op->operandoB[0] = memoria->VectorDeRegistros[op->operandoB[3]];
            }
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

            op->operandoB[4] = (instruccion & 0xFFF) + op->segmento.ds;

            if (op->operandoB[4] >= op->segmento.es)
                memoria->error = 1;
            else
                op->operandoB[0] = memoria->RAM[op->operandoB[4]];

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

            //Segmento
            op->segmento.actualB = (memoria->VectorDeRegistros[op->operandoB[3]] / 0x10000) & 0xF;

            if (op->segmento.actualB == 0)  //DS
            {
                // [REG+OFFSET]
                op->operandoB[4] = op->segmento.ds + (memoria->VectorDeRegistros[op->operandoB[3]] & 0xFFFF) + op->operandoB[5];

                if ((op->operandoB[4] < op->segmento.ds) || (op->operandoB[4] >= op->segmento.es))
                    memoria->error = 1;
            }
            else if (op->segmento.actualB == 1)    //SS
            {
                // [REG+OFFSET]
                op->operandoB[4] = op->segmento.ss + (memoria->VectorDeRegistros[op->operandoB[3]] & 0xFFFF) + op->operandoB[5];

                if ((op->operandoB[4] < op->segmento.ss) || (op->operandoB[4] >= op->segmento.finSS))
                    memoria->error = 1;
            }
            else if (op->segmento.actualB == 2)    //ES
            {
                // [REG+OFFSET]
                op->operandoB[4] = op->segmento.es + (memoria->VectorDeRegistros[op->operandoB[3]] & 0xFFFF) + op->operandoB[5];

                if ((op->operandoB[4] < op->segmento.es) || (op->operandoB[4] >= op->segmento.ss))
                    memoria->error = 1;
            }
            else if (op->segmento.actualB == 3){    //CS

                // [REG+OFFSET]
                op->operandoB[4] = op->segmento.cs + (memoria->VectorDeRegistros[op->operandoB[3]] & 0xFFFF) + op->operandoB[5];

                if (op->operandoB[4] < op->segmento.cs || (op->operandoB[4] >= op->segmento.ds))
                    memoria->error = 1;
            }

            op->operandoB[0] = memoria->RAM[op->operandoB[4]];
        }

        //--------------------------------------------------------------------------------------------
        //Operando A

        if (memoria->error==0)
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

                op->operandoA[4] = op->segmento.ds + ((instruccion & 0xFFF000) / 0x1000) ;

                op->segmento.actualA = 0;

                if (op->operandoA[4] >= op->segmento.es)
                    memoria->error = 1;
                else
                    op->operandoA[0] = memoria->RAM[op->operandoA[4]];
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

                //Segmento
                op->segmento.actualA = (memoria->VectorDeRegistros[op->operandoA[3]] / 0x10000) & 0xF;

                if (op->segmento.actualA == 0)  //DS
                {
                    // [REG+OFFSET]
                    op->operandoA[4] = op->segmento.ds + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                    if ((op->operandoA[4] < op->segmento.ds) || (op->operandoA[4] >= op->segmento.es))
                        memoria->error = 1;
                }
                else if (op->segmento.actualA == 1)    //SS
                {
                    // [REG+OFFSET]
                    op->operandoA[4] = op->segmento.ss + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                    if ((op->operandoA[4] < op->segmento.ss) || (op->operandoA[4] >= op->segmento.finSS))
                            memoria->error = 1;
                }
                else if (op->segmento.actualA == 2)    //ES
                {
                    // [REG+OFFSET]
                    op->operandoA[4] = op->segmento.es + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                    if ((op->operandoA[4] < op->segmento.es) || (op->operandoA[4] >= op->segmento.ss))
                        memoria->error = 1;
                }
                else if (op->segmento.actualA == 3){    //CS

                    // [REG+OFFSET]
                    op->operandoA[4] = op->segmento.cs + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                    if (op->operandoA[4] < op->segmento.cs || (op->operandoA[4] >= op->segmento.ds))
                        memoria->error = 1;
                }

                op->operandoA[0] = memoria->RAM[op->operandoA[4]];
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
            op->operandoA[4] = op->segmento.ds + (instruccion & 0xFFFF) ;

            op->segmento.actualA = 0;

            if (op->operandoA[4]>=op->segmento.es)
                memoria->error = 1;
            else
                op->operandoA[0] = memoria->RAM[op->operandoA[4]];
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

            //Segmento
            op->segmento.actualA = (memoria->VectorDeRegistros[op->operandoA[3]] / 0x10000) & 0xF;

            if (op->segmento.actualA == 0)  //DS
            {
                // [REG+OFFSET]
                op->operandoA[4] = op->segmento.ds + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                if ((op->operandoA[4] < op->segmento.ds) || (op->operandoA[4] >= op->segmento.es))
                    memoria->error = 1;
            }
            else if (op->segmento.actualA == 1)    //SS
            {
                // [REG+OFFSET]
                op->operandoA[4] = op->segmento.ss + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                if ((op->operandoA[4] < op->segmento.ss) || (op->operandoA[4] >= op->segmento.finSS))
                    memoria->error = 1;
            }
            else if (op->segmento.actualA == 2)    //ES
            {
                // [REG+OFFSET]
                op->operandoA[4] = op->segmento.es + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                if ((op->operandoA[4] < op->segmento.es) || (op->operandoA[4] >= op->segmento.ss))
                    memoria->error = 1;
            }
            else if (op->segmento.actualA == 3){    //CS

                // [REG+OFFSET]
                op->operandoA[4] = op->segmento.cs + (memoria->VectorDeRegistros[op->operandoA[3]] & 0xFFFF) + op->operandoA[5];

                if (op->operandoA[4] < op->segmento.cs || (op->operandoA[4] >= op->segmento.ds))
                    memoria->error = 1;
            }

            op->operandoA[0] = memoria->RAM[op->operandoA[4]];
        }
    }
}

//Funcion que quita caracter (utilizada para quitar por ejemplo el "%" de %08 ingresado por consola
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

//Funcion que cuenta cantidad de caracteres utilizada para saber la cantidad de datos ingresados por consola
int cuentaChars(char cadena[], char caracter,int longitud)
{
    int cont=0;

    for (int i = 0; i < longitud; i++)
        if (cadena[i] == caracter)
            cont++;

    return cont;
}

//Inicializa los registros según la especificación
void inicializaRegistros(Memoria *mem,OperandosYFlags op)
{
    for (int i=8; i<16; i++)
        mem->VectorDeRegistros[i]=0;

    mem->VectorDeRegistros[4] = 0x00020000; //HP
    mem->VectorDeRegistros[5] = 0x00030000; //IP
    mem->VectorDeRegistros[6] = 0x00010000 + (((mem->VectorDeRegistros[1]) & 0xFFFF0000)/0x10000); //SP
    mem->VectorDeRegistros[7] = 0x00010000; //BP

}

//Inicializa los mensajes de error según la especificación
void inicializaMensajesError(Memoria *memoria){
    memoria->error=0;
    strcpy(memoria->msjError[1].detalle,"Segmentation Fault");
    strcpy(memoria->msjError[2].detalle,"Stack Overflow");
    strcpy(memoria->msjError[3].detalle,"Stack Underflow");

}
