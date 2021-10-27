enum status {PRONTO, ESPERA, EXECUTANDO};

typedef struct fila Fila;
typedef struct processo Processo;

Processo* CriaProcesso(int pid, char* programa);
Fila* CriaFila();
void InsereProcesso(Fila* f, Processo* p);
Processo* RemoveProcesso(Fila* f);
void printa_fila(Fila* f);
Processo* CopiaProcesso(Processo* p);
