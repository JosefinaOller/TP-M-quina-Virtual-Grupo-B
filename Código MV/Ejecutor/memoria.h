typedef int32_t CeldaDeMemoria4B;
typedef int16_t CeldaDeMemoria2B;
typedef int8_t CeldaDeMemoria1B;

typedef struct{
    CeldaDeMemoria4B VectorDeRegistros[16];
    CeldaDeMemoria4B RAM[4096];
    CeldaDeMemoria2B SubVector16[6];
    CeldaDeMemoria1B SubVector8[12];
    } Memoria;
