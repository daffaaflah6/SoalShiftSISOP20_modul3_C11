#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int *p;
void multiplyMatrices(int matA[4][2], int matB[2][5], int multResult[4][5], int r1, int c1, int r2, int c2);


int main() {
    int matA[4][2], matB[2][5], mult[4][5];
    int r1 = 4, c1 = 2, r2 = 2, c2 = 5;

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    p = shmat(shmid, NULL, 0);

    matA[0][0]=1;
    matA[0][1]=2;
    matA[1][0]=1;
    matA[1][1]=2;
    matA[2][0]=1;
    matA[2][1]=2;
    matA[3][0]=1;
    matA[3][1]=2;

    matB[0][0]=1;
    matB[0][1]=1;
    matB[0][2]=1;
    matB[0][3]=1;
    matB[0][4]=1;
    matB[1][0]=1;
    matB[1][1]=1;
    matB[1][2]=1;
    matB[1][3]=1;
    matB[1][4]=1;

    multiplyMatrices(matA, matB, mult, r1, c1, r2, c2);

    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}


void multiplyMatrices(int matA[4][2], int matB[2][5], int mult[4][5], int r1, int c1, int r2, int c2) {
    int hasil = 0;
    
    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                hasil = hasil + matA[i][k] * matB[k][j];
            }

            mult[i][j] = hasil;
            hasil = 0;
        }
    }

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            *p = mult[i][j];
            sleep(1);
            printf("%d\n", *p);
        }
    }
}