#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// CPU BOUND

int main(){
    for(int i = 0;; i++){
        if (100%i==0){
            printf("programa1 executando; tempo = %d\n", i);
        }
    }
    return 0;
}