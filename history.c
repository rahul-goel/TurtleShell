#include "header.h"
#include "history.h"

void trim_history(char *path) {
    FILE *f = fopen(path, "r");

    if (!f) {
        perror("Error trim history");
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

void history() {
    char path[1024];
    strcpy(path, HOME);
    strcat(path, "/.history.txt");
    FILE *f = fopen(path, "r");

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
        if (cnt - 10 <= ptr && ptr <= cnt) {
            printf("%s", buffer);
        }
    }

    free(buffer);
    fclose(f);
}

