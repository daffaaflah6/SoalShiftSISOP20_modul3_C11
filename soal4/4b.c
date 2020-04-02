#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int penjumlahan[20];
int p;

void fak(void *arg){
    int i, jumlah=0, j, array;
    for(i=0; i<20; i++){
        array = *(p+i); 
        for(j=0; j<=p; j++){
            jumlah=jumlah+j;
        }
        penjumlahan[i]=jumlah;
    }
}

int main(){
    key_t key = 1234;
    int shmid = shmget(key, 20*sizeof(int), IPC_EXCL);
    p = (int*)shmat(shmid, 0, 0);

    pthread_t tid;

    pthread_create(&tid, NULL, &fak, NULL);
    pthread_join(tid, NULL);

    for(int i=0; i<20; i++){
        printf("%d", penjumlahan[i]);
    }

    shmdt((void*) p);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}