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

    inicializaFlags(&op,argc,argv);

    if (op.flags[1] == 1)
        system("cls");

    FILE *arch=fopen(argv[1],"rb");

    srand (getpid());   //Cambia la semilla en cada ejecución del programa para la instrucción random

    iniciaVectorFunciones(vecF);

    fread(&header,sizeof(Header),1,arch);

    int size[header.bloque[1]];

    if (verificoHeader(header))
    {
        fread(&memoria,sizeof(size),1,arch);

        fclose(arch);

        //Se setean IP y DS
        memoria.VectorDeRegistros[0] = header.bloque[1];
        memoria.VectorDeRegistros[5] = 0;

        if (op.flags[2] == 1){
            cod = decodificaCodigo(0xF000000F);
            decodificaOperandos(memoria,cod,0xF000000F,&op);
            vecF[cod](&memoria,op);
        }

        //Ejecución del programa
        while( (0 <= memoria.VectorDeRegistros[5]) && (memoria.VectorDeRegistros[5]<memoria.VectorDeRegistros[0]) ){
            ip = memoria.VectorDeRegistros[5]++;
            cod = decodificaCodigo(memoria.RAM[ip]);
            decodificaOperandos(memoria,cod,memoria.RAM[ip],&op);
            vecF[cod](&memoria,op);
        }
    }
    else{
        printf("El archivo recibido no pertenece a la MV-1 V.22");
        fclose(arch);
    }
    return 0;
}

