#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <fcntl.h>

// Environment Variables declared globally throughout the program.
char HOST[1024], USER[1024], PWD[2048], HOME[2048], SHELL[] = "TurtleShell";
char HOST_str[] = "HOST", USER_str[] = "USER", PWD_str[] = "PWD", HOME_str[] = "HOME", SHELL_str[] = "SHELL";

#endif /* HEADER_H */
