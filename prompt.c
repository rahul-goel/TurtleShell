#include "header.h"
#include "prompt.h"

void prompt(int status) {
    if (getcwd(PWD, sizeof(PWD) * 1024) == NULL) {
        perror("Error in finding the Current Wokring Directory.\n");
        exit(1);
    } else {
        char *occ = strstr(PWD, HOME);
        if (occ) {
            if (status == 0 && PROMPTSTATUS == 0) {
                printf(BGRN);
                printf(":')");
                printf(BBLU);
                printf("<%s@%s:~%s> ", USER, HOST, occ + strlen(HOME));
            } else {
                printf(BRED);
                printf(":'(");
                printf(BBLU);
                printf("<%s@%s:~%s> ", USER, HOST, occ + strlen(HOME));
            }
            printf(WHT);
        } else {
            if (status == 0 && PROMPTSTATUS == 0) {
                printf(BGRN);
                printf(":')");
                printf(BBLU);
                printf("<%s@%s:%s> ", USER, HOST, PWD);
            } else {
                printf(BRED);
                printf(":'(");
                printf(BBLU);
                printf("<%s@%s:%s> ", USER, HOST, PWD);
            }
            printf(WHT);
        }
        fflush(stdout);
    }
    PROMPTSTATUS = 0;
}
