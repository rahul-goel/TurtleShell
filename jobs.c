#include "header.h"
#include "bg_proc_list.h"
#include "jobs.h"

char find_process_status(pid_t pid) {
    char *file_path = (char *) malloc(sizeof(char) * 512);
    sprintf(file_path, "/proc/%d/stat", (int) pid);

    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error in opening the status file of a background process\n");
        free(file_path);
        return '\0';
    } else {
        char *buf;
        size_t buf_size = 0;
        if (getline(&buf, &buf_size, f) == -1) {
            fprintf(stderr, "Error in reading the status file of a background process\n");
            free(buf);
            free(file_path);
            return '\0';
        }
        char *token = strtok(buf, " \t\n\r");
        for (int i = 0; i < 2 && token != NULL; i++) {
            token = strtok(NULL, " \t\n\r");
        }
        char ret = token[0];
        free(file_path);
        free(buf);
        return ret;
    }
}

int check_job_validity(char *command) {
    char *buf = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);

    int cnt_token = 0;
    char *token = strtok(buf, " \t\n\r");
    while (token != NULL) {
        cnt_token++;
        token = strtok(NULL, " \t\n\n");
    }

    free(buf);

    return cnt_token == 1;
}

void jobs(char *command) {
    if (!check_job_validity(command)) {
        fprintf(stderr, "Error in syntax of jobs.\n");
        return;
    }

    for (int i = 0; i < cnt_bg_proc; i++) {
        char status = find_process_status(bg_proc_list[i].pid);
        char *status_name = (char *) malloc(sizeof(char) * 20);
    
        if (status == 'T') {
            strcpy(status_name, "Stopped");
        } else {
            strcpy(status_name, "Running");
        }

        printf("[%d] %s %s [%d]\n", i + 1, status_name, bg_proc_list[i].pname, bg_proc_list[i].pid);
        free(status_name);
    }
}
