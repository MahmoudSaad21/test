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
    int interactive = isatty(STDIN_FILENO);
    while (1) {
        if (interactive) {
            char prompt[] = "$ ";
            size_t prompt_len = sizeof(prompt) - 1;
            if (write(STDOUT_FILENO, prompt, prompt_len) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
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
            if (interactive) {
                if (write(STDOUT_FILENO, "\n", 1) == -1)
                perror("write");
                }
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
