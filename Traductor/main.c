#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prototipos.h"
#include "parser.h"

void cargaVecMnem(Mnemonico[]);
void traduce(char[], Mnemonico[], int, char[]);
void estructuraASM(char[], Linea[], int*, char[][100], int*, int[],int*,int*,int*);
void procesarLinea(char[], char[], char[], char[], char[], char[], char[], char[]);
void lecturaLabels(char*, Label[], int*, int*,char[],int *);
int busquedaLabel(Label[], char[], int, int*);
int codificaInstruccion(Linea, Mnemonico[], Label[], int, int*, int*, int*);
int tipoOperando(char[]);
void eliminaCorchetes(char[]);
int anyToInt(char*, char**);
int AEntero(char[]);
void truncamiento(int, int*, int*);
void salida(Linea, int, int, int, int);
void mayuscula(char[]);
void tratamiento_especial(char [],Label[],int,int *,char[]);


int main(int argc, const char *argv[]){
    char asmar[20],binario[20];
    int o=1,i;
    Mnemonico vecMnem[MNEMAX];
    cargaVecMnem(vecMnem); //Carga todos los mnemonicos con sus datos.
    argc=2;
    argv[0]="1.asm";
    argv[1]="1.bin";
    for(i=1;i<argc;i++){
        if(strstr(argv[i],".asm"))
            strcpy(asmar,argv[i]);
        else if(strstr(argv[i],".mv2"))
            strcpy(binario,argv[i]);
        else if (strstr (argv[i],"-o")) //El flag o, si esta activado, no se muestrà pero se genera el archivo binario.
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

    //SEGUNDA PARTE

    cargaMnemonico("SLEN",0xC,2,&vecMnem[25]);
    cargaMnemonico("SMOV",0xD,2,&vecMnem[26]);
    cargaMnemonico("SCMP",0xE,2,&vecMnem[27]);

    //PILA

    cargaMnemonico("PUSH",0xFC,1,&vecMnem[28]);
    cargaMnemonico("POP",0xFD,1,&vecMnem[29]);
    cargaMnemonico("CALL",0xFE,1,&vecMnem[30]);
    cargaMnemonico("RET",0xFF0,0,&vecMnem[31]);

}

void traduce(char nombAsm[],Mnemonico vecMnem[],int o, char binario[]){ //Funcion que procesa las instrucciones,las imprime y genera el archivo binario

    FILE *archESCRITURA; //para el archivo binario
    char listaComentarios[100][100],strings[100];
    Label rotulos[100];
    Linea codigo[500];
    int i,cantRotulos=0,error=0,conLin=0,nComentarios=0,indicelinea[100],inst, wrgA, wrgB,kcom=0,nroLinea,data=1024,extra=1024,stack=1024;
    lecturaLabels(nombAsm,rotulos,&cantRotulos,&error,strings,&nroLinea); //para guardar los rotulos y su posicion, tambien para guardar las constantes.
    if(error)
        printf("Error de archivo\n");
    else{
        for(i=0;i<100;i++)
            strcpy(listaComentarios[i],"");

        estructuraASM(nombAsm,codigo,&conLin,listaComentarios,&nComentarios,indicelinea,&data,&extra,&stack); //procesa las instrucciones asm
        if((data+extra+stack) <= (8192 - nroLinea)){ //Calculo de memoria
            if(conLin>=0 || nComentarios>=0){
                //empiezo a generar el archivo binario
                archESCRITURA=fopen(binario,"wb");
                //la cabecera
                int header=0x4D562D31;
                fwrite(&header,sizeof(int),1,archESCRITURA); //MV-2
                fwrite(&data,sizeof(int),1,archESCRITURA); //DS
                fwrite(&stack,sizeof(int),1,archESCRITURA); //SS
                fwrite(&extra,sizeof(int),1,archESCRITURA); //ES
                fwrite(&nroLinea,sizeof(int),1,archESCRITURA); //CS
                header=0x562E3232;
                fwrite(&header,sizeof(int),1,archESCRITURA); //V.22

                //codigo

                for(i=0;i<conLin;i++){
                    wrgA = wrgB = 0;//Advertencias
                    if(strcmp(codigo[i].mnem,"")!=0){
                        inst=codificaInstruccion(codigo[i], vecMnem, rotulos, cantRotulos, &error, &wrgA, &wrgB); //codificaInstruccion arma la instrucción en un int
                        if(!error) //Si no hay error, grabo la instruccion codificada
                            fwrite(&inst,sizeof(int),1,archESCRITURA);
                    }
                    if(o){
                       while(i==indicelinea[kcom] && strcmp(listaComentarios[kcom],"")!=0){
                            printf("%3s\n",strtok(listaComentarios[kcom++],"\n"));
                       }
                       salida(codigo[i], i, inst, wrgA, wrgB);
                    } //Si el flag de ocultamiento esta desactivado, muestro lineas

                }
                int cantStrings = nroLinea - conLin; //CONTROLAR
                for(i=0;i<cantStrings;i++){
                    fwrite(&strings[i],sizeof(int),1,archESCRITURA);
                }
                while(i==indicelinea[kcom] && strcmp(listaComentarios[kcom],"")!=0){
                    printf("%3s\n",strtok(listaComentarios[kcom++],"\n"));
                }
            }
        }
        else{
            error=1;
            printf("Falta de memoria\n");
        }
    }
    fclose(archESCRITURA);
    if(error){ //Si hay error de traduccion, borro el archivo binario
        printf("Error de traduccion\n");
        remove(binario);
    }
}

void estructuraASM(char nombAsm[],Linea codigo[],int *conLin,char listaComentarios[][100],int *nComentarios,int indicelinea[],int *data,int *extra,int *stack){

    FILE *arch;
    char linea[500],rotulo[10],mnem[5],argA[16],argB[16],com[100],seg[5],size[4];
    arch=fopen(nombAsm,"rt");
    if(arch!=NULL){
        *conLin=0;
        *nComentarios=0;
        while(fgets(linea,500,arch)!=NULL){
            strcpy(rotulo,"");strcpy(mnem,"");strcpy(argA,"");strcpy(argB,"");strcpy(com,""); //los inicializo
            strcpy(seg,"");strcpy(size,"");
            procesarLinea(linea,rotulo,mnem,argA,argB,com,seg,size);
            if(strcpy(seg,"")!=0){ //hay segmento
                if(strcpy(seg,"DATA")){
                    *data=atoi(size);
                }
                else{
                    if(strcpy(seg,"EXTRA")){
                        *extra=atoi(size);
                    }
                    else{
                        if(strcpy(seg,"STACK")){
                            *stack=atoi(size);
                        }
                    }
                }
            }
            else{
                cargaLinea(rotulo,mnem,argA,argB,com,&codigo[*conLin]);
                if(strcmp(com,"")!=0 && strcmp(rotulo,"")==0  && strcmp(mnem,"")==0 && strcmp(argA,"")==0 && strcmp(argB,"")==0){
                    strcpy(listaComentarios[*nComentarios],linea);
                    indicelinea[(*nComentarios)++]=*conLin;
                }
                if(strcmp(rotulo,"")!=0  || strcmp(mnem,"")!=0 || strcmp(argA,"")!=0 || strcmp(argB,"")!=0)
                    (*conLin)++;
                }

        } //while archivo
    }
    else
        printf("%s","ERROR DE ARCHIVO");
    fclose(arch);
}
void procesarLinea(char linea[],char rotulo[],char mnem[],char argA[],char argB[],char com[],char seg[],char size[]){ //Separa todos los terminos de la instruccion

    //Utilizamos el parser de Franco, gracias por tanto crack!!

    char **parsed = parseline(linea);

    strcpy(rotulo,parsed[0] ? parsed[0] : "");
    strcpy(mnem,parsed[1] ? parsed[1] : "");
    strcpy(argA,parsed[2] ? parsed[2] : "");
    strcpy(argB,parsed[3] ? parsed[3] : "");
    strcpy(com,parsed[4] ? parsed[4] : "");
    strcpy(seg,parsed[5] ? parsed[5] : "");
    strcpy(size,parsed[6] ? parsed[6] : "");

    freeline(parsed);
}
void lecturaLabels(char *archivo,Label rotulos[],int *cantRotulos,int *error,char strings[],int *nroLinea){

    FILE *arch=fopen(archivo,"rt");
    char linea[500],rotulo[16],mnem[5],constante[10],constante_valor[50];
    if(arch!=NULL){
        while(fgets(linea,500,arch)!=NULL){
           char **parsed = parseline(linea);
           strcpy(rotulo,parsed[0] ? parsed[0] : ""); //Rotulo
           strcpy(mnem,parsed[1] ? parsed[1] : ""); //Mnemonico, es para ver si existe para sumar la cantidad de lineas
           freeline(parsed);
           if(strcmp(rotulo,"")!=0){ //Hay rotulo
                mayuscula(rotulo);
                strcpy(rotulos[*cantRotulos].etiqueta,rotulo);
                rotulos[*cantRotulos].codigo=*nroLinea;
                (*cantRotulos)++;
                (*nroLinea)++;
           }
           else{
            if(strcmp(mnem,"")!=0)
                (*nroLinea)++;
           }
        } //while
    fclose(arch);
    while(fgets(linea,500,arch)!=NULL){ //vuelvo a leer para las constantes

           char **parsed = parseline(linea);
           strcpy(constante,parsed[7] ? parsed[7] : ""); //Constante
           strcpy(constante_valor,parsed[8] ? parsed[8] : ""); //Valor del constante
           freeline(parsed);

           if(strcmp(constante,"")!=0){ //Hay constante
                mayuscula(constante);
                if(busquedaLabel(rotulos,constante,*cantRotulos,error)==-1){ //No hau duplicado
                        strcpy(rotulos[*cantRotulos].etiqueta,constante);
                        switch(constante_valor[0]){
                        case '#':
                            sscanf(constante_valor+1,"%d",&(rotulos[*cantRotulos].codigo));
                            break;
                        case '@':
                            sscanf(constante_valor+1,"%o",&(rotulos[*cantRotulos].codigo));
                            break;
                        case '%':
                            sscanf(constante_valor+1,"%x",&(rotulos[*cantRotulos].codigo));
                            break;
                        case '0' ... '9':
                            rotulos[*cantRotulos].codigo=atoi(constante_valor);
                            break;
                        case '-': //VER
                            rotulos[*cantRotulos].codigo=atoi(constante_valor);
                            break;
                        case '\'': //VER
                            sscanf(constante_valor+1,"%d",&(rotulos[*cantRotulos].codigo));
                            break;
                        case '"': //Tratamiento especial
                            tratamiento_especial(constante_valor,rotulos,*cantRotulos,&(*nroLinea),strings);
                            break;
                    }
                    (*cantRotulos)++;
             } //if no hay duplicado
             else{
                printf("Símbolo duplicado\n");
                *error=1;
             }
           }
        }
    } //el archivo es null
    else{
        printf("Error de archivo\n");
        *error=1;
    }
    fclose(arch);

} //fin
void tratamiento_especial(char constante_valor[],Label rotulos[],int cantRotulos,int *nroLinea,char strings[]){
    int i=1;
    while(constante_valor[i]!='"'){
        rotulos[cantRotulos].codigo=(*nroLinea);
        strings[i-1]=constante_valor[i];
        (*nroLinea)++;
        i++;
    }
    strings[i-1]='\0';
}
int busquedaLabel(Label rotulos[],char etiqueta[],int cantRotulos,int *error){ //Para buscar la posicion del rotulo
    int i=0;
    while(i<cantRotulos && strcmp(etiqueta,rotulos[i].etiqueta)!=0)
        i++;
    if (i<cantRotulos)
        return rotulos[i].codigo;
    else{
        *error = 1;
        return -1;
    }
}
int codificaInstruccion(Linea codigo, Mnemonico vecMnem[], Label rotulos[], int cantRotulos, int *error, int *wrgA, int *wrgB){
    int i,valorReg,off,inst = 0;//Inicializo instrucción
    size_t length;
    Mnemonico mnem;
    char *out = NULL;
    if (vecMnem!=NULL)
        length = 25;//(sizeof(vecMnem)/sizeof(vecMnem[0]));
    if (strcmp(codigo.mnem, "") != 0){
        for (i = 0; i < strlen(codigo.mnem); i++)
            codigo.mnem[i] = toupper(codigo.mnem[i]);
        i = 0;
        while(i<length && strcmp(codigo.mnem, vecMnem[i].etiqueta) != 0)
            i++;
        if (i == length){
            printf("Error de instruccion\n");
            *(error) = -2;
            inst = 0xFFFFFFFE;
        }
        else{
            mnem = vecMnem[i];
            if (mnem.cantOp == 2){
                if (strcmp(codigo.argA, "") != 0 && strcmp(codigo.argB, "") != 0){
                    int topA = tipoOperando(codigo.argA);
                    int topB = tipoOperando(codigo.argB);
                    int vopA, vopB;
                    switch (topA){
                    case 0:
                        vopA = (codigo.argA[0] == '\'')? codigo.argA[1]: anyToInt(codigo.argA, &out);
                        truncamiento(2, &vopA, wrgA);
                        break;
                    case 1:
                        vopA = AEntero(codigo.argA);
                        break;
                    case 2:
                        eliminaCorchetes(codigo.argA);
                        vopA = (codigo.argA[0] == '\'')? codigo.argA[1]: anyToInt(codigo.argA, &out);
                        break;
                    case 3: //indirecto
                        valorReg=0;off=0;
                        eliminaCorchetes(codigo.argA);
                        operandoIndirecto(codigo.argA,&valorReg,&off,rotulos,cantRotulos,&error);
                        vopA= valorReg | (off<<16);
                        break;
                    }
                    switch (topB){
                    case 0:
                        vopB = (codigo.argB[0] == '\'')? codigo.argB[1]: anyToInt(codigo.argB, &out);
                        truncamiento(2, &vopB, wrgB);
                        break;
                    case 1:
                        vopB = AEntero(codigo.argB);
                        break;
                    case 2:
                        eliminaCorchetes(codigo.argB);
                        vopB = (codigo.argB[0] == '\'')? codigo.argB[1]: anyToInt(codigo.argB, &out);
                        break;
                    case 3:
                        valorReg=0;off=0;
                        eliminaCorchetes(codigo.argB);
                        operandoIndirecto(codigo.argB,&valorReg,&off,rotulos,cantRotulos,&error);
                        vopB= valorReg | (off<<4);
                        break;
                    }
                    inst = (mnem.codigo << 28) | ((topA << 26) & 0x0C000000) | ((topB << 24) & 0x03000000) | ((vopA << 12) & 0x00FFF000) | (vopB & 0x00000FFF);
                }else{
                    printf("Error de sintaxis\n");
                    *(error) = -1;
                    inst = 0xFFFFFFFF;
                }
            }else if (mnem.cantOp == 1){
                int j = 0, top, vop;
                if (strcmp(codigo.argA, "") != 0 && strcmp(codigo.argB, "") == 0){
                    while (j<cantRotulos && strcmp(codigo.argA, rotulos[j].etiqueta) != 0)
                        j++;
                    if (j<cantRotulos){
                        top = 0;//inmediato
                        vop = rotulos[j].codigo;
                        truncamiento(1, &vop, wrgA);
                        inst = 0xF0000000 | ((mnem.codigo << 24) & 0x0F000000) | ((top << 22) & 0x00C00000) | (vop & 0x0000FFFF);
                    }
                    else{
                        top = tipoOperando(codigo.argA);
                        switch (top){
                        case 0:
                            vop = (codigo.argA[0] == '\'')? codigo.argA[1]: anyToInt(codigo.argA, &out);
                            truncamiento(1, &vop, wrgA);
                            inst = 0xF0000000 | ((mnem.codigo << 24) & 0x0F000000) | ((top << 22) & 0x00C00000) | (vop & 0x0000FFFF);
                            break;
                        case 1:
                            vop = AEntero(codigo.argA);
                            if (vop != 0xFFF)
                                inst = 0xF0000000 | ((mnem.codigo << 24) & 0x0F000000) | ((top << 22) & 0x00C00000) | (vop & 0x0000FFFF);
                            else{
                                printf("No se encuentra el rotulo\n");
                                *(error) = 0xFFF;
                                inst = 0xF0000000 | (vop & 0x0000FFFF);
                            }
                            break;
                        case 2:
                            eliminaCorchetes(codigo.argA);
                            vop = (codigo.argA[0] == '\'')? codigo.argA[1]: anyToInt(codigo.argA, &out);
                            inst = 0xF0000000 | ((mnem.codigo << 24) & 0x0F000000) | ((top << 22) & 0x00C00000) | (vop & 0x0000FFFF);
                            break;

                        case 3:
                            valorReg=0;off=0;
                            eliminaCorchetes(codigo.argA);
                            operandoIndirecto(codigo.argA,&valorReg,&off,rotulos,cantRotulos,&error);
                            vop= valorReg | (off<<4);
                            break;
                        }
                    }
                }else{
                    printf("Error de sintaxis\n");
                    *(error) = -1;
                    inst = 0xFFFFFFFF;
                }
            }else
                inst = 0xFF100000;
        }
    }else{
        printf("Error de sintaxis\n");
        *(error) = -1;
        inst=0xFFFFFFFF;
    }
    return inst;
}
int tipoOperando(char vop[]){
    if (isdigit(vop[0]) || vop[0]=='#' || vop[0]=='@' || vop[0]=='%' || vop[0]=='$' || vop[0]=='\'' || vop[0]=='-')
        return 0;//inmediato
    else if (isalpha(vop[0]))
        return 1;//de registro
    else if (isalpha(vop[1]))
        return 3;//indirecto
    else
        return 2;//directo
}
/*void operandoIndirecto(char vop[], int* vopA){
    int valor = 0;
    char aux[20] = {'\0'};
    char aux2[20] = {'\0'};
    if (vop[2]=='+' || vop[2]=='-' || vop[3]=='+' || vop[3]=='-'){
        for (int i = 4; i < strlen(vop); i++){
            if (vop[i-1]=='+')
                aux[i-4] = vop[i];
            else if (vop[i-2]=='+' || vop[i-1]=='-')
                aux[i-4] = vop[i-1];
            else if (vop[i-2]=='-')
                aux[i-4] = vop[i-2];
        }
        valor = anyToInt(aux, aux2);
        //(valor << 4) & 0x00000FFF | *vopA;
        *vopA = valor;
    }
}*/
void operandoIndirecto(char vop[], int* valorReg,int *off,Label rotulos[],int cantRotulos,int *error){
    char reg[3];
    reg[0]=vop[0]; //Puede ser EAX o AX
    reg[1]=vop[1];
    reg[2] = (vop[2]=='+' || vop[2]=='-') ? '\0': vop[2];
    *valorReg=AEntero(reg); //no se si hay que hacerle desplazamiento

    if(vop[2]=='-')
        *off=((-1)*valorOpDirecto(vop+3,rotulos,cantRotulos,&(*error)))&0xFF;
    else if (vop[3]=='-')
        *off=((-1)*valorOpDirecto(vop+4,rotulos,cantRotulos,&(*error)))&0xFF;
    else if (vop[2]=='+')
        *off=(valorOpDirecto(vop+3,rotulos,cantRotulos,&(*error)))&0xFF;
    else if (vop[3]=='+')
        *off=(valorOpDirecto(vop+4,rotulos,cantRotulos,&(*error)))&0xFF;
}
int valorOpDirecto(char *s,Label rotulos[],int cantRotulos,int *error){
    int i;
    if(s[0]=='\'')
        return s[1];
    else if(s[0]=='@'|| s[0]=='#' || s[0]=='%' || (s[0]>='0' && s[0]<='9') || s[0]=='-')
        return anyToInt(s,NULL);
    else if(strlen(s)>2) {
        i= busquedaLabel(rotulos,s,cantRotulos,&(*error));
        if(i!=-1){
            return i; //Faltaria el desplazamiento de CS
        }
        else{
            printf("Rotulo indefinido\n");
        }
    }
    else
        return -99999;

}
void eliminaCorchetes(char vop[]){
    char aux[20] = {'\0'};
    int n = strlen(vop)-1;
    if (vop[0]=='[' && vop[n]==']')
        for (int i = 0; i < n; i++)
            aux[i] = vop[i+1];
    strcpy(vop, aux);
}
int anyToInt(char *s, char **out){
    char *BASES = {"**$*****@*#*****%"};
    int base = 10;
    char *bp = strchr(BASES, *s);
    if (bp!=NULL){
        base = bp - BASES;//Resta de punteros
        ++s;
    }
    return strtol(s, out, base);
}
int AEntero(char vop[]){
    //Registros = {"DS","SS","ES","CS","HP","IP","SP","BP","CC","AC","A","B","C","D","E","F"};
    int valor;
    for (int i = 0; i < strlen(vop); i++)
        vop[i] = toupper(vop[i]);
    if (strcmp(vop, "DS")==0)
        valor = 0x0;
    else if (strcmp(vop, "SS")==0)
        valor = 0x1;
    else if (strcmp(vop, "ES")==0)
        valor = 0x2;
    else if (strcmp(vop, "CS")==0)
        valor = 0x3;
    else if (strcmp(vop, "HP")==0)
        valor = 0x4;
    else if (strcmp(vop, "IP")==0)
        valor = 0x5 | (0x3 << 4);
    else if (strcmp(vop, "SP")==0)
        valor = 0x6 | (0x3 << 4);
    else if (strcmp(vop, "BP")==0)
        valor = 0x7 | (0x3 << 4);
    else if (strcmp(vop, "CC")==0)
        valor = 0x8;
    else if (strcmp(vop, "AC")==0)
        valor = 0x9;
    else if (strcmp(vop, "EAX")==0 || strcmp(vop, "AX")==0 || strcmp(vop, "AH")==0 || strcmp(vop, "AL")==0){
        valor = 0xA;
        if (strcmp(vop, "AX")==0)
            valor = valor | (0x3 << 4);
        else if (strcmp(vop, "AH")==0)
            valor = valor | (0x2 << 4);
        else if (strcmp(vop, "AL")==0)
            valor = valor | (0x1 << 4);
    }
    else if (strcmp(vop, "EBX")==0 || strcmp(vop, "BX")==0 || strcmp(vop, "BH")==0 || strcmp(vop, "BL")==0){
        valor = 0xB;
        if (strcmp(vop, "BX")==0)
            valor = valor | (0x3 << 4);
        else if (strcmp(vop, "BH")==0)
            valor = valor | (0x2 << 4);
        else if (strcmp(vop, "BL")==0)
            valor = valor | (0x1 << 4);
    }
    else if (strcmp(vop, "ECX")==0 || strcmp(vop, "CX")==0 || strcmp(vop, "CH")==0 || strcmp(vop, "CL")==0){
        valor = 0xC;
        if (strcmp(vop, "CX")==0)
            valor = valor | (0x3 << 4);
        else if (strcmp(vop, "CH")==0)
            valor = valor | (0x2 << 4);
        else if (strcmp(vop, "CL")==0)
            valor = valor | (0x1 << 4);
    }
    else if (strcmp(vop, "EDX")==0 || strcmp(vop, "DX")==0 || strcmp(vop, "DH")==0 || strcmp(vop, "DL")==0){
        valor = 0xD;
        if (strcmp(vop, "DX")==0)
            valor = valor | (0x3 << 4);
        else if (strcmp(vop, "DH")==0)
            valor = valor | (0x2 << 4);
        else if (strcmp(vop, "DL")==0)
            valor = valor | (0x1 << 4);
    }
    else if (strcmp(vop, "EEX")==0 || strcmp(vop, "EX")==0 || strcmp(vop, "EH")==0 || strcmp(vop, "EL")==0){
        valor = 0xE;
        if (strcmp(vop, "EX")==0)
            valor = valor | (0x3 << 4);
        else if (strcmp(vop, "EH")==0)
            valor = valor | (0x2 << 4);
        else if (strcmp(vop, "EL")==0)
            valor = valor | (0x1 << 4);
    }
    else if (strcmp(vop, "EFX")==0 || strcmp(vop, "FX")==0 || strcmp(vop, "FH")==0 || strcmp(vop, "FL")==0){
        valor = 0xF;
        if (strcmp(vop, "FX")==0)
            valor = valor | (0x3 << 4);
        else if (strcmp(vop, "FH")==0)
            valor = valor | (0x2 << 4);
        else if (strcmp(vop, "FL")==0)
            valor = valor | (0x1 << 4);
    }
    else//Si hubo error
        valor = 0xFFF;
    return valor;
}
void truncamiento(int cantOperandos, int *valor, int *flag){//Sólo entra si el operando es inmediato
    int maxval = (cantOperandos == 2)? 0xFFF : 0xFFFF;
    if ((*valor) > maxval){
        (*flag) = 1;
        (*valor) = (*valor) % (maxval+1);
    }
}
void salida(Linea codigo, int i, int inst, int wrgA, int wrgB){
    printf("[%04d]: %02X %02X %02X %02X", i, (inst>>24)&0xFF, (inst>>16)&0xFF, (inst>>8)&0xFF, (inst>>0)&0xFF);
    if(strcmp(codigo.label, "") != 0)
        printf("%12s: %s ", codigo.label, codigo.mnem);
    else
        printf("%12d: %s ", i+1, codigo.mnem);
    if (strcmp(codigo.argA, "") != 0){
        switch (strlen(codigo.mnem)){
            case 2: printf("%17s", codigo.argA);
            break;
            case 3: printf("%16s", codigo.argA);
            break;
            case 4: printf("%15s", codigo.argA);
            break;
        }
        if (strcmp(codigo.argB, "") != 0){
            printf(", %s", codigo.argB);//;%s
        }
    }
    if (strcmp(codigo.comentario, "") != 0){
        if (strcmp(codigo.argB, "") != 0){
            switch (strlen(codigo.argB)){
                case 1: printf("%10s%s", ";", codigo.comentario);
                break;
                case 2: printf("%9s%s", ";", codigo.comentario);
                break;
                case 3: printf("%8s%s", ";", codigo.comentario);
                break;
                case 4: printf("%7s%s", ";", codigo.comentario);
                break;
            }
        }
        else if (strcmp(codigo.argA, "") != 0)
            printf("%13s%s", ";", codigo.comentario);
        else
            printf("%20s%s", ";", codigo.comentario);
    }
    printf("\n");
    if (wrgA)
        printf("Warning: Línea %d Argumento A truncado\n", i);
    if (wrgB)
        printf("Warning: Línea %d Argumento B truncado\n", i);
}
void mayuscula(char x[]){
    int i;
    for (i=0;i<=strlen(x);i++)
        x[i]=toupper(x[i]);
}
