#include "launch.h"
#include <stdio.h>
int yash_launch(char **argv)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0)
    {
    //Child process
        if(execvp(argv[0],argv) == -1)
        {
            perror("yash");
        }
        exit(EXIT_FAILURE);
    }
    if(pid < 0)
    {
        perror("yash");
        return -1;
    }
    do{
        wpid = waitpid(pid, &status, WUNTRACED);
    }while(!WIFEXITED(status) && !WIFSIGNALED(status));

    return -1;
}


