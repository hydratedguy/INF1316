struct fila {
    int num_elementos;
    struct processo* primeiro_no;

} fila;

struct processo {
    int pid;
    int duracao;
    int inicio;
    processo* proximo_no;
}processo;





