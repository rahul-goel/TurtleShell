#ifndef EXECUTE_INTERFACE_H
#define EXECUTE_INTERFACE_H
    
int execute(char *line);
int redirect(char *command);
void trim_sidespaces(char *command);
void redirect_parse(char *command, char **parts, int *append_flag);

#endif
