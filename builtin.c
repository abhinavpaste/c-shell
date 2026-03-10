#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

int handle_builtin(char **args) {

    // created this to specifically handle exit and cd as it was not working , ive explained the detail in the readme file.
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // cd
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