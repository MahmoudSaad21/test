string_utils.c
#include "shell_functions.h"

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