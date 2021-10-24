enum status {PRONTO, ESPERA, EXECUTANDO};

typedef struct processo* Processo;
typedef struct fila* Fila;

Processo* CriaProcesso(int pid, char* programa);
Fila* CriaFila();
int FilaVazia(Fila* f);
Processo* ExecutaProcesso(Fila* f);
void InsereProcesso(Fila* f, Processo* p);
void atualizaProcesso(Processo* p, enum status status);
