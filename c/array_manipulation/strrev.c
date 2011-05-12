/* strrev.c
 * - reverse a string: "abc" -> "cba"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strrev.h"

static void reverse(char *s, int start, int end);
static void wordrev(char *s);

#define MAX_BUF 4

static int resize_buffer(char **buf, int size) {
    char *newbuf = realloc(*buf, size);
    if (!newbuf) {
        fprintf(stderr, "Can't realloc buffer\n");
        return -1;
    }

    *buf = newbuf;
    return 0;
}

int do_strrev(int argc, char **argv) {
    int i, n, bytes, size;
    char *buf;

    size = MAX_BUF;
    buf = malloc(size);

    /* 2 args for progname and algorithm to run (./algorithms strrev <string>),
     * rest of args are used as the string to reverse. */
    if (argc >= 3) {
        /* read from command line */
        for (i = 2, bytes = 0; argv[i]; ++i) {
            n = snprintf(buf+bytes, size-bytes, "%s ", argv[i]);
            if (n >= size-bytes) {
                if (resize_buffer(&buf, size*2) == -1) {
                    free(buf);
                    return -1;
                }

                size *= 2;
                /* retry the copy */
                n = snprintf(buf+bytes, size-bytes, "%s ", argv[i]);
            }
            bytes += n;
        }

        /* strip off trailing ' ' */
        buf[bytes-1] = '\0';
    } else {
        /* read from stdin */
        int c, i;
        i = 0;
        while ((c = getc(stdin))) {
            if (c == EOF || c == '\n')
                break;
            if (i >= size) {
                if (resize_buffer(&buf, size*2) == -1) {
                    free(buf);
                    return -1;
                }

                size *= 2;
            }
            buf[i++] = c;
        }
        buf[i] = '\0';
    }

    reverse(buf, 0, strlen(buf));
    printf("%s\n", buf);
    free(buf);
    return 0;
}

static void reverse(char *s, int start, int end) {
    /* reverses from start to end-1 */
    int i, j;
    char tmp;
    for (i = start, j = end-1; i < j; ++i, --j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

static void wordrev(char *s) {
    int i, j, tmp;
    for (i = 0; i < strlen(s); ++i) {
        j = i;
        while (s[i] != '\0' && s[i] != ' ')
            ++i;
        reverse(s, j, i);
    }
}

/*
int main(int argc, char **argv) {
    int i;
    char s[] = "hello world foo bar baz";
    printf("normal: %s\n", s);

    reverse(s, 0, strlen(s));
    printf("reversed string: %s\n", s);

    wordrev(s);
    printf("reversed words: %s\n", s);

    return 0;
}
*/
