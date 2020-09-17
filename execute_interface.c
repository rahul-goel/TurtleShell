#include "header.h"
#include "execute_interface.h"
#include "execute.h"

int execute(char *line) {
    if (line[0] == '\n') {
        return 1;
    }

    char *buf = (char *) malloc(strlen(line));
    strcpy(buf, line);
    char *token = buf;
    char *remember_token = token;

    char **list = malloc(sizeof(char *) * 10);
    int list_size = 0;
    token = strtok(token, ";");
    while (token != NULL) {
        list[list_size++] = token;
        token = strtok(NULL, ";");
    }

    for (int i = 0; i < list_size; i++) {
        if ((strstr(list[i], "<") != NULL || strstr(list[i], ">") != NULL)) {
            if (!redirect(list[i])) {
                free(remember_token);
                return 0;
            }
        } else if (!execute_one(list[i])) {
            free(remember_token);
            return 0;
        }
    }

    free(remember_token);
    return 1;
}

// function uses strtok so beware of calls of strtok across functions
void trim_sidespaces(char *command) {
    char *buf = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);
    strcpy(command, "\0");

    char *token = buf;
    token = strtok(token, " \t\r\n");
    while (token != NULL) {
        strcat(command, token);
        strcat(command, " ");
        token = strtok(NULL, " \t\r\n");
    }
    if (command[0] != '\0') {
        command[strlen(command) - 1] = '\0';
    } else {
        strcat(command, "\0");
    }

    free(buf);
}

/* takes in the command and the pointer to "parts" which is an array of strings.
   the size fo the array has to be 3 which contains the command, input file, output file
   if the input file or the outptut file does not exist then size of string is 0
   takes in append_flag too which must initially be 0

   command > output < input
   command >> output < input
   command < input > output
   command < input >> output
   command < input
   command > output
   command >> ouput
*/
void redirect_parse(char *command, char **parts, int *append_flag) {
    char *buf = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(buf, command);
    char *token;

    if (strstr(buf, "<") != NULL) {
        token = strtok(buf, "<>");
        trim_sidespaces(token);
        strcpy(parts[0], token);        // actual command

        // input is there.
        strcpy(buf, command);
        token = strtok(buf, "<");
        token = strtok(NULL, ">\n");
        trim_sidespaces(token);
        strcpy(parts[1], token);        // input

        strcpy(buf, command);
        if (strstr(buf, ">>") != NULL) {
            // appending output is there
            token = strtok(buf, ">");
            token = strtok(NULL, "><\n");
            trim_sidespaces(token);
            strcpy(parts[2], token);    // append output
            *append_flag = 1;

        } else if (strstr(buf, ">") != NULL) {
            // overwriting output is there
            token = strtok(buf, ">");
            token = strtok(NULL, "<\n");
            trim_sidespaces(token);
            strcpy(parts[2], token);    // overwrite output
        }

    } else if (strstr(buf, ">>") != NULL) {
        // appending output is there
        token = strtok(buf, ">");
        strcpy(parts[0], token);        // actual command
        token = strtok(NULL, ">\n");
        trim_sidespaces(token);
        strcpy(parts[2], token);        // append output
        *append_flag = 1;
    } else if (strstr(buf, ">") != NULL) {
        // overwriting output is there
        token = strtok(buf, ">");
        strcpy(parts[0], token);        // actual command
        token = strtok(NULL, "\n");
        trim_sidespaces(token);
        strcpy(parts[2], token);        // overwrite output
    }

    free(buf);
}

int redirect(char *command) {
    int remember_stdin = dup(STDIN_FILENO);
    int remember_stdout = dup(STDOUT_FILENO);

    char **parts = (char **)malloc(sizeof(char *) * 3);
    for (int i = 0, len = strlen(command); i < 3; i++) {
        parts[i] = (char *)malloc(sizeof(char) * len);
        strcpy(parts[i], "\0");
    }

    int append_flag = 0;
    redirect_parse(command, parts, &append_flag);

    if (strlen(parts[1])) {
        // input file is there
        int input_fd = open(parts[1], O_RDONLY);
        if (input_fd < 0) {
            fprintf(stderr, "Error : Input file does not exist / Permission Denied.\n");
            for (int i = 0; i < 3; i++) {
                free(parts[i]);
            }
            free(parts);
            return 1;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    if (strlen(parts[2])) {
        // output file is there
        int perm_flag = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //0644
        int open_flag = (append_flag ? O_APPEND : O_TRUNC) | O_WRONLY | O_CREAT;
        int output_fd = open(parts[2], open_flag, perm_flag);

        if (output_fd < 0) {
            fprintf(stderr, "Error : Could not open / create ouput file.\n");
            for (int i = 0; i < 3; i++) {
                free(parts[i]);
            }
            free(parts);
            return 1;
        }

        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    int ret = execute_one(parts[0]);
    dup2(remember_stdin, STDIN_FILENO);
    dup2(remember_stdout, STDOUT_FILENO);
    close(remember_stdin);
    close(remember_stdout);

    for (int i = 0; i < 3; i++) {
        free(parts[i]);
    }
    free(parts);
    return ret;
}
