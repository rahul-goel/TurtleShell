#include "header.h"
#include "ctrl_c.h"
#include "prompt.h"

void ctrl_c(int sig) {
    printf("\n");
    prompt();
}
