#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct Processo{
    int pid;
    enum status status; // p=pronto, w=espera,e=executando
    char* programa;
    Processo* proximo_no;
};

struct Fila{
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
}; 

Processo* CriaProcesso(int pid, char* programa){
    Processo* new = (Processo*) malloc (sizeof(Processo));
    new.pid = pid;
    new->status = PRONTO;
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


int FilaVazia(Fila* f){
    if(f->num_elementos == 0){
        return 1;
    }
    return 0;
}


void InsereProcesso(Fila* f, Processo* p){
    p->proximo_no = NULL;
    f->ultimo_no->proximo_no = p;

    if(FilaVazia(f)){
        f->primeiro_no = f->ultimo_no;
    }
    f->num_elementos++;
}


// Processo* RemoveProcesso(Fila* f){
//     if(FilaVazia(f)){
//         return -1;
//     }
//     int ret_pid;

//     Processo* retirado = f->primeiro_no;

//     f->primeiro_no = f->primeiro_no->proximo_no;
//     f->num_elementos--;
    
//     return retirado;
// }

void MataProcesso(Processo* p){// necessario??
    free(p);
}

void atualizaProcesso(Processo* p, enum status status) {
    p->status = status;
}


Processo* ExecutaProcesso(Fila* f){
    if(FilaVazia(f)){
        return -1;
    }

    Processo* executado = f->primeiro_no;
    
    f->primeiro_no = f->primeiro_no->proximo_no;

    f->ultimo_no->proximo_no = executado;
    executado->proximo_no = NULL;
    f->ultimo_no = executado;
    return executado;
}

