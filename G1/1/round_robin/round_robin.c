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
    char* programa;
    Processo* proximo_no;
};

void IOHandler(int sinal, int pid, Processo* p){
    printf("Processo %d entrou em espera\n", pid);
    alarm(3);
    

    // escalonador tira o processo de execucao
    // processo fica em estado de espera por 3 segundos
    // ao fim dos 3 segundos, processo manda um sinal
    // pro escalonador
    // escalonador coloca o processo no fim da fila

}
void ALARM_HANDLER(int signal);



// Cria um novo processo a partir do escalonador e retorna o pid dele.
int InicializaProcesso (char* nome_programa, char** args) {
	
    int pid;

	if((pid = fork()) < 0) { // Erro
		printf("Erro ao criar processo filho.\n");
    		exit(-1);

    } else if (pid == 0) { // Filho
        printf("Executando %s\n", nome_programa);
        execv(nome_programa, args);

	} else { // Pai
		kill(pid, SIGSTOP);
		// sleep(1);
	}

	return pid;
}
// struct prog_nome{
//     char* nome[10];
// };

int main (){
    // struct prog_nome* prog = (struct prog_nome *) malloc (sizeof(struct prog_nome));
    char* prog;
    char prog_exec[11];
    char ultimo_prog[] = "programax";
    int id, pid, segmento;

    segmento = shmget (IPC_PRIVATE,10*sizeof (char*), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (char *) shmat (segmento, 0, 0);
    
    FILE* programas;
    programas = fopen("programas.txt", "r");
    Fila* fila_prontos = CriaFila();
    Fila* fila_espera = CriaFila();
    strcpy(prog, ultimo_prog);

    if ((id = fork()) < 0) { // Cria INTERPRETADOR e ESCALONADOR
        
		printf ("Erro na criação do novo processo\n");
		exit (-2);

	} else if (id == 0) { // INTERPRETADOR (filho)

        while (!feof(programas)) {
            fscanf(programas, "Run < %s > ", prog);
            printf("lido %s\n",prog);
            sleep(1);
        }

	} else { // ESCALONADOR (pai)
            
        while(1) {
            if (strcmp(prog, ultimo_prog) != 0){  // Adiciona processo à lista de prontos
                printf("Colocando %s na fila\n", prog);
                // strcpy(prog_exec, prog);

                Processo* processo = CriaProcesso(1, prog); // Criacao da struct processo - pid default 1
                // printf("ult %s prog %s\n",ultimo_prog,prog);

                InsereProcesso(fila_prontos, processo);
                sleep(1);
                printa_fila(fila_prontos);
                strcpy(ultimo_prog, prog);

            }
            if(fila_espera->num_elementos>0){
                if(fila_espera->primeiro_no->status == 1){
                    printf("Colocando %s na fila\n", prog);
                    strcpy(prog_exec, prog);

                    Processo* processo = CriaProcesso(1, prog_exec); // Criacao da struct processo - pid default 1
                    // printf("ult %s prog %s\n",ultimo_prog,prog);

                    InsereProcesso(fila_prontos, processo);
                    sleep(1);
                    printa_fila(fila_prontos);
                    strcpy(ultimo_prog, prog);
                }
            }
            if (fila_prontos->num_elementos > 0){
                Processo* processo_executado;
                if (fila_prontos->primeiro_no->status == 0){ //  Inicializa novo processo
                    processo_executado = RemoveProcesso(fila_prontos);

                    strcpy(prog_exec, "./");
                    strcat(prog_exec, prog);
                    char* args[2]= {prog_exec, NULL};
                    pid = InicializaProcesso(prog, args); // execucao do fork()
                    processo_executado->pid = pid;
                    processo_executado->status = 1;

                    InsereProcesso(fila_prontos, CopiaProcesso(processo_executado));
                    printa_fila(fila_prontos);
                    
                } else if (fila_prontos->primeiro_no->status == 1) { // Continua execução do processo
                    processo_executado = RemoveProcesso(fila_prontos);
                    printf("retorno %d\n", kill(processo_executado->pid, 0));
                    if(kill(processo_executado->pid, SIGCONT) == -1) {
                        free(processo_executado);
                    } else {
                        sleep(1);
                        kill(processo_executado->pid, SIGSTOP);
                        if(signal(SIGUSR1,ALARM_HANDLER) == SIG_ERR) {
                            printf("aquiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
                        }
                        InsereProcesso(fila_prontos, CopiaProcesso(processo_executado)); 
                        printa_fila(fila_prontos);
                        // signal(IOHandler();
                    }
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
