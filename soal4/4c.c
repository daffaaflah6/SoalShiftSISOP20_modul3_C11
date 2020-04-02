#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 
	int fd[2];
	pid_t p; 

    char *arg_ls[] = {"ls", NULL};
    char *arg_wc[] = {"wc", "-l", NULL};

	if (pipe(fd)==-1){ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	p = fork(); 

	if (p < 0){ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	}  else if (p == 0){ 

		close(0);
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);

        execv("/bin/ls", arg_ls);

	} else { 

		close(0);
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);

        execv("/usr/bin/wc", arg_wc);
		exit(0); 
	} 
} 