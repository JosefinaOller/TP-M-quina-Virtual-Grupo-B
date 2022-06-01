#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "memoria.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
    Memoria memoria;
    Header header;
    VectorFunciones vecF;
    OperandosYFlags op;
    int ip, cod;
    int i;

    inicializaFlags(&op,argc,argv);
    inicializaDiscos(&memoria,argc,argv);
    inicializaMensajesError(&memoria);

    srand(time(0));

    if (op.flags[1] == 1)
        system("cls");

    FILE *arch=fopen(argv[1],"rb");

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

            //Se calcula cantidad de instrucciones en el CS (sin constantes strings)
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

            do{
                ip=memoria.VectorDeRegistros[5] & 0xFFFF;
                memoria.VectorDeRegistros[5]++;
                cod = decodificaCodigo(memoria.RAM[ip]);
                decodificaOperandos(&memoria,cod,memoria.RAM[ip],&op);
                vecF[cod](&memoria,op);
                fflush(stdin);
                //srand (time(0) + rand());
            } while( (ip >= 0) && (ip<op.segmento.ds) && (ip<op.cantInstrucciones) && memoria.error==0 );

            if (memoria.error!=0){
                printf("\nError: %s\n",memoria.msjError[memoria.error].detalle);
                exit(-1);
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

