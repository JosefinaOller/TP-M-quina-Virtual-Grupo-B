#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prototipos.h"
#include "parser.h"
/** \brief Este programa traductor traduce instrucciones en Assembler y genera un archivo .asm.
 *
 * \param argc      Cantidad de argumentos.
 * \param *argv[]   Los archivos, tanto el .asm como el .mv2, y el -o que habilita o deshabilita la salida por pantalla.
 * \return 0
 *
 */
void cargaVecMnem(Mnemonico[]);
void traduce(char[], Mnemonico[], int, char[]);
void estructuraASM(char[], Linea[], int*);
void procesarLinea(char[], char[], char[], char[], char[], char[], char[], char[], char[], char[]);
void lecturaLabelsYSegmentos(char*, Label[], int*, int*,int[],int *,int *,int*,int*,int*,char[][100],int *,int[]);
int busquedaLabel(Label[], char[], int);
int codificaInstruccion(Linea, Mnemonico[], Label[], int, int*, int*, int*);
int tipoOperando(char[]);
void eliminaCorchetes(char[]);
int anyToInt(char*, char**);
int AEntero(char[]);
void truncamiento(int, int*, int*);
void salida(Linea, int, int, int, int);
void mayuscula(char[]);
void tratamiento_especial(char [],Label[],int,int *,int[],int *);
void operandoIndirecto(char[], int*, int*, Label[], int, int*);
int valorOpDirecto(char*, Label[], int, int*);
int esRegistro(char[]);

int main(int argc, const char *argv[]){
    char asmar[20],binario[20];
    int o=1,i;
    Mnemonico vecMnem[MNEMAX];
    cargaVecMnem(vecMnem); //Carga todos los mnemonicos con sus datos.
    for(i=1;i<argc;i++){
        if(strstr(argv[i],".asm"))
            strcpy(asmar,argv[i]);
        else if(strstr(argv[i],".mv2"))
            strcpy(binario,argv[i]);
        else if (strstr (argv[i],"-o")) //El flag o, si esta activado, no se muestr� pero se genera el archivo binario.
            o=0;
    }
    if (argc>=2)
        traduce(asmar,vecMnem,o,binario);
    else
        printf("Faltaron par�metros\n");
    return 0;
}
/** \brief Carga datos de mnenomicos
 *
 * \param vecMnem[] es un vector de mnem�nicos.
 *
 */
void cargaVecMnem(Mnemonico vecMnem[]){

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
/** \brief Funci�n que procesa las instrucciones, las imprime y genera el archivo binario
 *
 * \param nombAsm[] Nombre del archivo .asm.
 * \param binario[] Nombre del archivo .mv2.
 * \param vecMnem[] y \param o explicados anteriormente.
 *
 */

void traduce(char nombAsm[],Mnemonico vecMnem[],int o, char binario[]){

    FILE *archESCRITURA; //para el archivo binario
    char lista[100][100];//lista incluye a las directivas,constantes y los comentarios
    Label rotulos[100];
    Linea codigo[500];
    int i,cantRotulos=0,error=0,conLin=0,nLista=0,indicelinea[100],inst, wrgA, wrgB,kcom=0,nroLinea,data=1024,extra=1024,stack=1024,cantStrings=0,strings[500];
    for(i=0;i<100;i++)
        strcpy(lista[i],"");

    lecturaLabelsYSegmentos(nombAsm,rotulos,&cantRotulos,&error,strings,&nroLinea,&cantStrings,&data,&extra,&stack,lista,&nLista,indicelinea); //para guardar los rotulos y su posicion, tambien para guardar las constantes.
    if(error)
        printf("Error de archivo\n");
    else{
        estructuraASM(nombAsm,codigo,&conLin); //procesa las instrucciones asm
        int cs = nroLinea;
        if((data+extra+stack) <= (8192 - cs) && (data>=0 && data<=65535) && (extra>=0 && extra<=65535) && (stack>=0 && stack<=65535)){ //Calculo de memoria
            if(conLin>=0 || nLista>=0){
                //empiezo a generar el archivo binario
                archESCRITURA=fopen(binario,"wb");
                //la cabecera
                int header=0x4D562D32;
                fwrite(&header,sizeof(int),1,archESCRITURA); //MV-2
                fwrite(&data,sizeof(int),1,archESCRITURA); //DS
                fwrite(&stack,sizeof(int),1,archESCRITURA); //SS
                fwrite(&extra,sizeof(int),1,archESCRITURA); //ES
                fwrite(&cs,sizeof(int),1,archESCRITURA); //CS
                header=0x562E3232;
                fwrite(&header,sizeof(int),1,archESCRITURA); //V.22

                //codigo

                for(i=0;i<conLin;i++){
                    wrgA = wrgB = 0;//Advertencias
                    if(strcmp(codigo[i].mnem,"")!=0){
                        inst=codificaInstruccion(codigo[i], vecMnem, rotulos, cantRotulos, &error, &wrgA, &wrgB); //codificaInstruccion arma la instrucci�n en un int
                        if(!error) //Si no hay error, grabo la instruccion codificada
                            fwrite(&inst,sizeof(int),1,archESCRITURA);
                    }
                    if(o){
                       while(i==indicelinea[kcom] && strcmp(lista[kcom],"")!=0){
                            printf("%3s\n",strtok(lista[kcom++],"\n"));
                       }
                       salida(codigo[i], i, inst, wrgA, wrgB);
                    } //Si el flag de ocultamiento esta desactivado, muestro lineas
                }
                for(int j=0;j<cantStrings;j++){
                   fwrite(&strings[j],sizeof(int),1,archESCRITURA);
                }
                while(i==indicelinea[kcom] && strcmp(lista[kcom],"")!=0){
                    printf("%3s\n",strtok(lista[kcom++],"\n"));
                }
            fclose(archESCRITURA);
            }
        }
        else{
            if((data+extra+stack) > (8192 - cs) && (data>=0 && data<=65535) && (extra>=0 && extra<=65535) && (stack>=0 && stack<=65535)){
                //En este caso, se genera un error si la suma de los 3 segmentos supera 8192 y los 3 valores est�n dentro del rango [0..65535]
                error=1;
                printf("Falta de memoria\n");
            }
            else{//En este caso, se genera un error si uno de los 3 segmentos est� fuera del rango
                error=2;
                printf("Valores no apropiados.\n");
            }
        }
    }
    if(error){ //Si hay error de traduccion, borro el archivo binario
        printf("Error de traduccion\n");
        remove(binario);
    }
}
/** \brief Este m�todo procesa las instrucciones.
 *
 * \param
 * \param
 * \return
 *
 */

void estructuraASM(char nombAsm[],Linea codigo[],int *conLin){

    FILE *arch;
    char linea[500],rotulo[30],mnem[5],argA[30],argB[30],com[1025],seg[6],size[8], constante[30],constante_valor[100];
    arch=fopen(nombAsm,"rt");
    if(arch!=NULL){
        *conLin=0;
        while(fgets(linea,500,arch)!=NULL){
            strcpy(rotulo,"");strcpy(mnem,"");strcpy(argA,"");strcpy(argB,"");strcpy(com,""); //los inicializo
            strcpy(seg,"");strcpy(size,"");
            procesarLinea(linea,rotulo,mnem,argA,argB,com, seg, size, constante, constante_valor);
            mayuscula(rotulo); mayuscula(mnem); mayuscula(argA); mayuscula(argB);
            cargaLinea(rotulo,mnem,argA,argB,com,&codigo[*conLin]);
            if(strcmp(rotulo,"")!=0  || strcmp(mnem,"")!=0 || strcmp(argA,"")!=0 || strcmp(argB,"")!=0)
                (*conLin)++;
        } //while archivo
    }
    else
        printf("%s","ERROR DE ARCHIVO");
    fclose(arch);
}
/** \brief Separa todos los t�rminos de la instrucci�n. */
void procesarLinea(char linea[],char rotulo[],char mnem[],char argA[],char argB[],char com[], char seg[], char size[], char constante[], char constante_valor[]){

    //Utilizamos el parser de Franco, gracias por tanto crack!!

    char **parsed = parseline(linea);

    strcpy(rotulo,parsed[0] ? parsed[0] : "");//Rotulo
    strcpy(mnem,parsed[1] ? parsed[1] : "");//Mnemonico, es para ver si existe para sumar la cantidad de lineas
    strcpy(argA,parsed[2] ? parsed[2] : "");
    strcpy(argB,parsed[3] ? parsed[3] : "");
    strcpy(com,parsed[4] ? parsed[4] : "");
    strcpy(seg,parsed[5] ? parsed[5] : ""); //seg
    strcpy(size,parsed[6] ? parsed[6] : ""); //size
    strcpy(constante,parsed[7] ? parsed[7] : ""); //Constante
    strcpy(constante_valor,parsed[8] ? parsed[8] : ""); //Valor del constante
    freeline(parsed);
}
/** \brief Lee los rotulos, constantes y segmentos */
void lecturaLabelsYSegmentos(char *archivo,Label rotulos[],int *cantRotulos,int *error,int strings[],int *nroLinea,int *cantStrings,int *data,int *extra,int *stack,char lista[][100],int *nLista,int indiceLinea[]){

    FILE *arch=fopen(archivo,"rt");
    char linea[500],rotulo[30],mnem[5],constante[30],constante_valor[100],seg[6],size[8],argA[30],argB[30],com[1025];
    *nLista=0; //Para imprimir las directivas y equ
    if(arch!=NULL){
        while(fgets(linea,500,arch)!=NULL){
            procesarLinea(linea,rotulo,mnem,argA,argB,com, seg, size, constante, constante_valor);
            if(strcmp(rotulo,"")!=0){ //Hay rotulo
                mayuscula(rotulo);
                strcpy(rotulos[*cantRotulos].etiqueta,rotulo);
                rotulos[(*cantRotulos)++].codigo=(*nroLinea)++;
            }
            else if(strcmp(mnem,"")!=0)
                (*nroLinea)++;
            if(strcmp(com,"")!=0 && strcmp(rotulo,"")==0  && strcmp(mnem,"")==0 && strcmp(argA,"")==0 && strcmp(argB,"")==0 && strcmp(constante,"")==0){
                    strcpy(lista[*nLista],linea);
                    indiceLinea[(*nLista)++]=*nroLinea;
            }
            if(strcmp(seg,"DATA")==0){
                *data=atoi(size);
                strcpy(lista[*nLista],linea);
                indiceLinea[(*nLista)++]=*nroLinea;
            }
            else{
                if(strcmp(seg,"EXTRA")==0){
                    *extra=atoi(size);
                    strcpy(lista[*nLista],linea);
                    indiceLinea[(*nLista)++]=*nroLinea;
                }
                else{
                    if(strcmp(seg,"STACK")==0){
                        *stack=atoi(size);
                        strcpy(lista[*nLista],linea);
                        indiceLinea[(*nLista)++]=*nroLinea;
                    }
                }
                if(strcmp(constante,"")!=0){
                   strcpy(lista[*nLista],linea);
                   indiceLinea[(*nLista)++]=*nroLinea;
                }
            }
        }
        fclose(arch);
        //Vuelvo a leer el archivo para las constantes, por su desplazamiento con CS.
        FILE *arch=fopen(archivo,"rt");
        while(fgets(linea,500,arch)!=NULL){
            procesarLinea(linea,rotulo,mnem,argA,argB,com, seg, size, constante, constante_valor);
            if(strcmp(constante,"")!=0){ //Hay constante
                mayuscula(constante);
                if(busquedaLabel(rotulos,constante,*cantRotulos)==-99999){ //No hay duplicado
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
                            tratamiento_especial(constante_valor,rotulos,*cantRotulos,&(*nroLinea),strings,&(*cantStrings));
                            break;
                    }
                    (*cantRotulos)++;

                 } //if no hay duplicado
                else{
                    printf("S�mbolo duplicado\n");
                    *error=1;
                }
            }
         }
         fclose(arch);
    } //el archivo es null
    else{
        printf("Error de archivo\n");
        *error=1;
    }
     int i=1;
     while(indiceLinea[i]==indiceLinea[i+2]){ //descubrimos que por sin ninguna razon, despues de realizar los tratamientos especiales, se pone indiceLinea[2]=33 cuando deberia ser indiceLinea[2] = 0, en el caso particular de discos.asm
        indiceLinea[i+1]=indiceLinea[i];
        i++;
    }
} //fin
/** \brief M�todo para tratar las constantes que son <i>string</i>. */
void tratamiento_especial(char constante_valor[],Label rotulos[],int cantRotulos,int *nroLinea,int strings[],int *cantStrings){
    int i=1;
    rotulos[cantRotulos].codigo=(*nroLinea);
    while(constante_valor[i]!='"'){
        strings[(*cantStrings)++]=constante_valor[i];
        (*nroLinea)++;
        i++;
    }
    strings[(*cantStrings)++]='\0';
    (*nroLinea)++;
}
/** \brief M�todo para buscar la posicion del r�tulo o constante
 *
 * \param rotulos[] Vector de <i>Label</i>.
 * \param etiqueta[] Etiqueta a buscar.
 * \param cantRotulos Cantidad de r�tulos.
 * \return rotulos[i].codigo si encontr� la etiqueta. En caso contrario, \return -99999.
 *
 */

int busquedaLabel(Label rotulos[],char etiqueta[],int cantRotulos){
    int i=0;
    while(i<cantRotulos && strcmp(etiqueta,rotulos[i].etiqueta)!=0)
        i++;
    if (i<cantRotulos)
        return rotulos[i].codigo;
    else{
        return -99999;
    }
}
/** \brief M�todo que codifica cada instrucci�n Assembler.
 *
 * \param codigo Linea de c�digo a codificar.
 * \param *error Si hubo un error de sintaxis vale -1, de instrucci�n -2.
 * \param *wrgA y \param *wrgB <i>warnings</i> por valor truncado.
 * \return inst Valor de instrucci�n.
 *
 */

int codificaInstruccion(Linea codigo, Mnemonico vecMnem[], Label rotulos[], int cantRotulos, int *error, int *wrgA, int *wrgB){
    int i,valorReg,off,inst = 0;//Inicializo instrucci�n
    size_t length;
    Mnemonico mnem;
    char *out = NULL;
    if (vecMnem!=NULL)
        length = MNEMAX;
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
                        if(isalpha(codigo.argA[0])){
                            int i=busquedaLabel(rotulos,codigo.argA,cantRotulos);
                            if(i!=-99999)
                                vopA = i;
                            else
                                printf("Rotulo indefinido\n");
                        }
                        else
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
                        valorReg=0; off=0;
                        eliminaCorchetes(codigo.argA);
                        operandoIndirecto(codigo.argA,&valorReg,&off,rotulos,cantRotulos, error);
                        vopA = (valorReg & 0xF) | (off << 4);
                        break;
                    }
                    switch (topB){
                    case 0:
                        if(isalpha(codigo.argB[0])){
                            int i=busquedaLabel(rotulos,codigo.argB,cantRotulos);
                            if(i!=-99999)
                                vopB = i;
                            else
                                printf("Rotulo indefinido\n");
                        }
                        else
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
                        valorReg=0; off=0;
                        eliminaCorchetes(codigo.argB);
                        operandoIndirecto(codigo.argB,&valorReg,&off,rotulos,cantRotulos, error);
                        vopB = (valorReg & 0xF) | (off << 4);
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
                            operandoIndirecto(codigo.argA,&valorReg,&off,rotulos,cantRotulos, error);
                            vop= (valorReg & 0xF) | (off << 4);
                            inst = 0xF0000000 | ((mnem.codigo << 24) & 0x0F000000) | ((top << 22) & 0x00C00000) | (vop & 0x0000FFFF);
                            break;
                        }
                    }
                }else{
                    printf("Error de sintaxis\n");
                    *(error) = -1;
                    inst = 0xFFFFFFFF;
                }
            }else
                inst = 0xFF000000 | ((mnem.codigo << 20) & 0x00F00000);
        }
    }else{
        printf("Error de sintaxis\n");
        *(error) = -1;
        inst=0xFFFFFFFF;
    }
    return inst;
}
/** \brief M�todo para analizar el tipo de operando
 *
 * \param vop[] valor del operando
 * \return 0 si es inmediato, 1 de registro, 2 directo, 3 indirecto.
 *
 */

int tipoOperando(char vop[]){
    if(isalpha(vop[0])){
        if(esRegistro(vop))
            return 1; //de registro
        else
            return 0; //inmediato
    }
    else{
        if (isdigit(vop[0]) || vop[0]=='#' || vop[0]=='@' || vop[0]=='%' || vop[0]=='$' || vop[0]=='\'' || vop[0]=='-')
        return 0;//inmediato
        else if (isalpha(vop[1]) && esRegistro(vop+1))
                return 3;//indirecto
        else
            return 2;//directo
    }
}
/** \brief Funci�n (que tendr�a que ser booleana) para analizar si es un registro
 *
 * \param vop[] valor del operando
 * \return true (!=0) si es un registro, false(==0) en caso contrario
 *
 */

int esRegistro(char vop[]){
    char vreg[10][3] = {"DS","SS","ES","CS","HP","IP","SP","BP","CC","AC"};//S�lo comtemplo los 10 primeros registros
        if((vop[0]=='E' && vop[1]>='A' && vop[1]<='F' && vop[2]=='X') || (vop[0]>='A' && vop[0]<='F' && (vop[1]=='X' || vop[1]=='H' || vop[1]=='L')))
            return 1; //de registro
        else{
            int i = 0;
            while (i < 10 && strncmp(vop, vreg[i], 2) != 0)
                i++;
            if (i < 10)
                return i;
            else
                return 0; //inmediato
    }
}
/** \brief Tratamiento para los operandos indirectos
 *
 * \param *valorReg Valor del registro.
 * \param *off <i>Offset</i>
 * \param vop[], \param rotulos[], \param cantRotulos y \param *error ya explicados
 *
 */

void operandoIndirecto(char vop[], int* valorReg,int *off,Label rotulos[],int cantRotulos,int *error){
    char reg[3];
    reg[0] = vop[0]; //Puede ser EAX o AX
    reg[1] = vop[1];
    reg[2] = (vop[2]=='+' || vop[2]=='-') ? '\0': vop[2];
    *valorReg = 0;
    if(esRegistro(reg)){
        *valorReg = AEntero(reg);
        if(vop[2] == '-')
            *off = ((-1)*valorOpDirecto(vop+3,rotulos,cantRotulos,&(*error)))&0xFF;
        else if (vop[3]=='-')
            *off = ((-1)*valorOpDirecto(vop+4,rotulos,cantRotulos,&(*error)))&0xFF;
        else if (vop[2]=='+')
            *off = (valorOpDirecto(vop+3,rotulos,cantRotulos,&(*error)))&0xFF;
        else if (vop[3]=='+')
            *off = (valorOpDirecto(vop+4,rotulos,cantRotulos,&(*error)))&0xFF;
    }
    else
        *off = (valorOpDirecto(vop,rotulos,cantRotulos,&(*error)))&0xFF;
}
/** \brief Para obtener un valor inmediato. */

int valorOpDirecto(char *s,Label rotulos[],int cantRotulos,int *error){
    int i;
    if(s[0]=='\'')
        return s[1];
    else if(s[0]=='@'|| s[0]=='#' || s[0]=='%' || (s[0]>='0' && s[0]<='9') || s[0]=='-')
        return anyToInt(s,NULL);
    else if(strlen(s)>2) {
        i= busquedaLabel(rotulos,s,cantRotulos);
        if(i!=-99999){
            return i;
        }
        else{
            printf("Rotulo indefinido\n");
            *error=1;
            return -1;
        }
    }
    else
        return -99999;

}
/** \brief Elimina los corchetes de un operando directo o indirecto. */

void eliminaCorchetes(char vop[]){
    char aux[20] = {'\0'};
    int n = strlen(vop)-1;
    if (vop[0]=='[' && vop[n]==']')
        for (int i = 1; i < n; i++)
            aux[i-1] = vop[i];
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
/** \brief Convierte el valor del registro a entero.
 *
 * \param vop[] ya explicado anteriormente.
 * \return valor Valor del registro entre 0 y 15. -1 si hubo un error.
 *
 */

int AEntero(char vop[]){
    //Registros = {"DS","SS","ES","CS","HP","IP","SP","BP","CC","AC","A","B","C","D","E","F"};
    int valor;
    if (vop[0] == 'D' && vop[1] == 'S') //Sabemos que van con strcmp pero de alguna forma por ser char, siempre arrastra basura y no se lee de manera correcta
        valor = 0x0;
    else if (vop[0] == 'S' && vop[1] == 'S')
        valor = 0x1;
    else if (vop[0] == 'E' && vop[1] == 'S')
        valor = 0x2;
    else if (vop[0] == 'C' && vop[1] == 'S')
        valor = 0x3;
    else if (vop[0] == 'H' && vop[1] == 'P')
        valor = 0x4;
    else if (vop[0] == 'I' && vop[1] == 'P')
        valor = 0x5 | (0x3 << 4);
    else if (vop[0] == 'S' && vop[1] == 'P')
        valor = 0x6 | (0x3 << 4);
    else if (vop[0] == 'B' && vop[1] == 'P')
        valor = 0x7 | (0x3 << 4);
    else if (vop[0] == 'C' && vop[1] == 'C')
        valor = 0x8;
    else if (vop[0] == 'A' && vop[1] == 'C')
        valor = 0x9;
    else if ((vop[0] == 'E' && vop[1] == 'A' && vop[2] == 'X') || (vop[0] == 'A' && vop[1] == 'X') || (vop[0] == 'A' && vop[1] == 'H') || (vop[0] == 'A' && vop[1] == 'L')){
        valor = 0xA;
        if (vop[0] == 'A' && vop[1] == 'X')
            valor = valor | (0x3 << 4);
        else if (vop[0] == 'A' && vop[1] == 'H')
            valor = valor | (0x2 << 4);
        else if (vop[0] == 'A' && vop[1] == 'L')
            valor = valor | (0x1 << 4);
    }
    else if ((vop[0] == 'E' && vop[1] == 'B' && vop[2] == 'X') || (vop[0] == 'B' && vop[1] == 'X') || (vop[0] == 'B' && vop[1] == 'H') || (vop[0] == 'B' && vop[1] == 'L')){
        valor = 0xB;
        if (vop[0] == 'B' && vop[1] == 'X')
            valor = valor | (0x3 << 4);
        else if (vop[0] == 'B' && vop[1] == 'H')
            valor = valor | (0x2 << 4);
        else if (vop[0] == 'B' && vop[1] == 'L')
            valor = valor | (0x1 << 4);
    }
    else if ((vop[0] == 'E' && vop[1] == 'C' && vop[2] == 'X') || (vop[0] == 'C' && vop[1] == 'X') || (vop[0] == 'C' && vop[1] == 'H') || (vop[0] == 'C' && vop[1] == 'L')){
        valor = 0xC;
        if (vop[0] == 'C' && vop[1] == 'X')
            valor = valor | (0x3 << 4);
        else if (vop[0] == 'C' && vop[1] == 'H')
            valor = valor | (0x2 << 4);
        else if (vop[0] == 'C' && vop[1] == 'L')
            valor = valor | (0x1 << 4);
    }
    else if ((vop[0] == 'E' && vop[1] == 'D' && vop[2] == 'X') || (vop[0] == 'D' && vop[1] == 'X') || (vop[0] == 'D' && vop[1] == 'H') || (vop[0] == 'D' && vop[1] == 'L')){
        valor = 0xD;
        if (vop[0] == 'D' && vop[1] == 'X')
            valor = valor | (0x3 << 4);
        else if (vop[0] == 'D' && vop[1] == 'H')
            valor = valor | (0x2 << 4);
        else if (vop[0] == 'D' && vop[1] == 'L')
            valor = valor | (0x1 << 4);
    }
    else if ((vop[0] == 'E' && vop[1] == 'E' && vop[2] == 'X') || (vop[0] == 'E' && vop[1] == 'X') || (vop[0] == 'E' && vop[1] == 'H') || (vop[0] == 'E' && vop[1] == 'L')){
        valor = 0xE;
        if (vop[0] == 'E' && vop[1] == 'X')
            valor = valor | (0x3 << 4);
        else if (vop[0] == 'E' && vop[1] == 'H')
            valor = valor | (0x2 << 4);
        else if (vop[0] == 'E' && vop[1] == 'L')
            valor = valor | (0x1 << 4);
    }
    else if ((vop[0] == 'E' && vop[1] == 'F' && vop[2] == 'X') || (vop[0] == 'F' && vop[1] == 'X') || (vop[0] == 'F' && vop[1] == 'H') || (vop[0] == 'F' && vop[1] == 'L')){
        valor = 0xF;
        if (vop[0] == 'F' && vop[1] == 'X')
            valor = valor | (0x3 << 4);
        else if (vop[0] == 'F' && vop[1] == 'H')
            valor = valor | (0x2 << 4);
        else if (vop[0] == 'F' && vop[1] == 'L')
            valor = valor | (0x1 << 4);
    }
    else//Si hubo error
        valor = 0xFFF;
    return valor;
}
/** \brief S�lo entra si el operando es inmediato. */

void truncamiento(int cantOperandos, int *valor, int *flag){
    int maxval = (cantOperandos == 2)? 0xFFF : 0xFFFF;
    if ((*valor) > maxval){
        (*flag) = 1;
        (*valor) = (*valor) % (maxval+1);
    }
}
/** \brief Impresi�n de las instrucciones. */

void salida(Linea codigo, int i, int inst, int wrgA, int wrgB){
    printf("[%04d]: %02X %02X %02X %02X", i, (inst>>24)&0xFF, (inst>>16)&0xFF, (inst>>8)&0xFF, (inst>>0)&0xFF);
    if(strcmp(codigo.label, "") != 0)
        printf("%20s: %s ", codigo.label, codigo.mnem);
    else
        printf("%20d: %s ", i+1, codigo.mnem);
    if (strcmp(codigo.argA, "") != 0){
        switch (strlen(codigo.mnem)){
            case 2: printf("%20s", codigo.argA);
            break;
            case 3: printf("%20s", codigo.argA);
            break;
            case 4: printf("%20s", codigo.argA);
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
                case 5: printf("%6s%s", ";", codigo.comentario);
                break;
                case 6: printf("%5s%s", ";", codigo.comentario);
                break;
                case 7: printf("%4s%s", ";", codigo.comentario);
                break;
                case 8: printf("%3s%s", ";", codigo.comentario);
                break;
                case 9: printf("%2s%s", ";", codigo.comentario);
                break;
                case 10: printf("%1s%s", ";", codigo.comentario);
                break;
                default: printf("%10s%s", ";", codigo.comentario);
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
        printf("Warning: L�nea %d Argumento A truncado\n", i);
    if (wrgB)
        printf("Warning: L�nea %d Argumento B truncado\n", i);
}
/** \brief M�todo para convertir cualquier cadena en may�sculas.
 *
 * \param x[] una cadena cualquiera.
 */

void mayuscula(char x[]){
    int i;
    for (i=0;i<=strlen(x);i++)
        x[i]=toupper(x[i]);
}
