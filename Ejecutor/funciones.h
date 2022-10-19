typedef struct{
    //operando[0]: Valor del operando
    //operando[1]: Tipo de operando
    //operando[2]: Tipo de registro del operando
    //operando[3]: Valor del registro (0 a 15)
    //operando[4]: Valor de X: [X]
    //flag[0]: -b
    //flag[1]: -c
    //flag[2]: -d
    int operandoA[5];
    int operandoB[5];
    int flags[3];
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

void STOP(Memoria *, OperandosYFlags);

//-------------------------

void inicializaRegistros(Memoria*);
void imprimeMnemonico(int);
void stringRegistro(int[],char[]);
void imprimeEstadoRegistros(int[]);
void imprimeOperandos(OperandosYFlags,int,int);
void disassembler(Memoria,OperandosYFlags);
void inicializaFlags(OperandosYFlags*,int,char*[]);
void iniciaVectorFunciones(VectorFunciones);
int verificoHeader(Header);
int decodificaCodigo(int);
void decodificaOperandos(Memoria,int,int, OperandosYFlags*);
void SYS1(Memoria *, OperandosYFlags);
void SYS2(Memoria *, OperandosYFlags);
void SYSF(Memoria *, OperandosYFlags);
void QuitaCaracter(char cadena[],char caracter,int longitud);
int cuentaChars(char cadena[], char caracter,int longitud);
