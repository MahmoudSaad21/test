#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void handle_alias(struct Alias aliases[], int* alias_count) {
    char alias_str[256];
    int i = 0;
    ssize_t bytes_read;
    char c;
    char* alias_cmd;  // Move declaration here
    char* alias_value;  // Move declaration here

    while (1) {
        bytes_read = read(STDIN_FILENO, &c, 1);
        if (bytes_read <= 0 || c == '\n') {
            alias_str[i] = '\0';
            break;
        }
        alias_str[i++] = c;
    }

    alias_cmd = strtok(alias_str, "=");  // Now it's just an assignment
    alias_value = strtok(NULL, "=");  // Now it's just an assignment

    if (alias_cmd != NULL && alias_value != NULL) {
        aliases[*alias_count].name = strdup(alias_cmd);
        aliases[*alias_count].value = strdup(alias_value);
        (*alias_count)++;
    } else {
        write_custom(STDERR_FILENO, "Invalid alias command\n");
    }
}
