#ifndef BG_PROC_LIST_H
#define BG_PROC_LIST_H

struct bg_proc {
    int pid;
    char pname[1024];
};

extern int cnt_bg_proc;
extern struct bg_proc bg_proc_list[100];

void insert_bg_process(struct bg_proc *proc);
int remove_bg_prc(int pid);
void find_processname(int pid, char *pname);
void remove_latest_insertion();

#endif
