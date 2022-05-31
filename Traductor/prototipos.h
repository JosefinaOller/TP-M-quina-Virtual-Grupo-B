#define MNEMAX 32 //la cantidad maxima de mnemonicos

typedef struct{
    char etiqueta[5];
    int cantOp;
    int codigo;
} Mnemonico; //la estructura de mnemonico

typedef struct{
    char label[30];
    char mnem[5];
    char argA[30];
    char argB[30];
    char comentario[1025];
}Linea; //la estructura de línea

typedef struct{
    char etiqueta[30];
    int codigo;
} Label;

void cargaMnemonico(char[],int ,int , Mnemonico *);
void cargaLinea(char [],char [],char [], char [], char [], Linea *);
