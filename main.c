#include "header.h"
#include "ls.h"
#include "execute_interface.h"
#include "execute.h"
#include "prompt.h"
#include "ctrl_c.h"
#include "history.h"
#include "bg_proc_list.h"
#include "signal_handle.h"

void initialise();
void print_open_msg();
void terminate();
int middle();
void global_assign();

int main() {
    initialise();
    parent_signal();

    do {
        prompt(0);
        fflush(stdout);
    } while (middle());
    terminate();
    return 0;
}

int middle() {
    char *buf;
    size_t n = 0;
    int ret = getline(&buf, &n, stdin);
    if (ret == -1) {
        return 0;
    }
    add_to_history(buf);
    int flag = execute(buf);
    free(buf);
    return flag;
}

void initialise() {
    global_assign();
    if (gethostname(HOST, 1024 * sizeof(HOST)) == -1) {
        perror("Error in retrieving hostname.");
        exit(1);
    }
    if (getlogin_r(USER, 1024 * sizeof(USER)) != 0) {
        perror("Error in retrieving username.");
        exit(1);
    }
    if (getcwd(HOME, 1024 * sizeof(HOME)) == NULL) {
        perror("Error in finding the Current Working Directory");
        exit(1);
    }
    strcpy(LWD, HOME);
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

void terminate() {
    free(HOST);
    free(USER);
    free(PWD);
    free(LWD);
    free(HOME);
    free(SHELL);
    free(HOST_str);
    free(USER_str);
    free(PWD_str);
    free(HOME_str);
    free(SHELL_str);

    printf(BRED);
    printf("\nWhat did The Big Bang Theory fans say after completing the Operating Systems Assignment...?\n.\n.\n.\nSheldon!\n\n");
    printf(WHT);
}

void global_assign() {
    cnt_bg_proc = 0;
    SHELLID = getpid();
    errno = 0;

    HOST = (char *) malloc(sizeof (char) * 1024);
    USER = (char *) malloc(sizeof (char) * 1024);
    PWD = (char *) malloc(sizeof (char) * 1024);
    LWD = (char *) malloc(sizeof (char) * 1024);
    HOME = (char *) malloc(sizeof (char) * 1024);
    SHELL = (char *) malloc(sizeof (char) * 1024);
    HOST_str = (char *) malloc(sizeof (char) * 1024);
    USER_str = (char *) malloc(sizeof (char) * 1024);
    PWD_str = (char *) malloc(sizeof (char) * 1024);
    HOME_str = (char *) malloc(sizeof (char) * 1024);
    SHELL_str = (char *) malloc(sizeof (char) * 1024);

    memset(HOST, '\0', sizeof(char) * 1024);
    memset(USER, '\0', sizeof(char) * 1024);
    memset(PWD, '\0', sizeof(char) * 1024);
    memset(LWD, '\0', sizeof(char) * 1024);
    memset(HOME, '\0', sizeof(char) * 1024);
    memset(SHELL, '\0', sizeof(char) * 1024);
    memset(HOST_str, '\0', sizeof(char) * 1024);
    memset(USER_str, '\0', sizeof(char) * 1024);
    memset(PWD_str, '\0', sizeof(char) * 1024);
    memset(HOME_str, '\0', sizeof(char) * 1024);
    memset(SHELL_str, '\0', sizeof(char) * 1024);

    strcpy(SHELL, "TurtleShell");
    strcpy(HOST_str, "HOST");
    strcpy(USER_str, "USER");
    strcpy(PWD_str, "PWD");
    strcpy(HOME_str, "HOME");
    strcpy(SHELL_str, "SHELL");
}
