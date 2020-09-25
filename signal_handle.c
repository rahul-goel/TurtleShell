#include "header.h"
#include "signal_handle.h"
#include "bg_proc_list.h"
#include "prompt.h"

// argument is sig just for the sake of it i guess;
// syntax for the functional call from signal;
void check_bg_process(int sig) {
    int status;
    // -1 means any child process;
    // since signal has been raised, then the process would have exited;
    // return value will give the id of the just completed process;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0){
        char *pname = (char *) malloc(sizeof(char) * 1024);
        find_processname(pid, pname);
        if (WIFEXITED(status)) {
            char out[100];
            memset(out, '\0', sizeof out);
            sprintf(out, "\nProcess %s with pid %d exited normally.\n", pname, pid);
            write(2, out, sizeof out);
        } else {
            char out[100];
            memset(out, '\0', sizeof out);
            sprintf(out, "\nProcess %s with pid %d did not exit normally.\n", pname, pid);
            write(2, out, sizeof out);
        }
        remove_bg_proc(pid);
        free(pname);
        prompt();
        fflush(stdout);
    }
}

//void interrupt_handler(int signum) {
//    if (getpid() == SHELLID) {
//        printf("\n");
//    } else {
//        kill(getpid(), SIGINT);
//    }
//    prompt();
//    fflush(stdout);
//}

void parent_signal() {
    signal(SIGCHLD, check_bg_process);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

void child_signal() {
    signal(SIGCHLD, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}

//void stop_handler(int signum) {
//    fprintf(stderr, "reached here.\n");
//    if (getpid() != SHELLID) {
//        fprintf(stderr, "reached here. pid is %d, shell pid is %d\n", getpid(), SHELLID);
//        kill(getpid(), SIGSTOP);
//        pid_t pid = getpid();
//        char *pname = find_proc_name((int) pid);
//        struct bg_proc temp;
//        temp.pid = pid;
//        strcpy(pname, temp.pname);
//        insert_bg_process(&temp);
//        free(pname);
//    }
//}
