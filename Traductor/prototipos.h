#define MNEMAX 32 //la cantidad maxima de mnemonicos

typedef struct{
    char etiqueta[5];
    int cantOp;
    int codigo;
} Mnemonico; //la estructura de mnemonico

typedef struct{
    char label[16];
    char mnem[5];
    char argA[16];
    char argB[16];
    char comentario[1025];
}Linea; //la estructura de línea

typedef struct{
    char etiqueta[16];
    int codigo;
} Label;
