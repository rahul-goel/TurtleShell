#ifndef PINFO_H 
#define PINFO_H

int get_virtual_memory(int pid);
char get_process_state(int pid);
int get_executable_path(int pid, char *path, int path_size);
int is_valid_process(int pid);
void print_out_pinfo(int pid);
void pinfo(char *line);

#endif
