#include "kjob.h"
#include "header.h"
#include "bg_proc_list.h"

void kjob(char *command) {
    char *buf = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);

    int token_cnt = 0;
    char *token = strtok(buf, " \t\r\n");
    while (token != NULL) {
        token_cnt++;
        token = strtok(NULL, " \t\r\n");
    }

    if (token_cnt != 3) {
        fprintf(stderr, "Error : Syntax for kjob is wrong. It should have exactly 2 arguments.\n");
        free(buf);
        return;
    }

    int job_pos, signal_id;
    strcpy(buf, command);
    token = strtok(buf, " \t\r\n");
    token = strtok(NULL, " \t\r\n");
    job_pos = atoi(token);
    token = strtok(NULL, " \t\r\n");
    signal_id = atoi(token);

    if (job_pos < 1 || job_pos > cnt_bg_proc) {
        fprintf(stderr, "Error : Wrong job ID entered\n"); 
    } else {
        if (kill(bg_proc_list[job_pos - 1].pid, signal_id) == -1) {
            fprintf(stderr, "Error : Could not send the signal to the process. The following could be the reasons.\nInvalid Signal Argument.\n Permission Denied to send the signal.\n No such proces found.\n");
        }
    }


    free(buf);
}
