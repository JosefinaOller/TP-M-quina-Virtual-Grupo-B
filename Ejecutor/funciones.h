typedef struct{
    int operandoA[4];
    int operandoB[4];
} Operandos;

/*
    operandoX[0] : Valor
    operandoX[1] : Tipo de operando
    operandoX[2] : Tipo de registro del operando
    operandoX[3] : Valor del registro
*/

void MOV(Memoria *, Operandos);
void ADD(Memoria *);
void SUB(Memoria *);
void SWAP(Memoria *, Operandos);
void MUL(Memoria *);
void DIV(Memoria *);
void CMP(Memoria *);
void SHL(Memoria *);
void SHR(Memoria *);
void AND(Memoria *, Operandos);
void OR(Memoria *, Operandos);
void XOR(Memoria *, Operandos);

void SYS(Memoria *);
void JMP(Memoria *);
void JZ(Memoria *);
void JP(Memoria *);
void JN(Memoria *);
void JNZ(Memoria *);
void JNP(Memoria *);
void JNN(Memoria *);
void LDL(Memoria *, Operandos);
void LDH(Memoria *, Operandos);
void RND(Memoria *);
void NOT(Memoria *, Operandos);

void STOP(Memoria *);

//-------------------------

int verificoHeader(Header);
int decodificaCodigo(int);
void decodificaOperandos(Memoria,int,int, Operandos*);
