#include <stdio.h>
#include <signal.h>
#include "signal_handler.h"

//wrote it so that when ctr+C is pressed instead of terminatiing the entire shell(which would lose access to the shell 
//we just continue in our shell at a new line)
void handle_sigint(int sig) {
    printf("\n");
    fflush(stdout);
}

void setup_signals() {
    signal(SIGINT,  handle_sigint);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

//this are when the child process runs , the child process , we expect to run the default behaviour
void reset_child_signals() {
    signal(SIGINT,  SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}