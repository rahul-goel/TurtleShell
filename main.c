#include "header.h"
#include "execute.c"

void prompt();
void initialise();
void print_open_msg();
void exit_shell();
int middle();

int main() {
    initialise();
    do {
        prompt();
    } while (middle());
    exit_shell();
    return 0;
}

int middle() {
    char *buf = (char *) malloc(2048 * sizeof(char));
    size_t n = 2048;
    getline(&buf, &n, stdin);
    return execute(buf);
    free(buf);
}

void prompt() {
    if (getcwd(PWD, sizeof PWD) == NULL) {
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
    print_open_msg();
}

void print_open_msg() {
    char str[128];
    FILE *fptr = NULL;
    if ((fptr = fopen("asciart.txt", "r")) == NULL) {
        return;
    }
    printf(BGRN);
    while (fgets(str, sizeof str, fptr) != NULL) {
        printf("%s", str);
    }
    printf(WHT);
    fclose(fptr);
}

void exit_shell() {
    printf(BGRN);
    printf("Listen kiddo...\n");
    printf("I don't have much time...\n");
    printf("The only way to respond to lol and lmao is...\n");
    printf(WHT);
}
