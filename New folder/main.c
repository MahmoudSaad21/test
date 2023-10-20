#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(void) {
    char input[256], *token, *args[256];
    char newline;
    int i, j, arg_count, last_command_exit_status = 0;
    int interactive = isatty(STDIN_FILENO);
    struct Alias aliases[256];
    int alias_count = 0;

    while (1) {
        if (interactive) {
            write_custom(STDOUT_FILENO, "$ ");
        }
        i = 0;

        while (1) {
            ssize_t bytes_read = read(STDIN_FILENO, &newline, 1);
            if (bytes_read <= 0 || newline == '\n') {
                input[i] = '\0';
                break;
            }
            input[i++] = newline;
        }

        if (i == 0) {
            if (interactive) {
                write_custom(STDOUT_FILENO, "\n");
                continue;
            }
            break;
        }
        if (areStringsEqual(input, "exit")) {
            exit(last_command_exit_status);
        } else if (areStringsEqual(input, "env")) {
            for (j = 0; environ[j] != NULL; j++) {
                write_custom(STDOUT_FILENO, environ[j]);
                if (environ[j + 1] != NULL) {
                    write_custom(STDOUT_FILENO, "\n");
                }
            }
        } else if (areStringsEqual(input, "alias")) {
            handle_alias(aliases, &alias_count);
        } else {
            token = strtok(input, "\n");
            while (token != NULL) {
                char *command = token;
                arg_count = 0;

                token = strtok(command, " ");
                while (token != NULL) {
                    args[arg_count] = token;
                    arg_count++;
                    token = strtok(NULL, " ");
                }
                args[arg_count] = NULL;

                handle_command(aliases, alias_count, args, &last_command_exit_status);

                token = strtok(NULL, "\n");
            }
        }
    }

    for (j = 0; j < alias_count; j++) {
        free(aliases[j].name);
        free(aliases[j].value);
    }

    return 0;
}
