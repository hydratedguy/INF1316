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
    char programa[9];
    Processo* proximo_no;
};

Processo* executando;

void IO_Handler(int sinal){
    // InsereProcesso(fila_espera,);
    printf("Processo %d entrou em espera\n", executando->programa);
    sleep(3);
    

    // escalonador tira o processo de execucao
    // processo fica em estado de espera por 3 segundos
    // ao fim dos 3 segundos, processo manda um sinal
    // pro escalonador
    // escalonador coloca o processo no fim da fila

}
void ALARM_HANDLER(int signal);
void CHILD_HANDLER(int signal);


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
        sleep(QUANTUM);
		kill(pid, SIGSTOP);
	}
	return executando->pid;
}


int main (){
    char* prog;
    
    int id, pid, segmento;
    
    segmento = shmget (IPC_PRIVATE, 10 * sizeof (char*), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (char *) shmat (segmento, 0, 0);
    
    char ultimo_prog[] = "programaxx";
    strcpy(prog, ""); 

    signal(SIGUSR1, IO_HANDLER);

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
        Fila* fila_prontos = CriaFila();
        Fila* fila_espera = CriaFila();  
        char prog_exec[22];
        int pid;
        while(1) {
            if (strlen(prog) != 0){  // Adiciona processo à lista de prontos
                printf("Colocando %s na fila\n", prog);

                Processo* processo = CriaProcesso(1, prog); // Criacao da struct processo - pid default 1
                InsereProcesso(fila_prontos, CopiaProcesso(processo));
                // printa_fila(fila_prontos);
                strcpy(ultimo_prog, prog);
                strcpy(prog, "");

            }
            if(fila_espera->num_elementos > 0){
                Processo* sai_espera = fila_espera->primeiro_no;
                if(sai_espera -> status == 1){
                    printf("Colocando %s na fila de prontos\n", sai_espera->programa);
                    InsereProcesso(fila_prontos, CopiaProcesso(sai_espera));
                    // printa_fila(fila_prontos);
                }
            }
            if (fila_prontos->num_elementos > 0){
                executando = RemoveProcesso(fila_prontos);

                if (executando->status == 0){ //  Inicializa novo processo
                    char nome[9];
                    strcpy(nome,executando->programa);
                    printf("%s vai ser inicializado\n",nome);//ta dando erro n sei pq
                    strcpy(prog_exec, "./");
                    strcat(prog_exec, nome);
                    char* args[2]= {prog_exec, NULL};

                    pid = InicializaProcesso(args); // execucao do fork()
                    if(pid < 0){
                        executando = NULL;
                    } else {
                        executando->pid = pid;
                        executando->status = 1;
                        InsereProcesso(fila_prontos, CopiaProcesso(executando));
                    }
                    printa_fila(fila_prontos);
                    
                } else if (executando->status == 1 ) { // Continua execução do processo
                    int b = kill(executando->pid, 0);
                    printf("processo %s vai continuar\n", executando->programa);
                    kill(executando->pid, SIGCONT);
                    sleep(QUANTUM);
                    kill(executando->pid, SIGSTOP);

                    printf("processo %s vai para fim da fila\n", executando->programa);
                    
                    if (executando != NULL){
                        InsereProcesso(fila_prontos, CopiaProcesso(executando)); 
                    }
                    printa_fila(fila_prontos);
                } else {
                    break;
                }
            }
        }
	}

    shmdt (prog);
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}

void ALARM_HANDLER(int signal) {
    // Processo* processo_removido = RemoveProcesso(fila_espera);
    // if (processo_removido) {
    //     InsereProcesso(fila_prontos, CopiaProcesso(processo_removido));
    // }
    // free(processo_removido);
    printf("sigttou\n");
}
