#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

void penjumlahan(int n){
    int z=0;

    for(int x=1; x<=n; x++){
        z = z+x;
    }

    printf("%d", z);
}
void main(){
        key_t key = 1234;
        int *p;
        int arr[20];
        int hasil, i, j;
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0644);
        p = shmat(shmid, NULL, 0);

        pthread_t tid[20];
        int count;
        for (int i = 0;i < 20;i++){
                arr[i]=*p;
                hasil=arr[i];
                pthread_create(&tid[count],NULL, &penjumlahan, (void*)hasil);
                count++;
                sleep(2);
                if((i+1)%5==0){
                        printf("\n");
                } else {
                        printf("\t");
                }
        }
        for(int i=0; i<count; i++){
            pthread_join(tid[i],NULL);
        }
        shmdt(p);
        shmctl(shmid, IPC_RMID, NULL);
}