#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <string.h>
#include "fila.h"
#include <errno.h>
#include "timer.h"

#define QUANTUM 1


typedef struct processo Processo;
typedef struct fila Fila;

struct fila { 
    int num_elementos;
    Processo* primeiro_no;
    Processo* ultimo_no;
};

struct processo {
    int pid;
    enum status status;
    char programa[10];
    Processo* proximo_no;
};

Processo* executando;
Fila* fila_espera;
Fila* fila_prontos;

void IO_HANDLER(int signal);

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
        sleep(QUANTUM);
		kill(pid, SIGSTOP);

	}
	return pid;
}


int main (){
    char* prog;
    
    int id, pid, segmento;
    
    segmento = shmget (IPC_PRIVATE, 10 * sizeof (char*), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (char *) shmat (segmento, 0, 0);
    
    char ultimo_prog[] = "programaxx";
    strcpy(prog, ""); 

    signal(SIGUSR1, IO_HANDLER);
    signal(SIGALRM, IO_HANDLER);

    fila_espera = CriaFila();
    fila_prontos = CriaFila();
    if ((id = fork()) < 0) { // Cria INTERPRETADOR e ESCALONADOR
        
		printf("Erro na criação do novo processo\n");
		exit (-2);

	} else if (id == 0) { // INTERPRETADOR (filho)
        FILE* programas;
        char* path = "exec.txt";
        programas = fopen(path, "r");

        while (!feof(programas)) {
            fscanf(programas, "Run < %s > ", prog);
            sleep(1);
        }
        fclose(programas);

	} else { // ESCALONADOR (pai)
        char prog_exec[12];
        int pid;
        while(1) {
            if (strlen(prog) != 0){  // Adiciona processo à lista de prontos
                printf("Colocando %s na fila\n", prog);

                Processo* processo = CriaProcesso(1, prog); // Criacao da struct processo - pid default 1
                InsereProcesso(fila_prontos, CopiaProcesso(processo));
                strcpy(ultimo_prog, prog);
                strcpy(prog, "");

            }

            if (fila_prontos->num_elementos > 0){
                executando = NULL;
                executando = RemoveProcesso(fila_prontos);

                if (executando->status == 0){ //  Inicializa novo processo
                    char nome[10];
                    strcpy(nome, executando->programa);
                    printf("%s vai ser inicializado\n", nome);
                    strcpy(prog_exec, "./");
                    strcat(prog_exec, nome);
                    char* args[2]= {prog_exec, NULL};

                    pid = InicializaProcesso(args); // execucao do fork()
                    executando->pid = pid;

                    if (executando->status != 2) {
                        executando->status = 1;
                        InsereProcesso(fila_prontos, CopiaProcesso(executando));
                    }

                } else if (executando->status == 1) { // Continua execução do processo
                    printf("processo %s vai continuar\n", executando->programa);
                    kill(executando->pid, SIGCONT);
                    sleep(QUANTUM);
                    if (executando->status != 2) {
                        kill(executando->pid, SIGSTOP);
                        printf("processo %s vai para fim da fila\n", executando->programa);
                        
                        if (executando != NULL){
                            InsereProcesso(fila_prontos, CopiaProcesso(executando)); 
                        }
                    }
                } else {
                    continue;
                }
                printa_fila(fila_prontos);
                printf("-----------------------------------------------------\n");
            }
        }
	}

    shmdt (prog);
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}

void IO_HANDLER(int signal) {
    // Processo* processo_removido = RemoveProcesso(fila_espera);
    if (signal == SIGUSR1) {
        kill(executando->pid, SIGSTOP);
        executando->status = 2;
        printf("%s entrou na fila de espera\n", executando->programa);
        InsereProcesso(fila_espera, CopiaProcesso(executando));
        alarm(3);
    } else if (signal == SIGALRM) {
        Processo* retirado = RemoveProcesso(fila_espera);
        printf("%s terminou seu IO\n", retirado->programa);
        retirado->status = 1;
        InsereProcesso(fila_prontos, CopiaProcesso(retirado));
    }
}
