#include "header.h"
#include "prompt.h"

void prompt() {
    if (getcwd(PWD, sizeof(PWD) * 1024) == NULL) {
        perror("Error in finding the Current Wokring Directory.\n");
        exit(1);
    } else {
        char *occ = strstr(PWD, HOME);
        if (occ) {
            printf(BBLU);
            printf("<%s@%s:~%s> ", USER, HOST, occ + strlen(HOME));
            printf(WHT);
        } else {
            printf(BBLU);
            printf("<%s@%s:%s> ", USER, HOST, PWD);
            printf(WHT);
        }
        fflush(stdout);
    }
}
