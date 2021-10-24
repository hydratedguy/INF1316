#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
// #include <>
// #include "fila.h"

#define QUANTUM 1


int IO_Bound(){
    while (1){
        
    }
    
}

int CPU_BOUND(){
    while(1){

    }
}

void IOHandler(int sinal, int pid){
    printf("Processo %d entrou em espera\n", pid);
    // escalonador tira o processo de execucao
    // processo fica em estado de espera por 3 segundos
    // ao fim dos 3 segundos, processo manda um sinal
    // pro escalonador
    // escalonador coloca o processo no fim da fila

}



int main (){
    char* prog; char programa[16];
    int id, status, pid;
    prog = shmget (IPC_PRIVATE, 16*sizeof (char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    // Processo* processo;
    
    FILE* programas;
    programas = fopen("programas.txt", "r");

    if ((id = fork()) < 0) {
        
		puts ("Erro na criação do novo processo");
		exit (-2);

	} else if (id == 0) {
		printf ("ESCALONADOR\n");
	} else {
        // signal(SIGKILL, essa)
		pid = wait (&status);

        for (int i = 0; i < 4; i ++) {
            fscanf(programas, "Run < %s > ", programa);
            printf("prog=%s\n", programa);
        }

	}

    shmctl (prog, IPC_RMID, 0);
    return 0;
}


