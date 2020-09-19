#include "header.h"
#include "env_var.h"

void do_setenv(char *command) {
    char *buf = (char *)malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);

    char *token;
    token = strtok(buf, " \t\r\n");
    int token_cnt = 0;
    while (token != NULL) {
        token_cnt++;
        token = strtok(NULL, " \t\r\n");
    }

    if (token_cnt < 2 || token_cnt > 3) {
        fprintf(stderr, "Error : Syntax error for setenv command\n");
        free(buf);
        return;
    }

    char *name = NULL;
    char *val = NULL;
    strcpy(buf, command);

    token = strtok(buf, " \t\r\n");
    token = strtok(NULL, " \t\r\n");
    name = (char *) malloc(sizeof(char) * (5 + strlen(token)));
    strcpy(name, token);
    token = strtok(NULL, " \t\r\n");
    if (token != NULL) {
        val = (char *) malloc(sizeof(char) * (5 + strlen(token)));
        strcpy(val, token);
    } else {
        val = "";
    }

    if (setenv(name, val, 1)) {
        perror("Error in setenv:");
    }

    free(buf);
}

void do_unsetenv(char *command) {
    char *buf = (char *)malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);

    char *token;
    token = strtok(buf, " \t\r\n");
    int token_cnt = 0;
    while (token != NULL) {
        token_cnt++;
        token = strtok(NULL, " \t\r\n");
    }

    if (token_cnt != 2) {
        fprintf(stderr, "Error : Syntax error for setenv command\n");
        free(buf);
        return;
    }

    strcpy(buf, command);
    token = strtok(buf, " \t\r\n");
    token = strtok(NULL, " \t\r\n");

    if (unsetenv(token) == -1) {
        perror("Error in unsetenv: ");
    }

    free(buf);
}
