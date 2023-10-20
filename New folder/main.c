main.c
#include "shell_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int main(void) {
    char input[256], *token, *args[256];
    char *alias_cmd, *alias_value, newline;
    char exec_error_msg[] = "./hsh: 1: ";
    char not_found_msg[] = ": not found\n";
    int i, j, arg_count, last_command_exit_status = 0, status;
    pid_t child_pid;
    int interactive = isatty(STDIN_FILENO);
    struct Alias aliases[256];
    int alias_count = 0;

    while (1) {
        if (interactive) {
            char prompt[] = "$ ";
            write_custom(STDOUT_FILENO, prompt);
        }
        i = 0;

        while (1) {
            char c;
            ssize_t bytes_read = read(STDIN_FILENO, &c, 1);
            if (bytes_read <= 0  c == '\n') {
                input[i] = '\0';
                break;
            }
            input[i++] = c;
        }

        if (i == 0) {
            if (interactive) {
                newline = '\n';
                write_custom(STDOUT_FILENO, &newline);
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
                    newline = '\n';
                    write_custom(STDOUT_FILENO, &newline);
                }
            }
        } else if (areStringsEqual(input, "alias")) {
            char alias_str[256];
            i = 0;

            while (1) {
                char c;
                ssize_t bytes_read = read(STDIN_FILENO, &c, 1);
                if (bytes_read <= 0  c == '\n') {
                    alias_str[i] = '\0';
                    break;
                }
                alias_str[i++] = c;
            }

            alias_cmd = strtok(alias_str, "=");
            alias_value = strtok(NULL, "=");

            if (alias_cmd != NULL && alias_value != NULL) {
                aliases[alias_count].name = strdup(alias_cmd);
                aliases[alias_count].value = strdup(alias_value);
                alias_count++;
            } else {
                char invalid_alias_msg[] = "Invalid alias command\n";
                write_custom(STDERR_FILENO, invalid_alias_msg);
            }
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

                for (j = 0; j < alias_count; j++) {
                    if (areStringsEqual(args[0], aliases[j].name)) {
                        args[0] = aliases[j].value;
                        break;
                    }
                }

                child_pid = fork();
                if (child_pid == -1) {
                    char fork_error_msg[] = "fork\n";
                    write_custom(STDERR_FILENO, fork_error_msg);
                    _exit(EXIT_FAILURE);
                } else if (child_pid == 0) {
                    execvp(args[0], args);
                    write_custom(STDERR_FILENO, exec_error_msg);
                    write_custom(STDERR_FILENO, args[0]);
                    write_custom(STDERR_FILENO, not_found_msg);
                    _exit(127);
                } else {
                    waitpid(child_pid, &status, 0);
                }
                
                token = strtok(NULL, "\n");
                last_command_exit_status = status;
            }
        }
    }

    for (j = 0; j < alias_count; j++) {
        free(aliases[j].name);
        free(aliases[j].value);
    }

    return 0;
}