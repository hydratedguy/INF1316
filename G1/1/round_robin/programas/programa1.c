#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// IO BOUND

int main(){
    while (1){
        printf("programa1 requisitou I/O\n");
        kill(getppid(), SIGUSR1); 
    }
}