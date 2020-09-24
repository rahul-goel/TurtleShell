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
        return;
    }


    pid_t cur_group_id = getpgid(0);
    if (setpgid(bg_proc_list[job_pos - 1].pid, cur_group_id) == -1) {
        perror("Error : ");
    }

    free(buf);
}
