#include "header.h"
#include "history.h"

void trim_history(char *path) {
    FILE *f = fopen(path, "r");

    if (!f) {
        perror("Error trim history");
        PROMPTSTATUS = 1;
        return;
    }

    int cnt = 0;
    char *buffer;
    size_t n = 0;
    fseek(f, 0, SEEK_SET);
    while (getline(&buffer, &n, f) != -1) {
        cnt++;
    }
    fclose(f);

    if (cnt >= 20) {
        char store[20][1024];
        int store_ptr = 0;
        int ptr = 0;
        f = fopen(path, "r");
        while (getline(&buffer, &n, f) != -1) {
            ptr++;
            if (cnt - 20 < ptr && ptr <= cnt) {
                strcpy(store[store_ptr++], buffer);
            }
        }
        fclose(f);
        f = fopen(path, "w");
        for (int i = 0; i < 20; i++) {
            fprintf(f, "%s", store[i]);
        }
        fclose(f);
    }
    free(buffer);
}

void add_to_history(char *command) {
    char path[1024];
    strcpy(path, HOME);
    strcat(path, "/.history.txt");
    FILE *f = fopen(path, "a");
    fprintf(f, "%s", command);
    fclose(f);
    trim_history(path);
}

void history(char *line) {
    char path[1024];
    strcpy(path, HOME);
    strcat(path, "/.history.txt");
    FILE *f = fopen(path, "r");

    int arg = 10;
    {
        char *buf = (char *) malloc(sizeof(char) * strlen(line));
        strcpy(buf, line);
        char *token = buf;
        char *remember_token = token;

        token = strtok(token, " \t\n\r");
        token = strtok(NULL, " \t\n\r");
        if (token != NULL) {
            arg = atoi(token);
            token = strtok(NULL, " \t\n\r");

            if (token != NULL) {
                printf("Error : Invalid Syntax for history.");
                free(remember_token);
                PROMPTSTATUS = 1;
                return;
            }
        }

        free(remember_token);
    }

    int cnt = 0;
    char *buffer;
    size_t n = 0;
    while (getline(&buffer, &n, f) != -1) {
        cnt++;
    }

    fseek(f, 0, SEEK_SET);
    int ptr = 0;
    while (getline(&buffer, &n, f) != -1) {
        ptr++;
        if (cnt - arg + 1 <= ptr && ptr <= cnt) {
            printf("%s", buffer);
        }
    }

    free(buffer);
    fclose(f);
}

