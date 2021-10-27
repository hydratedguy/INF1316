#include <stdio.h>
#include <stdlib.h>

enum status {NOVO, PRONTO, ESPERA};
typedef struct fila Fila;
typedef struct processo Processo;


struct fila {
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
};

struct processo {
    int pid;
    enum status status; // p=pronto, w=espera,e=executando
    char* programa;
    Processo* proximo_no;
};


void printa_fila(Fila* f){
    for (Processo* p = f->primeiro_no; p != NULL; p = p->proximo_no) {
        printf("%s, ", p->programa);
    }
    printf("\n");
}

Processo* CriaProcesso(int pid, char* programa){
    Processo* new = (Processo*) malloc (sizeof(Processo));
    new->pid = pid;
    new->programa = programa;
    new->status = NOVO;
    new->proximo_no = NULL;
    return new;
}

 Fila* CriaFila(){
    Fila* f = (Fila*) malloc (sizeof( Fila));
    f->num_elementos = 0;
    f->primeiro_no = NULL;
    f->ultimo_no = NULL;
    return f;
}

Processo* CopiaProcesso(Processo* p){
	Processo *novo;
	novo = CriaProcesso(p->pid, p->programa);
	novo->status = p->status;
	novo->proximo_no = NULL;
	return novo;
}


void InsereProcesso(Fila* f, Processo* p){
    Processo* new = CopiaProcesso(p);

    if (f->num_elementos >= 1) {
        f->ultimo_no->proximo_no = new;
    } else {
        f->primeiro_no = new;
    }

    f->ultimo_no = new;
    f->num_elementos++;
}


Processo* RemoveProcesso(Fila* f){
    
    if(f->num_elementos <= 0) {
        printf("num elementos da fila = %d\n", f->num_elementos);
        return NULL;
    }

    Processo* retirado = f->primeiro_no;

    // Processo* new = (Processo*) malloc (sizeof(Processo));

    // new->pid = retirado->pid;
    // new->programa = retirado->programa;
    // new->status = 1;
    // new->proximo_no = NULL;

    if (f->num_elementos > 1) {
        f->primeiro_no = retirado->proximo_no;
        retirado->proximo_no = NULL;

    } else {

        f->primeiro_no = NULL;
        f->ultimo_no = NULL;
    }

    f->num_elementos--;
    // free(retirado);

    
    return retirado;
}

