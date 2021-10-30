#include <stdlib.h>
#include <stdio.h>

// IO BOUND

int main(){
    while (1){
        printf("programa4 mandará sinal\n");
        kill(getppid(), SIGUSR1); 
        printf("programa4 mandou sinal\n");
    }    
}