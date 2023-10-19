#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char *strcpy_custom(char *dest, const char *src) {
    while ((*dest++ = *src++))
        ;
    return dest - 1;
}

size_t strlen_custom(const char *str) {
    const char *s = str;
    while (*s)
        s++;
    return s - str;
}
int areStringsEqual(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 == '\0' && *str2 == '\0';
}

int main(void) {
    char input[256], *error_message, exec_error[256], *token, *args[256];
    int i, arg_count;
    pid_t child_pid;
    int interactive = isatty(STDIN_FILENO);
    while (1) {
        if (interactive) {
            char prompt[] = "($) ";
            size_t prompt_len = 4;
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
                char newline = '\n';
                if (write(STDOUT_FILENO, &newline, 1) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        }
	if (areStringsEqual(input, "exit")) {
            exit(0);
        }
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

            child_pid = fork();
            if (child_pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (child_pid == 0) {
                execvp(args[0], args);
                error_message = "./hsh: 1: ";
		strcpy_custom(exec_error, error_message);
		strcpy_custom(exec_error, args[0]);
		strcpy_custom(exec_error, ": not found\n");
		write(STDERR_FILENO, exec_error, strlen_custom(exec_error));
		exit(127);
            } else {
                int status;
                waitpid(child_pid, &status, 0);
            }
            token = strtok(NULL, "\n");
        }
    }

    return 0;
}
