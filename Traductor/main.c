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
        printf("traduce"); //ahi iria la traduccion
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
    Linea codigo[500];
    estructuraASM(); //procesa las instrucciones, COMPLETAR PARAMETROS

    //el proceso para imprimir y generar el archivo binario

}

void estructuraASM(char nombAsm[],Mnemonico vecMnem[],int o, char binario[]){

    FILE *arch;
    char linea[500],rotulo[16],mnem[10],argA[16],argB[16],com[100];
    arch=fopen(nombAsm,"r");
    if(arch!=NULL){
        while(fgets(linea,500,arch)!=NULL){
            strcpy(rotulo,"");strcpy(mnem,"");strcpy(argA,"");strcpy(argB,"");strcpy(com,""); //los inicializo
            procesarLinea(linea,rotulo,mnem,argA,argB,com);

            //hay que procesar routlo, todo!
        }
    }
    else
        printf("%s","ERROR DE ARCHIVO");
    fclose(arch);
}
void procesarLinea(char linea[],char rotulo[],char mnem[],char argA,char argB,char com[]){ //Separa todos los terminos de la instruccion

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

int buscaString(char x[],char lista[][100],int n){ //Es para buscar la posicion de rotulo
    int i=0;
    char aux[30];
    strcpy(aux, toupper(x));
    while(i<n && strcmp(aux,lista[i])!=0)
        i++;
    if(i==n || n==0)
        return -1;
    else
        return i;

}
