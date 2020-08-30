#include "header.h"

void pwd() {
    if (getcwd(PWD, sizeof PWD) == NULL) {
        perror("Error in retrieving the present working directory.");
        return;
    }
    char *occ = strstr(PWD, HOME);
    if (occ) {
        printf("~/%s\n", occ + strlen(HOME));
    } else {
        printf("%s\n", PWD);
    }
}
/*
int main() {
    getcwd(PWD, sizeof PWD);
    getcwd(HOME, sizeof HOME);
    pwd();
}
*/
