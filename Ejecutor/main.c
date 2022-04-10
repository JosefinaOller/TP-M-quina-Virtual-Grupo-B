#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memoria.h"
#include "funciones.h"

typedef void (*VectorFunciones[4096])(Memoria*,Operandos);

//instruccion  memoria->RAM[memoria->VectorRegistros[4]]
//operando 1   memoria->RAM[memoria->VectorRegistros[4]+1]
//operando 2   memoria->RAM[memoria->VectorRegistros[4]+2]

int main(int argc, char *argv[])
{

    Memoria memoria;
    Header header;
    VectorFunciones vecF;
    Operandos op;
    int ip, cod;

    //FILE *arch=fopen("","rb");
    FILE *arch=fopen(argv[1],"rb");

    srand (getpid());   //Cambia la semilla en cada ejecución del programa para la instrucción random

    vecF[0x00]=&MOV;
    /*
    vecF[0x01]=&ADD;
    vecF[0x02]=&SUB; */
    vecF[0x03]=&SWAP; /*
    vecF[0x04]=&MUL;
    vecF[0x05]=&DIV;
    vecF[0x06]=&CMP;
    vecF[0x07]=&SHL;
    vecF[0x08]=&SHR; */
    vecF[0x09]=&AND;
    vecF[0x0A]=&OR;
    vecF[0x0B]=&XOR; /*

    vecF[0xF0]=&SYS;
    vecF[0xF1]=&JMP;
    vecF[0xF2]=&JZ;
    vecF[0xF3]=&JP;
    vecF[0xF4]=&JN;
    vecF[0xF5]=&JNZ;
    vecF[0xF6]=&JNP;
    vecF[0xF7]=&JNN;
    vecF[0xF8]=&LDL; */
    vecF[0xF9]=&LDH; /*
    vecF[0xFA]=&RND; */
    vecF[0xFB]=&NOT; /*

    vecF[0xFF1]=&STOP;

    */


    fread(&header,sizeof(Header),1,arch);

    //Verificacion del header
    //if (verificoHeader(header)){
    if (1)
    {

        //fread(&memoria,sizeof(header[1]),1,arch);
        fread(&memoria,sizeof(Memoria),1,arch);

        fclose(arch);

        system("cls");

        ip = memoria.VectorDeRegistros[5]++;

        //cod = decodificaCodigo(memoria.RAM[ip]);

        memoria.VectorDeRegistros[10] = 15;
        memoria.VectorDeRegistros[11] = 20;
        memoria.VectorDeRegistros[0] = 10;

        int aux = 0x0500A00B;
        cod = decodificaCodigo(aux); //Esta linea es para probar entrar a las funciones, despues BORRAR y usar la de arriba
        decodificaOperandos(memoria,cod,aux,&op);

        printf("Operando A: %d | Tipo A: %d | RegA: %d \n",op.operandoA[0],op.operandoA[1],op.operandoA[2]);
        printf("Operando B: %d | Tipo B: %d | RegB: %d \n",op.operandoB[0],op.operandoB[1],op.operandoB[2]);

        printf("AX: %d\n", memoria.VectorDeRegistros[10]);

        vecF[cod](&memoria, op);

        printf("AX: %d\n", memoria.VectorDeRegistros[10]);

        /* while( (0 <= memoria.VectorDeRegistros[5]) && (memoria.VectorDeRegistros[5]<memoria.VectorDeRegistros[0]) ){
             //Aca van las lineas de arriba
             //Cada vez que se incrementa el ip se settea el op!!
         }*/
    }
    else
        printf("El archivo recibido no pertenece a la MV-1 V.22");
    return 0;
}

