typedef struct{
    int actual;
    int cs;
    int ds;
    int es;
    int ss;
    int finSS;
} Segmento;

typedef struct{
    //operando[0]: Valor del operando
    //operando[1]: Tipo de operando
    //operando[2]: Tipo de registro del operando
    //operando[3]: Valor del registro (0 a 15)
    //operando[4]: Valor de X: [X] //
    //operando[5]: Offset del operando (caso indirecto)
    //flag[0]: -b
    //flag[1]: -c
    //flag[2]: -d
    //error: Error en la ejecución
    //Segmento: Posiciones iniciales de segmentos y segmento actual
    int operandoA[6];
    int operandoB[6];
    int flags[3];
    int error;
    Segmento segmento;
} OperandosYFlags;

typedef void (*VectorFunciones[4096])(Memoria*,OperandosYFlags);

void MOV(Memoria *, OperandosYFlags);
void ADD(Memoria *, OperandosYFlags);
void SUB(Memoria *, OperandosYFlags);
void SWAP(Memoria *, OperandosYFlags);
void MUL(Memoria *, OperandosYFlags);
void DIV(Memoria *, OperandosYFlags);
void CMP(Memoria *, OperandosYFlags);
void SHL(Memoria *, OperandosYFlags);
void SHR(Memoria *, OperandosYFlags);
void AND(Memoria *, OperandosYFlags);
void OR(Memoria *, OperandosYFlags);
void XOR(Memoria *, OperandosYFlags);
void SLEN(Memoria *, OperandosYFlags);
void SMOV(Memoria *, OperandosYFlags);
void SCMP(Memoria *, OperandosYFlags);

void SYS(Memoria *, OperandosYFlags);
void JMP(Memoria *, OperandosYFlags);
void JZ(Memoria *, OperandosYFlags);
void JP(Memoria *, OperandosYFlags);
void JN(Memoria *, OperandosYFlags);
void JNZ(Memoria *, OperandosYFlags);
void JNP(Memoria *, OperandosYFlags);
void JNN(Memoria *, OperandosYFlags);
void LDL(Memoria *, OperandosYFlags);
void LDH(Memoria *, OperandosYFlags);
void RND(Memoria *, OperandosYFlags);
void NOT(Memoria *, OperandosYFlags);
void PUSH(Memoria *, OperandosYFlags);
void POP(Memoria *, OperandosYFlags);
void CALL(Memoria *, OperandosYFlags);

void RET(Memoria *, OperandosYFlags);
void STOP(Memoria *, OperandosYFlags);

//-------------------------

int decodificaCodigo(int);
int verificoHeader(Header);
int seteoSegmentos(Memoria*,Header,OperandosYFlags*);
int cuentaChars(char cadena[], char caracter,int longitud);

void imprimeMnemonico(int);
void stringRegistro(int[],char[]);
void inicializaRegistros(Memoria*);
void imprimeEstadoRegistros(int[]);
void SYS1(Memoria *, OperandosYFlags);
void SYS2(Memoria *, OperandosYFlags);
void SYS3(Memoria *, OperandosYFlags);
void SYS4(Memoria *, OperandosYFlags);
void SYS7(Memoria *, OperandosYFlags);
void SYSD(Memoria *, OperandosYFlags);
void SYSF(Memoria *, OperandosYFlags);
void disassembler(Memoria,OperandosYFlags);
void iniciaVectorFunciones(VectorFunciones);
void imprimeOperandos(OperandosYFlags,int,int);
void inicializaFlags(OperandosYFlags*,int,char*[]);
void decodificaOperandos(Memoria,int,int, OperandosYFlags*);
void QuitaCaracter(char cadena[],char caracter,int longitud);
