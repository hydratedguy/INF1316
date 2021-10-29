#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/sem.h>
#include <string.h>
#include "fila.h"
#include <errno.h>


#define TEMPO_EXEC 60
int T_ATUAL = 0;
int ocupado = 0;

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
    int dependencia;
    int ti;
    int duracao;
    Processo* proximo_no;
};


Processo* executando;


typedef struct Infoprog Infoprog;

void Execucao(int ti){
    sleep(ti);
    T_ATUAL += ti;
    
}


// Cria um novo processo a partir do escalonador e retorna o pid dele.
int InicializaProcesso (char** args) {
    int pid;
        
	if((pid = fork()) < 0) { // Erro
		printf("Erro ao criar processo filho.\n");
    		exit(-1);

    } else if (pid == 0) { // Filho
        printf("Executando %s\n", executando->programa);
        execv(executando->programa, args);

	} else { // Pai
        executando->pid = pid;
        Execucao(executando->duracao);
        kill(SIGSTOP, pid);

	}
	return executando->pid;
}


char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}


struct Infoprog{
    char nome[10];
    char ti[9];
    int duracao;
};


int main (){
    
    Infoprog* prog;
    int id, pid, segmento, arrumaFila = 1;

    char prog_exec[11];

    segmento = shmget (IPC_PRIVATE, sizeof (Infoprog*), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (Infoprog*) shmat (segmento, 0, 0);
    strcpy(prog->nome , "");
    strcpy(prog->ti, "");
    prog->duracao= -1;

    if ((id = fork()) < 0) { // Cria INTERPRETADOR e ESCALONADOR
        
		printf ("Erro na criação do novo processo\n");
		exit (-2);

	} else if (id == 0) { // INTERPRETADOR (filho)
        FILE* programas;
        programas = fopen("programas.txt", "r");
        if (programas == NULL){
            printf("error ao abrir programas\n");
            exit(0);
        }
        while (!feof(programas)) {
            char line[80];
            
            fscanf(programas, "Run %[^\n] ", line);
            //Remove caracteres
            strremove(line, "<");
            strremove(line,"=");
            strremove(line,">");
            strremove(line,"I");
            strremove(line,"D");

            char* aux = strtok(line, " ");
            strcpy(prog->nome, aux);
            strcpy(aux, " ");

            aux = strtok(NULL, " ");
            strcpy(prog->ti, aux);
            strcpy(aux, " ");

            aux = strtok(NULL, " ");
            prog->duracao = atoi(aux);

            sleep(2);
        }
        fclose(programas);

	} else { // ESCALONADOR (pai)
        Fila* fila_prontos = CriaFila();
        Fila* fila_espera = CriaFila();  

        while(1) {
            sleep(1);
            printf("Tempo atual: %d\n", T_ATUAL);

            if (strcmp("", prog->nome) != 0){  // Adiciona processo à lista de prontos
                char new[10];
                char ti[9];
                int duracao = prog->duracao;
                strcpy(new, prog->nome);
                strcpy(ti, prog->ti);
        
                printf("Colocando %s I=%s D=%d na fila\n", new, ti, duracao);

                Processo* local;
                Processo* novo;
                if (atoi(ti) != 0){//verifica o tipo de inserção (se ti é numero ou outro processo)
                    novo = CriaProcesso(atoi(ti), duracao, new, NULL); // Criacao da struct processo - pid default 1
                } else {
                    local = BuscaProcessoNome(fila_prontos, ti);
                    novo = CriaProcesso(-1, duracao, prog->nome, local); // Criacao da struct processo - pid default 1
                }

                if(novo->ti >= T_ATUAL) {
                    InsereProcesso(fila_prontos, novo);
                } else if (novo->ti < T_ATUAL) {
                    printf("%s entrou na fila de espera\n", novo->programa);
                    InsereProcesso(fila_espera, novo);
                }
                printa_fila(fila_prontos, "Prontos");
                printa_fila(fila_espera, "Espera");

                strcpy(prog->nome, "");
            }

            if(arrumaFila && T_ATUAL >= TEMPO_EXEC) { // arruma a fila apos o primeiro minuto
                ArrumaFila(fila_prontos, fila_espera);
                arrumaFila = 0;
                printa_fila(fila_prontos, "Organizada");
            }


            if(fila_prontos->num_elementos == 1){ //inicialização de executando
                executando = fila_prontos->primeiro_no;
            }

            if(executando->ti + executando->duracao < T_ATUAL && fila_prontos->num_elementos > 1) {
                executando = fila_prontos->ultimo_no;
            }

            if (executando->ti == T_ATUAL){
                if (arrumaFila == 1 || executando->status == 0){ //  Inicializa novo processo
                    char nome[9];
                    strcpy(nome, executando->programa);
                    printf("%s vai ser inicializado\n", nome);
                    strcpy(prog_exec, "./");
                    strcat(prog_exec, nome);
                    char* args[2]= {prog_exec, NULL};

                    pid = InicializaProcesso(args); // execucao do fork()
                    executando->pid = pid;
                    executando->status = 1;
                    
                    if (executando == fila_prontos->ultimo_no) {
                        executando = fila_prontos->primeiro_no;
                    } else {
                        executando = executando->proximo_no;
                    }

                } else {
                    printf("Processo %s voltou a executar\n", executando->programa);
                    kill(executando->pid, SIGCONT);
                    Execucao(executando->duracao);
                    kill(executando->pid, SIGSTOP);
                    printf("Processo %s parou de executar\n", executando->programa);

                    if (executando != fila_prontos->ultimo_no) {
                        executando = executando->proximo_no;
                    }
                }
            } else {// Ocio 
                T_ATUAL += 1;
                printf("\n");
                sleep(1);
            }
            if (T_ATUAL == 61) {
                executando = fila_prontos->primeiro_no;
                T_ATUAL = 1;

            }
        }
	}

    shmdt (prog);
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}
