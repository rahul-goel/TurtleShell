#ifndef JOBS_H
#define JOBS_H

char find_process_status(pid_t pid);
int check_job_validity(char *command);
void jobs(char *command);

#endif
