#include "header.h"
#include "pwd.h"

void pwd() {
    if (getcwd(PWD, 1024) == NULL) {
        perror("Error in retrieving the present working directory.");
        PROMPTSTATUS = 1;
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
