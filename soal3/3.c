#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#define max 100

struct stat st;
struct dirent *drnt;

char *filename(char str[]){
    char *file = strchr(str, '/');
    char *nama;
    if(file==NULL){
        return str;
    } 

    while(file!=NULL){
        nama = file+1;
        file = strchr(nama, '/');
    }
    return nama;
}

char *extention(char str[]){
    char *file = filename(str);
    char *ekst = strchr(file, '.');

    if(ekst==NULL){
        return NULL;
    } else {
        return ekst+1;
    }
}

void* pindah(void *arg){

    char buf[1000];
    char *wd = getcwd(buf, 1000);

    char *dir = (char*)arg;
    char *ekstensi = extention((char*)arg);
    char ext[300];
    char dirname[300];

    strcpy(ext, ekstensi);
    
    for(int i=0;i<strlen(ext);i++){
        ext[i]=tolower(ext[i]);
    }

    if(ext == NULL){
        strcpy(wd, dirname);
        strcat(dirname, "/Unknown");

        if (stat(dirname, &st) == -1 && S_ISDIR(st.st_mode)) {
            mkdir(dirname, 0755);
        }

        char *args[]= {"mv", dir, dirname, NULL};
        execv("/bin/mv", args);
    } else {
        strcpy(wd, dirname);
        strcat(dirname, "/");
        strcat(dirname, ext);

        if (stat(dirname, &st) == -1 && S_ISDIR(st.st_mode)) {
            mkdir(dirname, 0755);
        }

        char *args1[]= {"mv", dir, dirname, NULL};
        execv("/bin/mv", args1);
    }
}

int main(int argc, char **arg){

    int i=2;
    pthread_t tid[max];

    if(strcmp(arg[1], "-f") == 0){
        while(arg[i] != NULL){
            pthread_create(&tid[i-2], NULL, &pindah, (void *)arg[i]);
            i++;
        }

        for(int j=0; j<i-1; j++){
            pthread_join(tid[j], NULL);
        }
    } else {
        DIR *dir;
        char directory[200];

        if ((strcmp(arg[1], "*")) == 0){

        } else if ((strcmp(arg[1], "-d")) == 0){
            dir = opendir(arg[2]);
            strcpy(directory, arg[2]);
        }

        if(dir){
            while(drnt = readdir(dir)){

            char namafile[100];

            strcpy(namafile, directory);
            strcat(namafile, "/");
            strcat(namafile, drnt->d_name);

            if(S_ISREG(st.st_mode) == 0 && stat(namafile, &st) == 0){
                pthread_create(&tid[i-2], NULL, &pindah, (void *)namafile);
                i++;
            }

            for(int j=0; j<i-1; j++){
                pthread_join(tid[j], NULL);
            }
        }
        }
        
    }

        
}