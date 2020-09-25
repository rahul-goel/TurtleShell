#ifndef SIGNAL_HANDLE_H
#define SIGNAL_HANDLE_H


void check_bg_process(int sig);
//void interrupt_handler(int signum);
//void stop_handler(int signum);
void parent_signal();
void child_signal();

#endif
