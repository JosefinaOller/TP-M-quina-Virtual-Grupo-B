#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prototipos.h"
#include "parser.h"

int main(int argc, const char *argv[])
{

    char asmar[20],binario[20];
    int o=1,i;

    Mnemonico vecMnem[MNEMAX];
    cargaVecMnem(vecMnem); //Carga todos los mnemonicos con sus datos.
    argc=2;
    argv[0]="prueba.asm";
    argv[1]="prueba.bin";
    for(i=0;i<argc;i++){
        if(strstr(argv[i],".asm"))
            strcpy(asmar,argv[i]);
        else if(strstr(argv[i],".mv1"))
            strcpy(binario,argv[i]);
        else
            if (strstr (argv[i],"-o")) //El flag o, si esta activado, no se muestrà pero se genera el archivo binario.
             o=0;
    }
    if (argc>=2)
        traduce(asmar,vecMnem,o,binario);
    else
        printf("Faltaron paramentros\n");
    return 0;
}
void cargaVecMnem(Mnemonico vecMnem[]){ //Carga datos de mnenomicos

    //DOS OPERANDOS

    cargaMnemonico("MOV",0x0,2,&vecMnem[0]);
    cargaMnemonico("ADD",0x1,2,&vecMnem[1]);
    cargaMnemonico("SUB",0x2,2,&vecMnem[2]);
    cargaMnemonico("SWAP",0x3,2,&vecMnem[3]);
    cargaMnemonico("MUL",0x4,2,&vecMnem[4]);
    cargaMnemonico("DIV",0x5,2,&vecMnem[5]);
    cargaMnemonico("CMP",0x6,2,&vecMnem[6]);
    cargaMnemonico("SHL",0x7,2,&vecMnem[7]);
    cargaMnemonico("SHR",0x8,2,&vecMnem[8]);
    cargaMnemonico("AND",0x9,2,&vecMnem[9]);
    cargaMnemonico("OR",0xA,2,&vecMnem[10]);
    cargaMnemonico("XOR",0xB,2,&vecMnem[11]);

    //UN OPERANDO

    cargaMnemonico("SYS",0xF0,1,&vecMnem[12]);
    cargaMnemonico("JMP",0xF1,1,&vecMnem[13]);
    cargaMnemonico("JZ",0xF2,1,&vecMnem[14]);
    cargaMnemonico("JP",0xF3,1,&vecMnem[15]);
    cargaMnemonico("JN",0xF4,1,&vecMnem[16]);
    cargaMnemonico("JNZ",0xF5,1,&vecMnem[17]);
    cargaMnemonico("JNP",0xF6,1,&vecMnem[18]);
    cargaMnemonico("JNN",0xF7,1,&vecMnem[19]);
    cargaMnemonico("LDL",0xF8,1,&vecMnem[20]);
    cargaMnemonico("LDH",0xF9,1,&vecMnem[21]);
    cargaMnemonico("RND",0xFA,1,&vecMnem[22]);
    cargaMnemonico("NOT",0xFB,1,&vecMnem[23]);

    //SIN OPERANDOS

    cargaMnemonico("STOP",0xFF1,0,&vecMnem[24]);

}

void traduce(char nombAsm[],Mnemonico vecMnem[],int o, char binario[]){ //Funcion que procesa las instrucciones,las imprime y genera el archivo binario

    FILE *archESCRITURA; //para el archivo binario
    char header[5],reservado[10],listaComentarios[100][100];//para guardar comentarios
    Label rotulos[100];
    Linea codigo[500];
    int i,cantRotulos=0,error=0,conLin=0,nComentarios=0,indicelinea[100],inst;
    lecturaLabels(nombAsm,rotulos,&cantRotulos,&error); //para guardar los rotulos y su posicion
    if(error)
        printf("Error de archivo\n");
    else{
        for(i=0;i<100;i++)
            strcpy(listaComentarios[i],"");

        estructuraASM(nombAsm,codigo,&conLin,listaComentarios,&nComentarios,indicelinea); //procesa las instrucciones asm
        if(conLin>=0 || nComentarios>=0){
            //empiezo a generar el archivo binario
            archESCRITURA=fopen(binario,"wb");
            //la cabecera
            strcpy(header,"MV-1");
            fwrite(&header,sizeof(int),1,archESCRITURA);
            fwrite(&conLin,sizeof(int),1,archESCRITURA);
            strcpy(reservado,"");
            fwrite(&reservado,sizeof(int),1,archESCRITURA);
            fwrite(&reservado,sizeof(int),1,archESCRITURA);
            fwrite(&reservado,sizeof(int),1,archESCRITURA);
            strcpy(header,"V.22");
            fwrite(&header,sizeof(int),1,archESCRITURA);

            //codigo
            /*
            for(i=0;i<conLin;i++){
                if(strcmp(codigo.mnem[i],"")!=0){
                    inst=codificaInstruccion(&error); //a completar, hay que hacer la funcion de codificarInstruccion que arma la instruccion en un int
                    if(!error) //Si no hay error, grabo la instruccion codificada
                        fwrite(&inst,sizeof(int),1,archESCRITURA);
                }
                if(o){ //Si el flag de ocultamiento esta desactivado, muestro lineas


                }

            }*/



        }

        //Hay que poner lo de advertencia
    }
    fclose(archESCRITURA);
    if(error){ //Si hay error de traduccion, borro el archivo binario
        printf("Error de traduccion\n");
        remove(binario);
    }
}

void estructuraASM(char nombAsm[],Linea codigo[],int *conLin,char listaComentarios[][100],int *nComentarios,int indicelinea[]){

    FILE *arch;
    char linea[500],rotulo[10],mnem[5],argA[16],argB[16],com[100];
    arch=fopen(nombAsm,"rt");
    if(arch!=NULL){
        *conLin=0;
        *nComentarios=0;
        while(fgets(linea,500,arch)!=NULL){
            strcpy(rotulo,"");strcpy(mnem,"");strcpy(argA,"");strcpy(argB,"");strcpy(com,""); //los inicializo
            procesarLinea(linea,rotulo,mnem,argA,argB,com);
            cargaLinea(rotulo,mnem,argA,argB,com,&codigo[*conLin]);
            if(strcmp(com,"")!=0){
                strcpy(listaComentarios[*nComentarios],linea);
                indicelinea[(*nComentarios)++]=*conLin;
            }
            (*conLin)++;
        }
    }
    else
        printf("%s","ERROR DE ARCHIVO");
    fclose(arch);
}
void procesarLinea(char linea[],char rotulo[],char mnem[],char argA[],char argB[],char com[]){ //Separa todos los terminos de la instruccion

    //Utilizamos el parser de Franco, gracias por tanto crack!!

    char **parsed = parseline(linea);

    strcpy(rotulo,parsed[0] ? parsed[0] : "");
    strcpy(mnem,parsed[1] ? parsed[1] : "");
    strcpy(argA,parsed[2] ? parsed[2] : "");
    strcpy(argB,parsed[3] ? parsed[3] : "");
    strcpy(com,parsed[4] ? parsed[4] : "");

    //para probar
    /*
    char **parsed = parseline("test: MOV AX, 123 ;inicializa AX");

    printf("    LABEL: %s\n", parsed[0] ? parsed[0] : "");
    printf(" MNEMONIC: %s\n", parsed[1] ? parsed[1] : "");
    printf("OPERAND 1: %s\n", parsed[2] ? parsed[2] : "");
    printf("OPERAND 2: %s\n", parsed[3] ? parsed[3] : "");
    printf("  COMMENT: %s\n", parsed[4] ? parsed[4] : "");
    */

    freeline(parsed);
}
void lecturaLabels(char *archivo,Label rotulos[],int *cantRotulos,int *error){

    FILE *arch=fopen(archivo,"rt");
    char linea[500],rotulo[16];
    int nrolinea=0;
    if(arch!=NULL){
        while(fgets(linea,500,arch)!=NULL){
           char **parsed = parseline(linea);
           strcpy(rotulo,parsed[0] ? parsed[0] : "");
           freeline(parsed);
           if(strcmp(rotulo,"")!=0){ //Hay rotulo
                strcpy(rotulos[*cantRotulos].etiqueta,rotulo);
                rotulos[*cantRotulos].codigo=nrolinea;
                (*cantRotulos)++;
           }
           nrolinea++;
        }
    }
    else{
        printf("Error de archivo\n");
        *error=1;
    }
    fclose(arch);
}
int busquedaLabel(Label rotulos[],char etiqueta[],int cantRotulos,int *error){ //Para buscar la posicion del rotulo
    int i=0;
    while(i<cantRotulos && strcmp(etiqueta,rotulos[i].etiqueta)!=0)
        i++;
    if (i<cantRotulos)
        return rotulos[i].codigo;
    else{
        *error=1;
        return -1;
    }
}
