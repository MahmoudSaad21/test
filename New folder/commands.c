#include "main.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>  
#include <stdlib.h>  

void handle_command(struct Alias aliases[], int alias_count, char* args[], int* last_command_exit_status) {
    pid_t child_pid;
    int j;

    for (j = 0; j < alias_count; j++) {
        if (areStringsEqual(args[0], aliases[j].name)) {
            args[0] = aliases[j].value;
            break;
        }
    }

    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        _exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    } else {
        waitpid(child_pid, last_command_exit_status, 0);
    }
}
