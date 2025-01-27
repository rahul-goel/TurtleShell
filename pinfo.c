#include "header.h"
#include "pinfo.h"

// returns the virtual memory consumed by a procss if it is consumed
// else returns -1
int get_virtual_memory(pid_t pid) {
    char file_path[64];
    sprintf(file_path, "/proc/%d/status", pid);
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        perror("Error");
        return -1;
    }
    char *buf = (char *) malloc(sizeof(char) * 512);
    size_t n = 512;
    int ret = 0;
    while (getline(&buf, &n, f) >= 0) {
        char *token = (char *) malloc(sizeof(char) * 512);
        char *remember_token = token;
        strcpy(token, buf);
        token = strtok(token, " \t\r\n");
        if (token != NULL && strcmp(token, "VmSize:") == 0) {
            token = strtok(NULL, " \t\r\n");
            ret = atoi(token);
        }
        free(remember_token);
    }

    free(buf);
    fclose(f);
    return ret;
}

char get_process_state(pid_t pid) {
    char file_path[64];
    sprintf(file_path, "/proc/%d/status", pid);
    FILE *f = fopen(file_path, "r");
    char ret = '\0';
    char *buf = (char *) malloc(sizeof(char) * 512);
    size_t n = 512;
    while (getline(&buf, &n, f) >= 0) {
        char *token = (char *) malloc(sizeof(char) * 512);
        char *remember_token = token;
        strcpy(token, buf);
        token = strtok(token, " \r\n\t");
        if (token != NULL && strcmp(token, "State:") == 0) {
            token = strtok(NULL, " \r\n\t");
            ret = token[0];
        }
        free(remember_token);
    }

    free(buf);
    fclose(f);
    return ret;

}

// returns -1 for unsucessful execution, stores the path in passed string
int get_executable_path(pid_t pid, char *path, int path_size) {
    char file_path[64];
    sprintf(file_path, "/proc/%d/exe", pid);
    int success_flag = readlink(file_path, path, path_size);
    return success_flag;
}

int is_valid_process(pid_t pid) {
    char path[64];
    sprintf(path, "/proc/%d/status", pid);
    FILE *f = fopen(path, "r");
    int flag = (f != NULL);
    if (f) {
        fclose(f);
    }
    return flag;
}

void print_out_pinfo(pid_t pid) {
    printf("Process Id: %d\n", pid);

    char p_state = get_process_state(pid);
    if (!p_state) {
        perror("Error");
    } else {
        printf("Process State: %c\n", p_state);
    }

    int p_virtual_memory = get_virtual_memory(pid);
    if (p_virtual_memory < 0) {
        perror("Error");
    } else {
        printf("Process Memory: %d\n", p_virtual_memory);
    }

    char *exe_path = (char *) malloc(sizeof(char) * 512);
    int exe_path_size = 512;
    memset(exe_path, '\0', exe_path_size);
    int exe_path_ret = get_executable_path(pid, exe_path, exe_path_size);
    if (exe_path_ret == -1) {
        printf("Could not find path for executable.\n");
    } else {
        // TODO - handle the case when home is the part of exe_path
        // handling if the executable is in the home directory
        char *occ = strstr(exe_path, HOME);
        printf("Executable Path : ");
        printf(BBLU);
        if (occ) {
            printf("~%s\n", occ + strlen(HOME));
        } else {
            printf("%s\n", exe_path);
        }
        printf(WHT);
    }
    free(exe_path);
}

// takes in the command mentioned as input. it must also contain pinfo as the first wrokd
void pinfo(char *line) {
    char *buf = (char *) malloc((strlen(line)) * sizeof(char));
    strcpy(buf, line);
    char *token = buf;
    char *remember_token = token;

    token = strtok(token, " \r\t\n");
    token = strtok(NULL, " \r\t\n");
    pid_t pid;

    // no argument provided
    if (token == NULL) {
        pid = getpid();
        print_out_pinfo(pid);
    } else {
        /*
         * TODO - handle strtok bug of multiple args
        {
            while (token != NULL) {
                printf("Address of token : %p\n", &token);
                printf("Token is %s\n", token);
                pid = atoi(token);
                if (!is_valid_process(pid)) {
                    printf("Error: No process with id %d exists.\n", pid);
                } else {
                    print_out_pinfo(pid);
                }
                token = strtok(NULL, " \r\t\n");
            }
        }
        */
        pid = atoi(token);
        if (!is_valid_process(pid)) {
            printf("Error: No process with id %d exists.\n", pid);
        } else {
            print_out_pinfo(pid);
        }
    }

    free(remember_token);
}
/*
int main() {
    char *line = (char *) malloc(256 * sizeof(char));
    size_t n = 256;
    getline(&line, &n, stdin);
    pinfo(line);
    free(line);
    return 0;
}
*/
