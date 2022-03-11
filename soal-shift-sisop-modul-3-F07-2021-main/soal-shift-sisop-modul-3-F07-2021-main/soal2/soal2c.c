#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid;
    int pipeing[2][2];
    
    if(pipe(pipeing[0]) == -1)
    {
        perror("failed pipe");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1)
    {
        perror("failed fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        dup2(pipeing[0][1], 1);
        close(pipeing[0][0]);
        close(pipeing[0][1]);
        execlp("ps", "ps", "-aux", NULL);
        exit(EXIT_SUCCESS);
    }

    if(pipe(pipeing[1]) == -1)
    {
        perror("failed pipe");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1)
    {
        perror("failed fork");
        exit(EXIT_FAILURE);
    }else if(pid == 0)
    {
        dup2(pipeing[0][0], 0);
        dup2(pipeing[1][1], 1);
        close(pipeing[0][0]);
        close(pipeing[0][1]);
        close(pipeing[1][0]);
        close(pipeing[1][1]);
        execlp("sort", "sort", "-nrk", "3,3", NULL);
        perror("failed sort");
        exit(EXIT_SUCCESS);
    }

    close(pipeing[0][0]);
    close(pipeing[0][1]);

    if((pid = fork()) == -1)
    {
        perror("failed fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        dup2(pipeing[1][0], 0);
        close(pipeing[1][0]);
        close(pipeing[1][1]);
        execlp("head", "head", "-5", NULL);
        perror("failed head");
        exit(EXIT_SUCCESS);
    }
}
