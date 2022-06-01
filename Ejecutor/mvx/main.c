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

    //Inicializa los flags
    inicializaFlags(&op,argc,argv);
    //Inicializa los discos
    inicializaDiscos(&memoria,argc,argv);
    //Inicializa los mensajes de error
    inicializaMensajesError(&memoria);

    //Se genera una semilla para el random
    srand(time(0));

    //Se limpia la pantalla si hay un -c
    if (op.flags[1] == 1)
        system("cls");

    //Se abre el archivo .mv2 para lectura
    FILE *arch=fopen(argv[1],"rb");

    //Se inicia el vector de funciones
    iniciaVectorFunciones(vecF);

    //Se lee el header del archivo
    fread(&header,sizeof(Header),1,arch);

    //Variable auxiliar utilizada para saber el tamaño del CS
    int size[header.bloque[4]];

    //Si el header pertenece a la .mv2
    if (verificoHeader(header))
    {
        //Se inicializan los segmentos
        if (seteoSegmentos(&memoria,header,&op)){
            //Se inicializan los registros
            inicializaRegistros(&memoria,op);
            //Se lee el CS a memoria
            fread(&memoria,sizeof(size),1,arch);
            fclose(arch);

            //Se inicializa la cantidad de instrucciones
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

            //Se llama al SYSF en caso de un -b
            if (op.flags[2] == 1){
                cod = decodificaCodigo(0xF000000F);
                decodificaOperandos(&memoria,cod,0xF000000F,&op);
                vecF[cod](&memoria,op);
            }

            do{
                //En caso de salir del SYSF se continúa ejecutando el código
                ip=memoria.VectorDeRegistros[5] & 0xFFFF;
                memoria.VectorDeRegistros[5]++;
                cod = decodificaCodigo(memoria.RAM[ip]);
                decodificaOperandos(&memoria,cod,memoria.RAM[ip],&op);
                vecF[cod](&memoria,op);
                fflush(stdin);
            } while( (ip >= 0) && (ip<op.segmento.ds) && (ip<op.cantInstrucciones) && memoria.error==0 );

            //Si la ejecución tuvo un error, se muestra y termina la ejecución
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

