#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum status {NOVO, PRONTO, ESPERA, EXECUTANDO};
typedef struct Fila Fila;
typedef struct Processo Processo;

struct Fila {
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
};

struct Processo {
    int pid;
    char status; // p=pronto, w=espera,e=executando
    char programa[9];
    int ti;
    int duracao;
    Processo* proximo_no;
};


void printa_fila(Fila* f){
    printf("Fila \n");
    for (Processo* p = f->primeiro_no; p != NULL; p = p->proximo_no) {
        printf("%s, ", p->programa);
    }
    printf("\n");
}

Processo* CriaProcesso(int ti, int d, char* programa, Processo* dependencia){
    Processo* new = (Processo*) malloc (sizeof(Processo));
    new->pid = 1;
    strcpy(new->programa, programa);
    new->status = 0;
    new->ti = ti;
    if(dependencia == NULL){
        new->duracao = -1;
    }
    else{
        new->duracao = d;
    }
    new->proximo_no = dependencia;
    return new;
}

Fila* CriaFila(){
    Fila* f = (Fila*) malloc (sizeof( Fila));
    f->num_elementos = 0;
    f->primeiro_no = NULL;
    f->ultimo_no = NULL;
    return f;
}


////////////////////////////////////////////////