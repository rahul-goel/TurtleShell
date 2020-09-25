#include "header.h"
#include "bg_proc_list.h"
#include "fg.h"

void fg(char *command) {
    char *buf = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);
    char *token;

    // syntax check
    int cnt_token = 0;
    token = strtok(buf, " \t\r\n");
    while (token != NULL) {
        cnt_token++;
        token = strtok(NULL, " \t\r\n");
    }
    if (cnt_token != 2) {
        fprintf(stderr, "Error : Syntax error in fg command\n");
        free(buf);
        PROMPTSTATUS = 1;
        return;
    }

    // check validity of process
    strcpy(buf, command);
    token = strtok(buf, " \t\r\n");
    token = strtok(NULL, " \t\r\n");
    int job_pos = atoi(token);
    if (job_pos < 1 || job_pos > cnt_bg_proc) {
        fprintf(stderr, "Error : Invalid Job ID\n");
        free(buf);
        PROMPTSTATUS = 1;
        return;
    }


    pid_t pid = bg_proc_list[job_pos - 1].pid;

    // ignore the input output in the shell
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // Change the foreground process group
    tcsetpgrp(STDIN_FILENO, getpgid(pid));

    // Send continue signal to the process group
    if (kill(pid, SIGCONT) < 0) {
        perror("fg kill:");
    }

    // remove the process from the bg_proc_list
    remove_bg_proc(pid);

    // Now wait for the recently foreground brought process to complete
    int status;
    waitpid(pid, &status, WUNTRACED);


    // restore the shell
    tcsetpgrp(STDIN_FILENO, getpgid(0));

    // restore the default action when input/output signal is raised
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    // if it is sent to background again, reinsert it
    if (WIFSTOPPED(status)) {
        struct bg_proc temp;
        temp.pid = pid;
        char *pname = find_proc_name(pid);
        strcpy(temp.pname, pname);
        free(pname);
        insert_bg_process(&temp);
    }

    free(buf);
}
