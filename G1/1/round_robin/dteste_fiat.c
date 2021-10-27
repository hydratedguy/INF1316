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





// int main(){
//     Fila* f = CriaFila();

//     Processo* p1, *p2, *p3;
//     Processo* rem;
//     p1 = CriaProcesso(1, "programa1");
//     p2 = CriaProcesso(2, "programa2");
//     p3 = CriaProcesso(3, "programa3");
    
//     // InsereProcesso();
//     printf("PROCESSO 1: %d, %d, %s\n", p1->pid, p1->status, p1->programa);
//     printf("PROCESSO 2: %d, %d, %s\n", p2->pid, p2->status, p2->programa);
//     printf("PROCESSO 3: %d, %d, %s\n", p3->pid, p3->status, p3->programa);

    
//     InsereProcesso(f, p1);
//     InsereProcesso(f, p2);
//     InsereProcesso(f, p3);

//     RemoveProcesso(f);

      
//     return 0;
// }
struct p{
    char* a[6];
}
int main (){
    char* prog[]= {"programa1", "programa2", "programa3", "programa4", "programa5","programa6"};
    int i = 0;
    int segmento;
    Fila* fila_prontos = CriaFila();
    segmento = shmget (IPC_PRIVATE, 6*sizeof (char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    prog = (struct p *) shmat (segmento, 0, 0);

    while(1) {
        printf("");
        if (i!=5){  // adiciona processo à lista de prontos
            Processo* processo = CriaProcesso(1, prog[i]); // Criacao da struct processo - pid default 1
            printf("a %d %s\n",i,prog->a[i]);
            InsereProcesso(fila_prontos, processo);

            i++;
            printa_fila(fila_prontos);

        }
        if (fila_prontos->num_elementos > 0){
            // Processo* processo_executado;
            if (fila_prontos->primeiro_no->status == 0){ //  Inicializa novo processo
                // processo_executado = RemoveProcesso(fila_prontos);
                // printf("b\n");
                // // strcpy(prog_exec, "./");
                // // strcat(prog_exec, prog);
                // // char* args[2]= {prog_exec, NULL};

                // // pid = CriaNovoProcesso(prog, args); // execucao do fork()
                // processo_executado->pid = 0;
                // processo_executado->status = 1;
                // InsereProcesso(fila_prontos, processo_executado);
                // printa_fila(fila_prontos);
                

            } else if (fila_prontos->primeiro_no->status == 1) { // Continua execução do processo
                // printf("c\n");
                // processo_executado = RemoveProcesso(fila_prontos);
                // // kill(processo_executado->pid, SIGCONT);
                // sleep(1);
                // // kill(processo_executado->pid, SIGSTOP);
                // InsereProcesso(fila_prontos, processo_executado);  
                // printa_fila(fila_prontos);
                
            } else {
                printf("d\n");
                break;
            }

        }
    }
    for (Processo* p = fila_prontos->primeiro_no; p != NULL; p = p->proximo_no) {
        Processo* aux = p;
        free(aux);
    }
    free(fila_prontos);
    shmdt (prog);
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}