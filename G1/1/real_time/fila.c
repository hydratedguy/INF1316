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
    char status; //
    char programa[9];
    char* ti;
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

int checa_string(char* item){ //verifica se item é tempo ou dependencia
    char* aux = strtok(item,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return strlen(item) == strlen(aux);
}


Processo* CopiaProcesso(Processo* p){
	Processo *novo;
    if (checa_string(p->ti)){
        novo = CriaProcesso( p->ti, p->duracao,p->programa, NULL); // Criacao da struct processo - pid default 1
    }else{
        novo = CriaProcesso(-1, p->duracao,p->programa, p->ti); // Criacao da struct processo - pid default 1
    }
	novo->proximo_no = NULL;
	return novo;
}

Fila* CriaFila(){
    Fila* f = (Fila*) malloc (sizeof( Fila));
    f->num_elementos = 0;
    f->primeiro_no = NULL;
    f->ultimo_no = NULL;
    return f;
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

Processo* RemoveProcesso(Fila* f){
    
    if(f->num_elementos <= 0) {
        printf("num elementos da fila = %d\n", f->num_elementos);
        return NULL;
    }

    Processo* retirado = f->primeiro_no;

    if (f->num_elementos > 1) {
        f->primeiro_no = retirado->proximo_no;
        retirado->proximo_no = NULL;

    } else {

        f->primeiro_no = NULL;
        f->ultimo_no = NULL;
    }

    f->num_elementos--;
    
    return retirado;
}


////////////////////////////////////////////////

Processo* buscaprocessonome(Fila* f, char* buscado) {
	Processo* p = f->primeiro_no;

	while (p != NULL) {
		if (strcmp(p->programa,buscado) == 0) {
			return p;
		}
		p = p->proximo_no;
	}
	return NULL;
}
Processo* buscaprocessoti(Fila* f, int buscado) {
	Processo* p = f->primeiro_no;

	while (p != NULL) {
		if (p->ti,buscado == 0) {
			return p;
		}
		p = p->proximo_no;
	}
	return NULL;
}

void InsereMeio(Fila* f, Processo* p, Processo* novo){
    Processo* seguinte =CopiaProcesso(p->proximo_no);
    if(seguinte==NULL){
        InsereFinal(f,novo);
    }else{
        novo->proximo_no = seguinte;
        f->num_elementos++;
    }
}
