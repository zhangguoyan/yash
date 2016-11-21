#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "launch.h"
#define YASH_RL_BUFSIZE 1024
#define YASH_TOK_BUFSIZE 64
#define YASH_TOK_DELIM " \t\r\n\a"
#define YASH_TOK_PIPE "|"


void yash_loop();
char * yash_read_line();
char** yash_split_line(char *line);
int yash_execute(char** args);
int _yash_cd(char **args);
int _yash_help(char** args);
int _yash_exit(char** args);
int _yash_number_builtins();

char *builtin_str[]={ "cd",
                      "help",
                      "exit"
};

int (*builtin_func[])(char **) = {
    &_yash_cd,
    &_yash_help,
    &_yash_exit
};

//char * previousPath = getenv("PWD");
//char * currentPath = getenv("PWD");


int main(int argc, char * argv[])
{
    yash_loop();
    return EXIT_SUCCESS;
}

void yash_loop()
{
    char *line;
    char **args;
    int status;
    do{
        printf("yash#");
        line = yash_read_line();
        args = yash_split_line(line);
        status = yash_execute(args);
        //printf("the start previous is %s\n",previousPath);
        //printf("the start current is %s\n",currentPath);
  

        free(line);
        free(args);
     }while(status);
}

char * yash_read_line()
{
    int bufsize = YASH_RL_BUFSIZE;
    int position = 0;
    char *buffer = (char*)malloc((sizeof(char) * bufsize));
    int c;
    if(!buffer)
    {
        fprintf(stderr, "yash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(true)
    {
        c = getchar();
        if(c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        buffer[position] = c;
        position++;

        if(position >= bufsize)
        {
            bufsize += YASH_RL_BUFSIZE;
            buffer = (char*)realloc(buffer,bufsize);
            if(!buffer)
            {
                fprintf(stderr, "yash: allocation error\n");
                exit(EXIT_FAILURE);
           
            }
        }
    }
}
//TODO:support delim of "" and backslash
char** yash_split_line(char *line)
{
    int bufsize = YASH_TOK_BUFSIZE, position = 0;
    char** tokens = (char**)malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens)
    {
        fprintf(stderr, "yash: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line,YASH_TOK_DELIM);
    while(token != NULL)
    {
        tokens[position] = token;
        position++;
        if(position >= bufsize)
        {
            tokens = (char**)realloc(tokens, bufsize * sizeof(char *));
            if(!tokens)
            {
                fprintf(stderr, "yash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL,YASH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


int yash_execute(char** args)
{
    int i;
    if(args[0] == NULL)
    {
        return 1;
    }    
    for(i = 0; i < _yash_number_builtins(); i++)
    {
        if(strcmp(args[0], builtin_str[i]) == 0)
        {
            return builtin_func[i](args);
        }
    }
    return yash_launch(args);
}


int _yash_number_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int _yash_cd(char** args)
{
    char *path = NULL;
    if(args[1] == NULL)
    {
        fprintf(stderr, "yash:expected argument to \"cd \"\n");
    }
    else
    {
        if(args[1][0] == '~')
        { 
            path = getenv("HOME");
        }
        //else if (args[1][0] == '-')
        //{
        //   printf("the - effect\n");
        //   path = previousPath;
        //   printf("in - the path is %s\n",path);
        //   if(currentPath != previousPath)
        //   {   
        //       char * temp = previousPath;
        //       previousPath = currentPath;
        //       currentPath = temp;
        //     
        //   }
        //}
        else
        {
           path = args[1];
        }
        
        if(chdir(path) != 0)
        {
            perror("yash");

        }
        //else
        //{
        //    currentPath = path;
        //    printf("the currentPath is %s\n",currentPath);
        //}
    }
    return 1;
}

int _yash_help(char **args)
{
    int i;
    printf("Guoyan's YASH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for(i = 0; i < _yash_number_builtins(); i++)
    {
        printf(" %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs\n");
    return 1;
}

int _yash_exit(char** args)
{
    return 0;
}

