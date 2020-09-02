#ifndef LS_H
#define LS_H
    
int is_hidden(char *path);
char *gnu_basename(char *path);
void ls_normal_single(char *directory, char *filename);
void ls_normal_dir(char *directory, int hiddenflag);
void ls_normal_file(char *abspath, int hiddenflag);
void ls_l_single(char *directory, char *filename);
void ls_l_dir(char *directory, int hiddenflag);
void ls_l_file(char *directory, int hiddenflag);
void ls_flag_wrapper(char *abspath, int hiddenflag, int listflag);
void ls(char *command);

#endif
