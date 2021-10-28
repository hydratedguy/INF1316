// enum status {NOVO, PRONTO, ESPERA, EXECUTANDO};

typedef struct Fila Fila;
typedef struct Processo Processo;



Processo* CriaProcesso(int pid, char* programa);
Fila* CriaFila();
// void InsereProcesso(Fila* f, Processo* p);
Processo* RemoveProcesso(Fila* f);
void printa_fila(Fila* f);
// Processo* CopiaProcesso(Processo* p);


