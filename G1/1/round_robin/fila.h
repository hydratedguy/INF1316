struct fila {
    int num_elementos;
    struct processo* primeiro_no;
    struct processo* ultimo_no;

} fila;

struct processo {
    int pid;
    processo* proximo_no;
}processo;