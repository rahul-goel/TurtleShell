#include "header.h"
#include "cd.c"
#include "ls.c"
#include "echo.c"

// command and argv will be filled with the command and argv
// malloc before calling execvp_parse and execvp and the free them
// arg_cnt should be zero for input. through it the number of args will be given out
void execvp_parse(char *input, char *command, char **arg, int* arg_cnt) {
    char *buf = (char *) malloc(sizeof (input) + 10);
    strcpy(buf, input);
    char *token = buf;
    char *remember_token = token;

    token = strtok(token, " \t\r\n");
    strcpy(command, token);
    while (token != NULL) {
        strcpy(arg[*arg_cnt], token);
        (*arg_cnt)++;
        token = strtok(NULL, " \t\r\n");
    }
    arg[*arg_cnt] = NULL;

    free(remember_token);
}


void run_excvp(char *buf) {
    char *command = (char *) malloc(1024 * sizeof(char));
    char **arg = (char **) malloc(100 * sizeof(char *));
    int arg_cnt = 0;
    for (int i = 0; i < 100; i++) {
        arg[i] = (char *) malloc(1024 * sizeof (char));
    }

    execvp_parse(buf, command, arg, &arg_cnt);
    int pid = fork();
    if (pid < 0) {
        perror("Error");
    } else if (pid == 0) {
        if (execvp(command, arg) < 0) {
            // did not execute correctly so i need to free the memory
            perror("Error : No such command.");
            for (int i = 0; i < 100; i++) {
                free(arg[i]);
            }
            free(arg);
            free(command);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (status < 0) {
            perror("Error");
        }
    }

    for (int i = 0; i < 100; i++) {
        free(arg[i]);
    }
    free(arg);
    free(command);
}

int check_last_ampersand(char *line) {
    char *buf = (char *) malloc(sizeof(line));
    strcpy(buf, line);
    char *token = buf;
    char *remember_token = token;
    char *last_token = NULL;
    
    token = strtok(token, " \t\r\n"); 
    while (token != NULL) {
        last_token = token;
        token = strtok(NULL, " \t\r\n");
    }

    int flag;
    if (last_token == NULL || strcmp(last_token, "&")) {
        flag = 1;
    } else {
        flag = 0;
    }

    free(remember_token);
    return flag;
}

int execute(char *line) {
    if (line[0] == '\n') {
        return 1;
    }

    char *buf = (char *) malloc(sizeof(line));
    strcpy(buf, line);
    char *token = buf;
    char *remember_token = token;

    token = strtok(token, " \t\r\n");
    if (strcmp(token, "exit") == 0) {
        return 0;
    }

    if (check_last_ampersand(line)) {

    }

    if (strcmp(token, "cd") == 0) {
        cd(line);
    } else if (strcmp(token, "ls") == 0) {
        ls(line);
    } else if (strcmp(token, "echo") == 0) {
        echo(line);
    } else {
        run_excvp(line);
    }


    free(remember_token);
    return 1;
}
