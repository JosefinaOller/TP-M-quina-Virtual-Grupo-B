#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memoria.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    /*Memoria memoria;
    Header header;
    VectorFunciones vecF;
    OperandosYFlags op;
    int ip, cod;

    inicializaFlags(&op,argc,argv);
    inicializaDiscos(&memoria,argc,argv);

    for(int i=0; i<3; i++)
        op.flags[i]=1;
    op.error=0;

    if (op.flags[1] == 1)
        system("cls");

    srand (getpid());   //Cambia la semilla en cada ejecución del programa para la instrucción random

    iniciaVectorFunciones(vecF);

    inicializaRegistros(&memoria,op);


    memoria.VectorDeRegistros[3] = 0x00C80000;//CS 0
    memoria.VectorDeRegistros[0] = 0x03E800C8;//DS 200
    memoria.VectorDeRegistros[2] = 0x03E807D0;//ES 1200
    memoria.VectorDeRegistros[1] = 0x03E80BB8;//SS 2200

    op.segmento.ds = memoria.VectorDeRegistros[0]&0xFFFF;
    op.segmento.es = memoria.VectorDeRegistros[2]&0xFFFF;
    op.segmento.ss = memoria.VectorDeRegistros[1]&0xFFFF;
    op.segmento.finSS = (memoria.VectorDeRegistros[1]&0xFFFF) + (memoria.VectorDeRegistros[1]/0x10000);
    memoria.VectorDeRegistros[6] = 0x00010000 + ((op.segmento.finSS+1) & 0xFFFF); //SP

    memoria.VectorDeRegistros[10] = 0x00000001; //EAX
    memoria.VectorDeRegistros[11] = 0x0000000A; //EBX
    memoria.VectorDeRegistros[12] = 0x00020000; //ECX
    memoria.VectorDeRegistros[13] = 0x0000000A; //EDX


    memoria.RAM[210] = 100;
    memoria.RAM[211] = 100;
    memoria.RAM[212] = 100;
    memoria.RAM[213] = 100;
    memoria.RAM[214] = 0;

    int inst = 0xF000000D;

    cod = decodificaCodigo(inst);
    decodificaOperandos(memoria,cod,inst,&op);
    vecF[cod](&memoria,op);

*/


    /*printf("CH: %d\n",(memoria.VectorDeRegistros[12]&0xFF00)/0x100);
    printf("CL: %d\n",memoria.VectorDeRegistros[12]&0xFF);
    printf("DH: %d\n",(memoria.VectorDeRegistros[13]&0xFF00)/0x100);
    printf("AH: %d\n",(memoria.VectorDeRegistros[10]&0xFF00)/0x100);*/
   /* printf("Cod: %X\n",cod);
    printf("OPb[0]: %d | OPa[0]: %d\n",op.operandoB[0],op.operandoA[0]);
    printf("cc: %d",memoria.VectorDeRegistros[8]);
    printf("smov : %c\n",memoria.RAM[210]);
    printf("smov : %c\n",memoria.RAM[211]);
    printf("smov : %c\n",memoria.RAM[212]);
    printf("smov : %c\n",memoria.RAM[213]);
    printf("smov : %d\n",memoria.RAM[214]);
    printf("JMP: %d\n",memoria.VectorDeRegistros[5]);*/

    //-----------------------------------------

    Memoria memoria;
    Header header;
    VectorFunciones vecF;
    OperandosYFlags op;
    int ip, cod;
    int i;

    inicializaFlags(&op,argc,argv);
    inicializaDiscos(&memoria,argc,argv);
    memoria.error=0;

    if (op.flags[1] == 1)
        system("cls");

    FILE *arch=fopen(argv[1],"rb");

    srand (getpid());   //Cambia la semilla en cada ejecución del programa para la instrucción random

    iniciaVectorFunciones(vecF);

    fread(&header,sizeof(Header),1,arch);

    int size[header.bloque[4]];

    if (verificoHeader(header))
    {
        if (seteoSegmentos(&memoria,header,&op)){
            inicializaRegistros(&memoria,op);
            fread(&memoria,sizeof(size),1,arch);
            fclose(arch);

            op.cantInstrucciones=0;
            i=0;
            while (i<op.segmento.ds && op.cantInstrucciones==0){
                if (((memoria.RAM[(op.segmento.cs&0xFFFF)+i] & 0xFFFFF000) / 0x1000) == 0){
                    op.cantInstrucciones = i;
                } else if (i==op.segmento.ds-1)
                    op.cantInstrucciones = op.segmento.ds;
                i++;
            }

            if (op.flags[2] == 1){
                cod = decodificaCodigo(0xF000000F);
                decodificaOperandos(&memoria,cod,0xF000000F,&op);
                vecF[cod](&memoria,op);
            }

            //Ejecución del programa
            while( (0 <= (memoria.VectorDeRegistros[5]&0xFFFF)) && ((memoria.VectorDeRegistros[5]&0xFFFF)<op.segmento.ds)
                  && ((memoria.VectorDeRegistros[5]&0xFFFF)<op.cantInstrucciones) && memoria.error==0){

                ip = memoria.VectorDeRegistros[5]++;
                cod = decodificaCodigo(memoria.RAM[ip]);
                decodificaOperandos(&memoria,cod,memoria.RAM[ip],&op);
                vecF[cod](&memoria,op);
            }

        } else{
            printf("El proceso no puede ser cargado por memoria insuficiente.");
            fclose(arch);
        }
    }
    else{
        printf("El formato del archivo recibido no pertenece a la MV-2 V.22 (x.mv2).");
        fclose(arch);
    }

    return 0;
}

