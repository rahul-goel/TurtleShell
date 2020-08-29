#include "header.h"

void cd(char *path) {
    if (path[0] == '~') {
        chdir(HOME);
        if (strlen(path) > 2) {
            path = path + 2;
        }
    }

    if (chdir(path) == -1) {
        perror("Error: Specified Directory does not exist.\n");
        return;
    }

    getcwd(PWD, sizeof PWD);
    printf("%s\n", PWD);
}

/*
int main() {
    getcwd(HOME, sizeof HOME);
    char str[] = "~/juicy/fruit pudding";
    cd(str);
    return 0;
}
*/

