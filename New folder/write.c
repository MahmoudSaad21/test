#include "main.h"
#include <unistd.h>
#include <stdio.h>  
#include <stdlib.h> 

void write_custom(int fd, const char *str) {
    size_t len = strlen_custom(str);
    if (write(fd, str, len) == -1) {
        perror("write");
        _exit(EXIT_FAILURE);
    }
}
