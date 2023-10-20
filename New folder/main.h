#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>

struct Alias {
    char *name;
    char *value;
};

char *strcpy_custom(char *dest, const char *src);
size_t strlen_custom(const char *str);
int areStringsEqual(const char *str1, const char *str2);
void write_custom(int fd, const char *str);
int compareEnvVars(const char *a, const char *b);
void sortEnvVars(const char **envVars, int envCount);
void handle_alias(struct Alias aliases[], int *alias_count);
void handle_command(struct Alias aliases[], int alias_count, char* args[], int* last_command_exit_status);  /* Make sure this line is here */

#endif
