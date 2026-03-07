#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include "signal_handler.h"

#define MAX_IP 1024

int handle_builtin(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            chdir(getenv("HOME"));
        } else {
            if (chdir(args[1]) != 0) {
                perror(args[1]);
            }
        }
        return 1;
    }
    return 0;
}

int main(){
    char input[MAX_IP];
    char *args[MAX_IP];
    pid_t pid;
    int status;
    
    //so what the while loop does is, it loops infinetly seeking input from the user , the starting if statements before
    //fork() are repsonsible for parsing the input string or the command the user enters

    setup_signals();
    while(1){
        printf("abhi-sh> ");
        fflush(stdout);

        if(fgets(input,MAX_IP,stdin)==NULL){
            printf("\n");
            exit(0); 
        }
        input[strcspn(input, "\r\n")] = 0;

        if (strlen(input) == 0) continue;

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }
         if (handle_builtin(args)) continue;
        //child process begins here

        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            continue;
        }

        if (pid == 0) {
            //for child
            reset_child_signals();
            if (execvp(args[0], args) == -1) {
                perror(args[0]);
                exit(1);
            }
        } else {
            // praents block
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}