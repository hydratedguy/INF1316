struct fila {
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
};

typedef struct processo Processo;
struct processo {
    int pid;
    char status; // p=pronto, w=espera,e=executando
    char* programa;
    int ti;
    int duracao;
    Processo* proximo_no;
};