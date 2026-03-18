#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "signal_handler.h"
#include "builtin.h"
#include "pipe.h"              

#define MAX_INPUT 1024
#define MAX_ARGS  64

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *left, *right;
    pid_t pid;
    int status;

    setup_signals();
    //so what the while loop does is, it loops infinetly seeking input from the user , the starting if statements before
    //fork() are repsonsible for parsing the input string or the command the user enters
    while (1) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("navsh:%s> ", cwd);
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        input[strcspn(input, "\r\n")] = 0;
        if (strlen(input) == 0) continue;

        // pipe check
        if (parse_pipe(input, &left, &right)) {
            execute_pipe(left, right);
            continue;
        }

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (handle_builtin(args)) continue;

        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            continue;
        }

        if (pid == 0) {
            reset_child_signals();
            if (execvp(args[0], args) == -1) {
                perror(args[0]);
                exit(1);
            }
        } else {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}