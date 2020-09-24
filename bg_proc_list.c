#include "header.h"
#include "bg_proc_list.h"

int cnt_bg_proc;
struct bg_proc bg_proc_list[100];

// finds the process name using the pid
// uses /proc/pid/stat
// the char * returned needs to be freed from the caller function
char *find_proc_name(int pid) {
    char *path = (char *)malloc(sizeof(char) * 512);
    sprintf(path, "/proc/%d/stat", pid);

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error: Invalid Process Id.\n");
        free(path);
        return NULL;
    }

    char *buf;
    size_t buf_size = 0;
    char *ret = (char *) malloc(sizeof(char) * 64);
    getline(&buf, &buf_size, f);

    char *open_bracket = strstr(buf, "(");
    char *close_bracket = strstr(buf, ")");
    *close_bracket = '\0';
    open_bracket++;
    strcpy(ret, open_bracket);

    free(buf);
    free(path);
    return ret;
}

// takes in the struct
void insert_bg_process(struct bg_proc *proc) {
    bg_proc_list[cnt_bg_proc++] = *proc;
}

// takes in the pid
int remove_bg_proc(int pid) {
    int flag = 0;
    for (int i = 0; i < cnt_bg_proc; i++) {
        if (flag) {
            bg_proc_list[i-1] = bg_proc_list[i];
        } else if (bg_proc_list[i].pid == pid) {
            flag = 1;
        }
    }
    cnt_bg_proc--;
    return (flag ? 0 : -1);
}

// finds process name - linear search
void find_processname(int pid, char *pname) {
    for (int i = 0; i < cnt_bg_proc; i++) {
        if (bg_proc_list[i].pid == pid) {
            strcpy(pname, bg_proc_list[i].pname);
            break;
        }
    }
}

void remove_latest_insertion() {
    cnt_bg_proc--;
}
