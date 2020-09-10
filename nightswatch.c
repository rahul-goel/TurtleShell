#include <termios.h>
#include <time.h>
#include "header.h"
#include "nightswatch.h"
#include <ncurses.h>

int kbhit(void) {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void nightswatch(char *line) {
    char *buf = (char *) malloc(sizeof(char) * (5 + strlen(line)));
    char *token = buf;
    char *remember_token = token;
    strcpy(buf, line);

    int cnt_words = 0;
    token = strtok(token, " \t\r\n");
    while (token != NULL) {
        cnt_words++;
        token = strtok(NULL, " \t\r\n");
    }

    if (cnt_words != 4) {
        printf("Error : Syntax error in nightswatch.\n");
        free(remember_token);
        return;
    }

    // another pass
    strcpy(buf, line);
    token = buf;
    token = strtok(token, " \t\r\n");
    token = strtok(NULL, " \t\r\n");
    if (strcmp(token, "-n") != 0) {
        printf("Error : Syntax error in nightswatch.\n");
        free(remember_token);
        return;
    }
    token = strtok(NULL, " \t\r\n");
    time_t n = atoi(token);
    if (n == 0) {
        printf("Error : Syntax error in nightswatch.\n, The second argument must be a positive integer.");
        free(remember_token);
        return;
    }

    token = strtok(NULL, " \t\n\r");

    int interruptflag = 0;
    if (strcmp(token, "interrupt") == 0) {
        interruptflag = 1;
    }

    newterm(NULL, stderr, stdin);
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int row = 0;

    if (interruptflag) {
        move(row++, 0);
        refresh();
        print_interrupt_CPU();
    }

    time_t tiktok = time(0) - n;

    while (1) {
        if (kbhit()) {
            int ch;
            if ((ch = getch()) != 'q') {
            } else {
                break;
            }
        } else {
            if (time(0) - tiktok >= n){
                tiktok = time(0);
                move(row++, 0);
                refresh();
                if (interruptflag) {
                    interrupt();
                } else {
                    newborn();
                }
            }
        }
    }

    endwin();

    free(remember_token);
}

int print_interrupt_CPU() {
    FILE *f = fopen("/proc/interrupts", "r");
    if (f == NULL) {
        printw("Error : Unable to open the interrupt file.");
        return -1;
    } else {
        char *line = (char *) malloc(sizeof (char) * 2048);
        char *remember_line = line;
        size_t line_sz = 2048;
        getline(&line, &line_sz, f);

        char out[500];
        memset(out, '\0', sizeof(char) * 500);
        char temp[100];

        line = strtok(line, " \t\r\n");
        while (line != NULL) {
            sprintf(temp, "%10s", line);
            strcat(out, temp);
            line = strtok(NULL, " \t\r\n");
        }
        printw("%s\n", out);

        free(remember_line);
    }
    fclose(f);
    return 0;
}

// prints the keyboard interrupts which is the i9042 line in /proc/interrupts
// which is the 3rd line always
void interrupt() {
    FILE *f = fopen("/proc/interrupts", "r");
    if (f == NULL) {
        printf("Error : Unable to open the interrupt file.");
    } else {
        char *line = (char *) malloc(sizeof (char) * 2048);
        char *remember_line = line;
        size_t line_sz = 2048;

        while (getline(&line, &line_sz, f) != -1) {
            char *copy = (char *) malloc(sizeof(char) * strlen(line));
            strcpy(copy, line);
            char *token = copy;

            int flag = 0;
            token = strtok(token, " \r\t\n");
            while (token != NULL) {
                if (strcmp(token, "i8042") == 0) {
                    flag = 1;
                }
                token = strtok(NULL, " \r\t\n");
            }

            free(copy);

            if(flag == 1) {
                break;
            }
        }

        int int_list[32], int_cnt = 0;
        line = strtok(line, " \t\r\n");
        line = strtok(NULL, " \t\r\n");
        while (line != NULL) {
            int_list[int_cnt++] = atoi(line);
            line = strtok(NULL, " \t\r\n");
            if (line[0] == 'I') {
                break;
            }
        }

        char out[500];
        char temp[100];
        memset(out, '\0', sizeof(char) * 500);

        for (int i = 0; i < int_cnt; i++) {
            sprintf(temp, "%10d", int_list[i]);
            strcat(out, temp);
        }
        printw("%s\n", out);

        free(remember_line);
    }
    fclose(f);
}

void newborn() {
    struct dirent *e;
    DIR *d = opendir("/proc/");

    if (d == NULL) {
        printf("Error : Could not open the /proc dirctory.\n");
    } else {
        long long max_start_time = 0;
        pid_t max_pid = 0;
        while ((e = readdir(d))!= NULL) {
            if (atoi(e->d_name) != 0) {
                char statfile[1024];
                memset(statfile, '\0', 1024);
                sprintf(statfile, "/proc/%s/stat", e->d_name);

                FILE *f = fopen(statfile, "r");
                if (f == NULL) {
                    printf("Error : Could not open the stat file for the process %d\n", atoi(e->d_name));
                } else {
                    char *buf = (char *) malloc(sizeof(buf) * 1024);
                    size_t buf_size = 1024;
                    getline(&buf, &buf_size, f);
                    char *token = buf;
                    char *remember_token = token;

                    token = strtok(token, " \t\n\r");
                    for (int i = 0; i < 21; i++) {
                        token = strtok(NULL, " \t\n\r");
                    }

                    long long start_time = atoll(token);
                    if (start_time > max_start_time) {
                        max_pid = atoi(e->d_name);
                    }

                    free(remember_token);
                }

                fclose(f);
            }
        }

        printw("%d\n", max_pid);
    }
}
