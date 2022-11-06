#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
printf("I'm shell process: pid(%d), I am running man ls | grep -e  \"-S \" -m 1  > output.txt command\n", (int) getpid());
    int input = dup(0);
    int output = dup(1);

    int pipe1[2];
    pipe(pipe1); 

    dup2(pipe1[1],1);
    close(pipe1[1]);
    int ch1 = fork();
    
    if (ch1 < 0) {
        printf("Fork failed!\n");
        exit(1);
    } else if (ch1 == 0) {
        //first child process
        close(pipe1[0]);
        char *myArgs[3];
        myArgs[0] = strdup("man");
        myArgs[1] = strdup("ls");
        myArgs[2] = NULL;
        execvp(myArgs[0], myArgs);

    }
    dup2(pipe1[0],0);
    close(pipe1[0]);
    int fd = open("output.txt", O_WRONLY|O_CREAT);
    dup2(fd ,1);
    close(fd);

    int ch2 = fork();
    if (ch2 < 0) {
        printf("Fork failed!\n");
        exit(1);
    } else if (ch2 == 0) {
        //first child process
            char *myArgs[5];
            myArgs[0] = strdup("grep");
            myArgs[1] = strdup("-e");
            myArgs[2] = strdup("-S");
            myArgs[3] = strdup("-m");
            myArgs[4] = strdup("1");
            execvp(myArgs[0], myArgs);
    }
    wait(NULL);
    dup2(input, 0);
    dup2(output, 1);
    printf("I'm shell process: pid(%d), I am running man ls | grep -e  \"-S \" -m 1  > output.txt command\n", (int) getpid());

    return 0;
}