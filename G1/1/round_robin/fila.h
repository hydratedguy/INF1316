typedef struct fila Fila {
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
} Fila;

enum status {PRONTO, ESPERA, EXECUTANDO};

typedef struct processo Processo{
    int pid;
    enum status status; // p=pronto, w=espera,e=executando
    char* programa;
    Processo* proximo_no;
}Processo;