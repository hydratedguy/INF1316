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
#include "timer.h"

// Processo* executando;


typedef struct Processo Processo;
typedef struct Fila Fila;

enum status {NOVO, PRONTO, ESPERA, EXECUTANDO};
typedef struct Fila Fila;
typedef struct Processo Processo;



typedef struct Infoprog Infoprog;



void IOHandler(int sinal, int pid, Processo* p){
    printf("Processo %d entrou em espera\n", pid);
    alarm(3);
    

    // escalonador tira o processo de execucao
    // processo fica em estado de espera por 3 segundos
    // ao fim dos 3 segundos, processo manda um sinal
    // pro escalonador
    // escalonador coloca o processo no fim da fila

}


// Cria um novo processo a partir do escalonador e retorna o pid dele.
// int InicializaProcesso (char** args) {
// 	struct timeval start, stop, overall_t1, overall_t2; 
//     gettimeofday (&overall_t1,NULL);
//     int pid;
//     gettimeofday(&start,NULL);
        
// 	if((pid = fork()) < 0) { // Erro
// 		printf("Erro ao criar processo filho.\n");
//     		exit(-1);

//     } else if (pid == 0) { // Filho
//         printf("Executando %s\n", executando->programa);
//         execv(executando->programa, args);

// 	} else { // Pai
//         executando->pid = pid;

//         gettimeofday(&stop,NULL);
//         printf("Tempo de exec %s : %.10f \n",executando->programa,timedifference_msec(start,stop));


// 	}
// 	return executando->pid;
// }

struct Infoprog{
    char nome[9];
    char* ti;
    int duracao;
};

int main (){
    
    Infoprog* prog;
    
    int id, pid, segmento;

    segmento = shmget (IPC_PRIVATE, sizeof (Infoprog*), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (Infoprog*) shmat (segmento, 0, 0);
    strcpy( prog->nome , "programax");
    prog->ti= "P1";
    prog->duracao= -1;
    printf("%s %s %d\n", prog->nome, prog->ti, prog->duracao);

    if ((id = fork()) < 0) { // Cria INTERPRETADOR e ESCALONADOR
        
		printf ("Erro na criação do novo processo\n");
		exit (-2);

	} else if (id == 0) { // INTERPRETADOR (filho)
        FILE* programas;
        programas = fopen("programas.txt", "r");
        if (programas == NULL){
            printf("error ao abrir progrmas\n");
            exit(0);
        }
        while (!feof(programas)) {
            int i;
            fscanf(programas, " Run < %s > I=< %s > D=< %d > \n", prog->nome, prog->ti, &i);
            printf("lido %s %s %d\n", prog->nome, prog->ti, i);
            sleep(1);
        }
        fclose(programas);

	} else { // ESCALONADOR (pai)
        // Fila* fila_prontos = CriaFila();
        // Fila* fila_espera = CriaFila();  
        // char prog_exec[11];
        // struct timeval start, stop; 
        // int pid;
        while(1) {            
            // if (strlen(prog) != 0){  // Adiciona processo à lista de prontos
            //     printf("Colocando %s na fila\n", prog);

            //     Processo* processo = CriaProcesso(1, prog); // Criacao da struct processo - pid default 1

            //     InsereProcesso(fila_prontos, processo);
            //     printa_fila(fila_prontos);
            //     strcpy(ultimo_prog, prog);
            //     strcpy(prog,"");

            // }
            // if(fila_espera->num_elementos>0){
            //     Processo* sai_espera = fila_espera->primeiro_no;
            //     if(sai_espera -> status == 1){
            //         printf("Colocando %s na fila de prontos\n", sai_espera->programa);
            //         InsereProcesso(fila_prontos, sai_espera);
            //         printa_fila(fila_prontos);
            //     }
            // }
            // if (fila_prontos->num_elementos > 0){
            //     executando = RemoveProcesso(fila_prontos);

            //     if (executando->status == 0){ //  Inicializa novo processo
            //         char nome[9];
            //         strcpy(nome,executando->programa);
            //         printf("%s vai ser inicializado\n",nome);//ta dando erro n sei pq
            //         strcpy(prog_exec, "./");
            //         strcat(prog_exec, nome);
            //         char* args[2]= {prog_exec, NULL};

            //         pid = InicializaProcesso(args); // execucao do fork()
            //         printf("pid pos inicializado %d\n",pid);
            //         if(pid < 0){
            //             executando = NULL;
            //         }else{
            //             executando->pid = pid;
            //             executando->status = 1;
            //             InsereProcesso(fila_prontos, CopiaProcesso(executando));
            //         }
            //         printa_fila(fila_prontos);
                    
            //     } else if (executando->status == 1 ) { // Continua execução do processo
            //         int b = kill(executando->pid, 0);
            //         printf("processo %s vai continuar\n",executando->programa);
            //         gettimeofday(&start,NULL);
            //         kill(executando->pid, SIGCONT);
            //         sleep(QUANTUM);
            //         kill(executando->pid, SIGSTOP);
            //         gettimeofday(&stop,NULL);
            //         printf("Tempo de exec 1: %.10f \n",timedifference_msec(start,stop));

            //         printf("processo %s vai para fim da fila\n",executando->programa);
                    
            //         if (executando != NULL){
            //             InsereProcesso(fila_prontos, CopiaProcesso(executando)); 
            //         }
            //         printa_fila(fila_prontos);
            //     } else {
            //         break;
            //     }
            // }
        }
	}

    shmdt (prog);
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}
