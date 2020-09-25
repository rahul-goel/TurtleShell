#include "header.h"
#include "overkill.h"
#include "bg_proc_list.h"

void overkill(char *command) {
    char *buf = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);

    char *token;
    int token_cnt = 0;

    token = strtok(buf, " \t\r\n");
    while (token != NULL) {
        token_cnt++;
        token = strtok(NULL, " \t\r\n");
        
    }

    if (token_cnt != 1) {
        fprintf(stderr, "Syntax error in overkill command.\n");
        PROMPTSTATUS = 1;
        free(buf);
        return;
    }

    while (cnt_bg_proc > 0) {
        if (kill(bg_proc_list[cnt_bg_proc - 1].pid, SIGKILL) < 0) {
            perror("overkill");
        }
    }

    free(buf);
}
