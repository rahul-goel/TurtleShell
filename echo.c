#include "header.h"
#include "echo.h"

int print_env_var(char *v) {
    v++;
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
        PROMPTSTATUS = 1;
        return -1;
    }
    return 0;
}

// takes in one argument from the command
// handles environment variables as well
void echo_single(char* str) {
    if (strcmp(str, "$") == 0) {
        printf("$");
    } else if(str[0] == '$') {
        print_env_var(str);
    } else {
        printf("%s ", str);
    }
}

// takes in the command which includes the echo keyword
void echo(char *line) {
    char *token = (char *) malloc(strlen(line) + 5);
    char *remember_token = token;
    strcpy(token, line);

    token = strtok(token, " \n\t\r");
    token = strtok(NULL, " \n\t\r");
    while (token != NULL) {
        echo_single(token);
        token = strtok(NULL, " \n\t\r");
    }
    printf("\n");
    free(remember_token);
}

/*
int main(int argv, char *argc[]) {
    char *str = (char *) malloc(1024 * sizeof(char));
    size_t n = 1024;;
    getline(&str, &n, stdin);
    echo(str);
    return 0;
}
*/
