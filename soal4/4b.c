 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

void *fak(int num){
    int i; 
    unsigned long long fact = 1;

    for (i = 1; i <= num; ++i) {
        fact *= i;
    }
    printf("%llu ", fact);

    pthread_exit(0);
}

int main() {
    int n, i, num, count = 0;
    pthread_t tid[20];

    key_t key = 1234;
    int *p;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    p = shmat(shmid, 0, 0);
    int arr[20];

    for(int j=0; j<20; j++){
        int *num = malloc(sizeof(*num));
        *num = p[i];
        pthread_create(&tid[count], NULL, fak, num);
        count++;
        sleep(1);
    }

    for(int i=0; i<20; i++){
        pthread_join(tid[i],NULL);
    }

    shmdt((void*) p);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}