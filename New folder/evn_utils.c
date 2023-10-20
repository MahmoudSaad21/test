env_utils.c
#include "shell_functions.h"

int compareEnvVars(const char *a, const char *b) {
    while (*a != '=' && *b != '=') {
        if (*a < *b) return -1;
        if (*a > *b) return 1;
        a++;
        b++;
    }
    if (*a == '=' && *b != '=') return -1;
    if (*a != '=' && *b == '=') return 1;
    return 0;
}

void sortEnvVars(const char **envVars, int envCount) {
    int i, j;
    for (i = 0; i < envCount - 1; i++) {
        for (j = 0; j < envCount - i - 1; j++) {
            if (compareEnvVars(envVars[j], envVars[j + 1]) > 0) {
                const char *temp = envVars[j];
                envVars[j] = envVars[j + 1];
                envVars[j + 1] = temp;
            }
        }
    }
}