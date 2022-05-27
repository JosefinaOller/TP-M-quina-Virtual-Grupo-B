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

typedef struct{
    int RAM[8192];
    int VectorDeRegistros[16];
    Disco discos;
    } Memoria;

typedef struct{
    int bloque[6];
} Header;

