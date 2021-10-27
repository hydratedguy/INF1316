#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// CPU BOUND

int main(){
    for(int i = 0;i < 10000; i++){
        printf("programa1 executando; tempo = %d\n", i);
    }
}