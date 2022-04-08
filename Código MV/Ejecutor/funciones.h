void MOV(Memoria *);
void ADD(Memoria *);
void SUB(Memoria *);
void SWAP(Memoria *);
void MUL(Memoria *);
void DIV(Memoria *);
void CMP(Memoria *);
void SHL(Memoria *);
void SHR(Memoria *);
void AND(Memoria *);
void OR(Memoria *);
void XOR(Memoria *);

void SYS(Memoria *);
void JMP(Memoria *);
void JZ(Memoria *);
void JP(Memoria *);
void JN(Memoria *);
void JNZ(Memoria *);
void JNP(Memoria *);
void JNN(Memoria *);
void LDL(Memoria *);
void LDH(Memoria *);
void RND(Memoria *);
void NOT(Memoria *);

void STOP(Memoria *);

//-------------------------

int decodificaCodigo(int);
