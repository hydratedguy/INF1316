#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Fila Fila;
typedef struct Processo Processo;
enum status {NOVO, PRONTO};

struct Fila {
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
};

struct Processo {
    int pid;
    char status; 
    char programa[9];
    int dependencia;
    int ti;
    int duracao;
    Processo* proximo_no;
};


void printa_fila(Fila* f, char* nome){
    printf("Fila %s\n",nome);
    for (Processo* p = f->primeiro_no; p != NULL; p = p->proximo_no) {
        printf("%s, ", p->programa);
    }
    printf("\n");
}

Processo* CriaProcesso(int ti, int d, char* programa, Processo* dependencia){
    Processo* new = (Processo*) malloc (sizeof(Processo));
    new->pid = 1;
    new->status = NOVO;
    strcpy(new->programa, programa);
    new->duracao = d;

    if(dependencia == NULL){
        new->ti = ti;
        new->dependencia = 0;
    }
    else{
        new->ti = (dependencia->ti + dependencia->duracao) + 1;
        new->dependencia = 1;
    }
    new->proximo_no = dependencia;
    return new;
}

int checa_string(char* item){ //verifica se item é tempo ou dependencia
    char* aux = strtok(item, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return strlen(item) == strlen(aux);
}


Processo* CopiaProcesso(Processo* p){
	Processo *novo;
    novo = CriaProcesso(p->ti, p->duracao,p->programa, p->proximo_no); 
	return novo;
}

Fila* CriaFila(){
    Fila* f = (Fila*) malloc (sizeof( Fila));
    f->num_elementos = 0;
    f->primeiro_no = NULL;
    f->ultimo_no = NULL;
    return f;
}


////////////////////////////////////////////////

Processo* BuscaProcessoNome(Fila* f, char* buscado) {
	Processo* p = f->primeiro_no;

	while (p != NULL) {
		if (strcmp(p->programa, buscado) == 0) {
			return p;
		}
		p = p->proximo_no;
	}
	return NULL;
}


Processo* BuscaProcessoTI(Fila* f, int buscado) {
	Processo* p = f->primeiro_no;

	while (p != NULL) {
		if (p->ti == buscado) {
			return p;
		}
		p = p->proximo_no;
	}
	return NULL;
}

void InsereFinal(Fila* f, Processo* p){
    if (p){
        Processo* new = CopiaProcesso(p);

        if (f->num_elementos >= 1) {
            f->ultimo_no->proximo_no = new;
        } else {
            f->primeiro_no = new;
        }
        f->ultimo_no = new;
        f->num_elementos++;
    }

}

void InsereApos(Fila* f, Processo* p, Processo* novo) {
    if (f->num_elementos == 0){ return;}
        
    Processo* seguinte = CopiaProcesso(p->proximo_no);
 
    p->proximo_no = novo;
    novo->proximo_no = seguinte;
    f->num_elementos++;
    
    
}


void InsereProcesso(Fila* f, Processo* novo){
    novo->proximo_no = NULL;
    if(f->num_elementos == 0){
        if (novo->ti + novo->duracao <= 60) {
            f->primeiro_no = novo;
            f->ultimo_no = novo;
            f->num_elementos++;
            return;
        }
    }
    Processo* atual;
    Processo* proximo = f->primeiro_no->proximo_no;

    if((novo->ti + novo->duracao) < f->primeiro_no->ti) { // Inserir antes do inicio da fila
        novo->proximo_no = f->primeiro_no;
        f->primeiro_no = novo;
        f->num_elementos++;
        return;

    } else {
        for(atual = f->primeiro_no; proximo != NULL; atual = atual->proximo_no) { // Inserir no meio
            int fim_atual = atual->ti + atual->duracao, ini_prox = proximo->ti;
            if(novo->ti < proximo->ti && proximo->dependencia == 0 && novo->ti > fim_atual && novo->ti < ini_prox ) {
                InsereApos(f, atual, novo);
                return;
            }
        proximo = proximo->proximo_no;
        }
    }

    if(novo->ti > f->ultimo_no->ti && novo->ti >= f->ultimo_no->ti + f->ultimo_no->duracao + 1 && novo->ti + novo->duracao <= 60) { // Inserir no fim
        InsereFinal(f, novo);
        return;
    }
    
    printf("O processo %s nao foi escalonado!\n", novo->programa);
    free(novo);
    
}


void ArrumaFila(Fila* f1, Fila* f2) {
    if(!f1 || !f2){return;}

    Processo* p1, *p2;


    for (p2 = f2->primeiro_no; p2 != NULL; p2 = p2->proximo_no) {
        p1 = p2;
        InsereProcesso(f1, CopiaProcesso(p1));

    }

}