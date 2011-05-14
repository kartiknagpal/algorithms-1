#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

int resize_char_buffer(char **buf, int size) {
    char *newbuf = realloc(*buf, size);
    if (!newbuf) {
        fprintf(stderr, "Can't realloc buffer\n");
        return -1;
    }

    *buf = newbuf;
    return 0;
}
