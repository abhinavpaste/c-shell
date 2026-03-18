#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pipe.h"

#define MAX_ARGS 64

static void tokenize(char *cmd, char **args) {
    int i = 0;
    args[i] = strtok(cmd, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

int parse_pipe(char *input, char **left, char **right) {
    char *pipe_pos = strchr(input, '|');
    if (pipe_pos == NULL) return 0;

    *pipe_pos = '\0';
    *left  = input;
    *right = pipe_pos + 1;
    return 1;
}

void execute_pipe(char *left, char *right) {
    char *args_left[MAX_ARGS];
    char *args_right[MAX_ARGS];
    int fd[2];
    pid_t pid1, pid2;

    while (*right == ' ') right++;   // trim leading space

    tokenize(left,  args_left);
    tokenize(right, args_right);

    if (pipe(fd) == -1) {
        perror("pipe failed");
        return;
    }

    // child 1 — left command
    pid1 = fork();
    if (pid1 == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        if (execvp(args_left[0], args_left) == -1) {
            perror(args_left[0]);
            exit(1);
        }
    }

    // child 2 — right command
    pid2 = fork();
    if (pid2 == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        if (execvp(args_right[0], args_right) == -1) {
            perror(args_right[0]);
            exit(1);
        }
    }

    // parent closes both ends
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}