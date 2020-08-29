#include "header.h"

void prompt();
void initialise();

int main() {
    initialise();
    prompt();
    return 0;
}

void prompt() {
    if (getcwd(PWD, sizeof PWD) == NULL) {
        perror("Error in finding the Current Wokring Directory.\n");
        exit(1);
    } else {
        char *occ = strstr(PWD, HOME);
        if (occ) {
            printf("\033[0;34m");
            printf("<%s@%s:~/%s> ", USER, HOST, occ + strlen(HOME));
            printf("\033[0m");
        } else {
            printf("\033[0;34m");
            printf("<%s@%s:%s> ", USER, HOST, PWD);
            printf("\033[0m");
        }
    }
}

void initialise() {
    if (gethostname(HOST, sizeof HOST) == -1) {
        perror("Error in retrieving hostname.");
        exit(1);
    }
    if (getlogin_r(USER, sizeof USER) != 0) {
        perror("Error in retrieving username.");
        exit(1);
    }
    if (getcwd(HOME, sizeof HOME) == NULL) {
        perror("Error in finding the Current Working Directory");
        exit(1);
    }
}
