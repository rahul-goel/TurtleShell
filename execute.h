#ifndef EXECUTE_H
#define EXECUTE_H

void check_bg_process(int sig);
void execvp_parse(char *input, char *command, char **arg, int *arg_cnt);
void run_excvp_bg(char *buf);
void run_excvp(char *buf);
int chek_last_ampersand(char *line);
void remove_last_ampersand(char *line);
int execute(char *line);

#endif
