#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <termios.h>
#include <pthread.h>
#define PORT 8080

void sc1(int sock);
void sc2(int sock);
void* MatchTap(void* args);
void* MatchRead(void* args);

int health = 100;
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    sc1(sock);
    return 0;
}
void sc1(int sock){
    int val;
    char ch[100],user[100] = "", passw[100] = "";
    printf("1. Login\n2. Register\n    Choices : ");

    scanf("%s", ch);
    send(sock , ch , strlen(ch) , 0 );

    if(!strcmp(ch, "login") == 0 && !strcmp(ch, "register") == 0){
        sc1(sock);
        return;
    }

    printf("Username : ");
    getchar();
    scanf("%[^\n]s", user);
    send(sock , user , strlen(user) , 0 );

    printf("Password : ");
    getchar();
    scanf("%[^\n]s", passw);
    send(sock , passw , strlen(passw) , 0 );

    char mess[100] = "";
    val = read( sock , mess, 1024);
    printf("%s\n", mess);

    if(strcmp(mess, "Login Success") == 0 || strcmp(mess, "Register Success") == 0){
        sc2(sock);
    }
    else{
        sc1(sock);
    }
}

void sc2(int sock){
    health = 100;
    int val;
    char ch2[100];
    printf("1. Find Match\n2. Logout\n   Choices : " );
    scanf("%s", ch2);
    send(sock , ch2 , strlen(ch2) , 0 );

    if(strcmp(ch2, "logout") == 0){
        sc1(sock); //balik ke sc1
    }
    else if(strcmp(ch2, "find") == 0){
        char auth[100] = "";
        do {
            val = read( sock , auth, 1024);
            printf("Waiting for player ...\n");
        /* code */
        }
        while(strcmp(auth, "Waiting for player ...") == 0);

        printf("The game starts, please tap tap as fast as possible  !!!\n");
        //send signal loop waiting for signal back
        pthread_t thread[2];

        pthread_create(&thread[0], NULL, MatchTap, &sock);
        pthread_create(&thread[1], NULL, MatchRead, &sock);
        pthread_join(thread[1], NULL);
        printf("lanjut\n");

        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);//ngembalikan settingan stdin

        sc2(sock);
    }
}

void* MatchTap(void* args){
    tempThread = pthread_self();

    int sock = *(int *) args;
    int karakter;

    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.karakter_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
    while((karakter = getchar()) != '-'){
        if(karakter == ' '){
        char HP[10] = "hit";
        send(sock , HP , strlen(HP) , 0 );
        printf("hit !!\n");
        }
        putchar(karakter);
        if(health <= 0) break;
    }
    printf("\n");
}

void* MatchRead(void* args){
    int sock = *(int *) args, valread;
    char HR[100];
    while (health > 0){
        valread = read( sock , HR, 1024);
        if(strcmp(HR, "End Game, You LOSE") == 0)
        {
        printf("%s\n", HR);
        pthread_cancel(tempThread);
        send(sock , HR , strlen(HR) , 0 ); // send biar ga stuck
        break;
        }
        else if (strcmp(HR, "End Game, You WIN") == 0)
        {
        printf("%s\n", HR);
        pthread_cancel(tempThread);
        break;
        }
        health -= 10;
        printf("%d\n", health);
    }
}