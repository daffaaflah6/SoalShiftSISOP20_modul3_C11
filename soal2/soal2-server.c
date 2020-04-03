#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <termios.h>
#define PORT 8080

pthread_mutex_t lock;
struct ident{
  char user[100];
  char passw[100];
};

int pemain = 0, koneksi_pemain = 0;
int sock_id[2];
int HP1 = 100, HP2 = 100;

void sc2Handler(int new_socket);
void* sc1Handler (void* args);
void playHandler()

int main(int argc, char const *argv[]){
    int server_fd, new_socket, val;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t thread[5];
    for(;;){
        addrlen = sizeof(address);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("connected! %d\n", pemain);

        int *p_client_socket = (int *) malloc(2 * sizeof(int));
        sock_id[pemain] = new_socket;
    }
    return 0;
}

void sc2Handler(int new_socket){
  printf("new game--------------------\n");
  HP1 = 100;
  HP2 = 100;

  int val;
  char findOrLogout[100] = "";
  val = read( new_socket , findOrLogout, 1024);
  if(strcmp(findOrLogout, "find") == 0){
    koneksi_pemain++;
    while(koneksi_pemain < 2){
      //wait
      char mess[100] = "Waiting for player ...";
      send(new_socket , mess , strlen(mess) , 0 );
      usleep(100000);
    }
    char mess[100] = "mulai";
    send(new_socket , mess , strlen(mess) , 0 );

    char menang[100] = "End Game, You WIN";
    while(true){
      char hit[100], hitSend[100] = "hit";
      val = read( new_socket , hit, 1024);
      if(HP2 <= 0 || HP1 <= 0)
        break;
      if(new_socket == sock_id[0]){
        HP2 = HP - 10;
        if(HP2 <= 0){
          strcpy(hitSend, "End Game, You WIN");
          send(sock_id[1] , hitSend , strlen(hitSend) , 0 );
          send(sock_id[0] , menang , strlen(menang) , 0 );
          break;
        }
        //send to 1
        printf("hit\n");
        send(sock_id[1] , hitSend , strlen(hitSend) , 0 );
      }
      else if (new_socket == sock_id[1]){
        HP1 = HP - 10;
        if(HP1 <= 0){
          strcpy(hitSend, "End Game, You LOSE");
          send(sock_id[0] , hitSend , strlen(hitSend) , 0 );
          send(sock_id[1] , menang , strlen(menang) , 0 );
          break;
        }
        //send to 0
        printf("hit\n");
        send(sock_id[0] , hitSend , strlen(hitSend) , 0 );
      }
    }
  }
  else if(strcmp(findOrLogout, "logout") == 0){
    sc1Handler(&new_socket);
  }
  else{
    sc2Handler(new_socket);
    return;
  }
  sleep(1);
  koneksi_pemain = 0;
  sc2Handler(new_socket);
}

void* sc1Handler (void* args){
    int val, new_socket = *(int*)args;
    char messSer [100] = "", loginRegister[100] = "", user[100] = "", pass[100] = "";
    struct ident id;
    val = read( new_socket , loginRegister, 1024);
    if(!strcmp(loginRegister, "login") == 0 && !strcmp(loginRegister, "register") == 0){
        sc1Handler(&new_socket);
        pthread_cancel(pthread_self());
    }
    val = read( new_socket , user, 1024);
    val = read( new_socket , pass, 1024);

    strcpy(id.user, user);
    strcpy(id.passw, pass);
    printf("%s\n", id.user);
    printf("%s\n", id.passw);

    FILE *f;
    f = fopen("akun.txt", "a+");
    if(strcmp(loginRegister, "login") == 0)
    {
        struct ident cek_ID;
        bool loginSuccess = false;
        char loginCheck[100] = "";
        while(fread(&cek_ID,sizeof(struct ident), 1, f)){
            if(strcmp(id.user, cek_ID.user) == 0 && strcmp(id.passw, cek_ID.passw) == 0){
                printf("Auth Success\n");
                loginSuccess = true;
                strcat(loginCheck, "Login Success");
                break;
            }
        }
        if(!loginSuccess){
            printf("Auth Failed\n");
            strcat(loginCheck, "Login Failed");
            send(new_socket , loginCheck , strlen(loginCheck) , 0 );
            sc1Handler(&new_socket);
            pthread_cancel(pthread_self());
        }
        else
        send(new_socket , loginCheck , strlen(loginCheck) , 0 );
        printf("login\n");
    }
    else if(strcmp(loginRegister, "register") == 0){
        pthread_mutex_lock(&lock);

        printf("regis\n");

        printf("List account :\n");
        struct ident cek_ID;
        while(fread(&cek_ID,sizeof(struct ident), 1, f)){
            printf("Username: %s\nPassw: %s\n\n", cek_ID.user, cek_ID.passw);
        }
        printf("Username: %s\nPassw: %s\n\n", id.user, id.passw);
        char regis[100] = "Register Success";
        send(new_socket , regis , strlen(regis) , 0 );
        fwrite(&id, sizeof(struct ident), 1, f);

        pthread_mutex_unlock(&lock);
    }
    else{
        strcat(messSer, "Command Not Found");
    }
    fclose(f);
    sc2Handler(new_socket);
}

void playHandler(){

}