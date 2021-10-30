#include <stdlib.h>
#include <stdio.h>

// IO BOUND

int main(){
    while (1){
        printf("programa2 mandará sinal\n");
        kill(getppid(), SIGUSR1); 
        printf("programa2 mandou sinal\n");
    }    
}