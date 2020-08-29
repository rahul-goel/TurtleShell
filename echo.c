#include "header.h"

int print_env_var(char *v) {
    if (strcmp(v, HOST_str) == 0) {
        printf("%s ", HOST);
    } else if (strcmp(v, USER_str) == 0) {
        printf("%s ", USER);
    } else if (strcmp(v, PWD_str) == 0) {
        printf("%s ", PWD);
    } else if (strcmp(v, HOME_str) == 0) {
        printf("%s ", HOME);
    } else if (strcmp(v, SHELL_str) == 0) {
        printf("%s ", SHELL);
    } else {
        perror("Error : No such environment variable is defined.\n");
        return -1;
    }
    return 0;
}

void echo(char *str[], int cnt) {
    for (int i = 0; i < cnt; i++) {
        if (*str[i] == '$') {
            if (print_env_var(str[i] + 1) == -1) {
                return;
            }
        } else {
            printf("%s ", str[i]);
        }
    }
}
