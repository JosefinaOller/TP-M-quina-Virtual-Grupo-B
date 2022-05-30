typedef struct{
    FILE *arch;
    int nroUnidad;
    unsigned char cantCilindros;
    unsigned char cantCabezas;
    unsigned char cantSectores;
    unsigned int tamSector;
    char ultimoEstado;
} DatosDisco;

typedef struct{
    int cant;
    DatosDisco info[255];
} Disco;

typedef struct {
    char detalle[100];
}Mensaje;

typedef struct{
    int RAM[8192];
    int VectorDeRegistros[16];
    Disco discos;
    int error;
    Mensaje msjError[10];
            //1: Segmentation Fault
            //2: Stack Overflow
            //3: Stack Underflow
    } Memoria;

typedef struct{
    int bloque[6];
} Header;

