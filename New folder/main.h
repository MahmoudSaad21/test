main.h
#ifndef MAIN_H
#define MAIN_H

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

#endif