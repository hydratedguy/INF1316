#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>

#QUANTUM 1


int IOBound(){
    while (true){
        
    }
    
}

int CPUBOUND(){
    while(true){

    }
}





int main (){
    
    shared = shmget (IPC_PRIVATE, 16*sizeof (char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    pthread_t p1,p2;


    pthread_create(&p1,NULL,CPUBOUND,(void*)p);
    pthread_create(&p2,NULL,IOBound,(void*)p);

    while (1){
        if(){

        }
    }

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_exit(NULL);
    return 0;
}