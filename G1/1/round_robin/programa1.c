#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// IO BOUND

int main(){
    while (1){
        printf("programa3 requisitou I/O\n");
        kill(getppid(), SIGUSR1);
        sleep(2);
        printf("Executando programa 3\n");
        sleep(1);
    }
}