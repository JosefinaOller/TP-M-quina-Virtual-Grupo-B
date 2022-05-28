#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memoria.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    Memoria memoria;
    Header header;
    VectorFunciones vecF;
    OperandosYFlags op;
    int ip, cod;

    for(int i=0; i<3; i++)
        op.flags[i]=1;
    op.error=0;

    if (op.flags[1] == 1)
        system("cls");

    srand (getpid());   //Cambia la semilla en cada ejecución del programa para la instrucción random

    iniciaVectorFunciones(vecF);

    inicializaRegistros(&memoria,op);

    /*
    MOV BH, 2
    MOV BL, 5
    MOV [EAX+5], [EBX+5]
    */

    memoria.VectorDeRegistros[3] = 0x00C80000;//CS 0
    memoria.VectorDeRegistros[0] = 0x00C800C8;//DS 200
    memoria.VectorDeRegistros[2] = 0x00C80190;//ES 400
    memoria.VectorDeRegistros[1] = 0x00C80258;//SS 600

    op.segmento.ds = memoria.VectorDeRegistros[0]&0xFFFF;
    op.segmento.es = memoria.VectorDeRegistros[2]&0xFFFF;
    op.segmento.ss = memoria.VectorDeRegistros[1]&0xFFFF;
    op.segmento.finSS = (memoria.VectorDeRegistros[1]&0xFFFF) + (memoria.VectorDeRegistros[1]/0x10000);
    memoria.VectorDeRegistros[6] = 0x00010000 + ((op.segmento.finSS+1) & 0xFFFF); //SP

    memoria.VectorDeRegistros[10] = 0x00000005; //EAX
    memoria.VectorDeRegistros[11] = 0x00020005; //EBX

    memoria.RAM[210] = 97;
    memoria.RAM[211] = 98;
    memoria.RAM[212] = 99;
    memoria.RAM[213] = 100;
    memoria.RAM[214] = 0;

    memoria.RAM[410] = 98;
    memoria.RAM[411] = 99;
    memoria.RAM[412] = 67;
    memoria.RAM[413] = 68;
    memoria.RAM[414] = 0;

    int inst = 0xEF05A05B;
    //int inst = 0xCC05A05B;

    cod = decodificaCodigo(inst);
    decodificaOperandos(memoria,cod,inst,&op);
    vecF[cod](&memoria,op);
    //printf("Cod: %X\n",cod);
    //printf("OPb[0]: %d | OPa[0]: %d\n",op.operandoB[0],op.operandoA[0]);
    printf("cc: %d",memoria.VectorDeRegistros[8]);
    /*printf("smov : %c\n",memoria.RAM[210]);
    printf("smov : %c\n",memoria.RAM[211]);
    printf("smov : %c\n",memoria.RAM[212]);
    printf("smov : %c\n",memoria.RAM[213]);
    printf("smov : %d\n",memoria.RAM[214]);*/
    //printf("JMP: %d\n",memoria.VectorDeRegistros[5]);

    //-----------------------------------------
    /*
    Memoria memoria;
    Header header;
    VectorFunciones vecF;
    OperandosYFlags op;
    int ip, cod;

    inicializaFlags(&op,argc,argv);
    inicializaDiscos(&memoria,argc,argv);

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


            if (op.flags[2] == 1){
                cod = decodificaCodigo(0xF000000F);
                decodificaOperandos(memoria,cod,0xF000000F,&op);
                vecF[cod](&memoria,op);
            }

            //Ejecución del programa
            while( (0 <= memoria.VectorDeRegistros[5]) && (memoria.VectorDeRegistros[5]<op.segmento.ds) && op.error==0){
                ip = memoria.VectorDeRegistros[5]++;
                cod = decodificaCodigo(memoria.RAM[ip]);
                decodificaOperandos(memoria,cod,memoria.RAM[ip],&op);
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
*/
    return 0;
}

