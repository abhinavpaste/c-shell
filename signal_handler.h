#ifndef SIGNAL_HANDLING_H
#define SIGNAL_HANDLING_H

void handle_sigint(int sig);
void setup_signals();
void reset_child_signals();

#endif