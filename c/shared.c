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

int skipc(FILE *fp, const char k) {
    /*
     * Skips chars defined by `k` (generally spaces).
     * Puts the current char back into the stream.
     */
    int c;

    if (fp == NULL)
        return -1;

    /* read until c isn't a space */
    c = getc(fp);
    while (c != EOF && c == ' ')
        c = getc(fp);

    ungetc(c, fp);
    return 0;
}
