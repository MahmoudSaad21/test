#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    char input[256], *token, *args[256];
    int i, arg_count;
    pid_t child_pid;
    while (1) {
        i = 0;

        while (1) {
            char c;
            ssize_t bytes_read = read(STDIN_FILENO, &c, 1);
            if (bytes_read <= 0 || c == '\n') {
                input[i] = '\0';
                break;
            }
            input[i++] = c;
        }

        if (i == 0) {
            break;
        }

        token = strtok(input, " ");
        arg_count = 0;

        while (token != NULL) {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        child_pid = fork();
        if (child_pid == -1) {
            perror("fork");
            _exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            _exit(EXIT_FAILURE);
        } else {
            int status;
            waitpid(child_pid, &status, 0);
        }
    }

    return 0;
}
