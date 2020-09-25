#include "header.h"
#include "cd.h"
#include "pwd.h"

void cd(char *path) {
    char *token = (char *) malloc(sizeof(char) * (strlen(path) + 5));
    char *remember_token = token;
    strcpy(token, path);
    token = strtok(token, " \t\n\r");
    
    char *store_LWD = (char *) malloc(strlen(LWD) * sizeof(char));
    strcpy(store_LWD, LWD);
    strcpy(LWD, PWD);

    // if the absolute path through root is given
    token = strtok(NULL, " \t\n\r");
    if (token[0] == '/') {
        chdir("/");
    }
    getcwd(PWD, sizeof PWD);

    // reset the token
    token = remember_token;
    strcpy(token, path);
    token = strtok(token, " \t\n\r");
    token = strtok(NULL, "/\n\t\r");


    // store the PWD to return incase of error
    char store_PWD[2048];
    strcpy(store_PWD, PWD);

    while (token != NULL) {
        if (strcmp(token, "-") == 0) {
            chdir(store_LWD);
            pwd();
        } else if (strcmp(token, "~") == 0) {
            chdir(HOME);
        } else if (chdir(token) == -1) {
            chdir(store_PWD);
            perror("Error");
            free(remember_token);
            free(store_LWD);
            return;
        }
        token = strtok(NULL, "/\n\t\r");
    }

    getcwd(PWD, sizeof PWD);
    free(remember_token);
    free(store_LWD);
    return;
}

/*

int main() {
    getcwd(PWD, sizeof PWD);
    getcwd(HOME, sizeof HOME);
    while (1) {
        char *command = (char *) malloc(1024 * sizeof(char));
        size_t n = 1024;
        getline(&command, &n, stdin);
        cd(command);
    }
    return 0;
}

*/

