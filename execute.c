#include "header.h"
#include "cd.c"
#include "ls.c"
#include "echo.c"
#include "pinfo.c"

// argument is sig just for the sake of it i guess;
// syntax for the functional call from signal;
void check_bg_process(int sig) {
    int status;
    // -1 means any child process;
    // since signal has been raised, then the process would have exited;
    // return value will give the id of the just completed process;
    int pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0){
        if (WEXITSTATUS(status)) {
            char out[100];
            memset(out, '\0', sizeof out);
            sprintf(out, "\nProcess with pid %d exited normally.\n", pid);
            write(2, out, sizeof out);
        } else {
            char out[100];
            memset(out, '\0', sizeof out);
            sprintf(out, "\nProcess with pid %d did not exit normally.\n", pid);
            write(2, out, sizeof out);
        }
    }
}

// command and argv will be filled with the command and argv
// malloc before calling execvp_parse and execvp and the free them
// arg_cnt should be zero for input. through it the number of args will be given out
void execvp_parse(char *input, char *command, char **arg, int* arg_cnt) {
    char *buf = (char *) malloc(strlen(input) + 10);
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

void run_excvp_bg(char *buf) {
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
        // change the child process group so that it happens in the background..
        setpgid(0, 0);
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
    }

    for (int i = 0; i < 100; i++) {
        free(arg[i]);
    }
    free(arg);
    free(command);
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
    char *buf = (char *) malloc(strlen(line));
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
        flag = 0;
    } else {
        flag = 1;
    }

    free(remember_token);
    return flag;
}

void remove_last_ampersand(char *line) {
    int len = strlen(line);
    for (int i = len - 1; i >= 0; i--) {
        if (line[i] == '&') {
            line[i] = '\0';
            break;
        }
    }
}

int execute(char *line) {
    if (line[0] == '\n') {
        return 1;
    }

    char *buf = (char *) malloc(strlen(line));
    strcpy(buf, line);
    char *token = buf;
    char *remember_token = token;

    token = strtok(token, " \t\r\n");
    if (strcmp(token, "exit") == 0) {
        return 0;
    }

    if (check_last_ampersand(line)) {
        remove_last_ampersand(line);
        run_excvp_bg(line);
    } else if (strcmp(token, "pinfo") == 0) {
        pinfo(line);
    } else if (strcmp(token, "cd") == 0) {
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
