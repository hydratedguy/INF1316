#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "fila.h"

#define QUANTUM 1

int p_id = 0;


// int executaProcesso(char* programa){
    
    
// }


void IOHandler(int sinal, int pid){
    printf("Processo %d entrou em espera\n", pid);
    
    sleep(3);

    // escalonador tira o processo de execucao
    // processo fica em estado de espera por 3 segundos
    // ao fim dos 3 segundos, processo manda um sinal
    // pro escalonador
    // escalonador coloca o processo no fim da fila

}



int main (){
    char* prog, prog_exec[13];
    int id, status, pid, segmento;
    segmento = shmget (IPC_PRIVATE, 9*sizeof (char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (char *) shmat (segmento, 0, 0);

    Fila* fila_prontos = CriaFila();
    Fila* fila_espera  = CriaFila(); //
    FILE* programas;
    Processo* processo_executado;

    
    programas = fopen("programas.txt", "r");

    if ((id = fork()) < 0) {
        
		puts ("Erro na criação do novo processo");
		exit (-2);

	} else if (id == 0) {
        // signal(SIGKILL, essa)
		pid = wait (&status);

        fscanf(programas, "Run < %s > ", prog);
        sleep(1);
        
	} else {
        
        printf ("ESCALONADOR\n");
        if (prog != NULL){
            Processo* processo = CriaProcesso(++p_id, prog);
            InsereProcesso(fila_prontos, processo);
        }

        // if(fila_espera != NULL){
        //     InsereProcesso(RemoveProcesso(fila_espera));
        // }
        
        if(!FilaVazia(fila_prontos)) {
            processo_executado = ExecutaProcesso(fila_prontos);
            
            strcpy(prog_exec, "./");
            strcat(prog_exec, prog);
            strcat(prog_exec, ".c");

            char* args[2]= {prog_exec, NULL};
            execv(prog, args);
            
            sleep(1);
            // kill(,SIG);            
            
            // if (IOHandler()){

            // }


        }        

        
	}
    shmdt (prog);
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}


