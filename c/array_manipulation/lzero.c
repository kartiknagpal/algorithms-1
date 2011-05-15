/*
 * Partition an array in such a way that zeroes are moved to the left side of the array
 * and all other numbers to the right side of the array. In-place only.
 */

#include <stdio.h>
#include <stdlib.h>

#include "lzero.h"

int do_lzero(int argc, char **argv) {
    int i, n, cur, tmp;
    int *list;


    if (argc > 2) {
        /* get number of args remaining */
        n = argc - 2;
        list = malloc(sizeof(int)*n);
        for (i = 0; i < n; ++i) {
            list[i] = atoi(argv[i+2]);
        }
    } else {
        int c, size = 32;
        list = malloc(sizeof(int)*size);
        n = 0;

        /* remove leading spaces */
        while ((c = getc(stdin)))
            if (c != ' ')
                break;

        if (c == EOF || c == '\n')
            return -1;

        ungetc(c, stdin);

        while ((c = getc(stdin))) {
            if (c == ' ') {
                ++n;
                if (n >= size) {
                    int *newlist = realloc(list, size*2);
                    if (!newlist) {
                        free(list);
                        return -1;
                    }

                    size *= 2;
                }
            }

            /* move to next number */
            while (c != EOF && c == ' ')
                c = getc(stdin);
            if (c == EOF || c == '\n')
                break;

            list[n] *= 10;
            list[n] += c-'0';
        }
        ++n;
    }

    cur = 0;
    for (i = 0; i < n; ++i) {
        if (list[i] == 0) {
            tmp = list[cur];
            list[cur] = list[i];
            list[i] = tmp;
            ++cur;
        }
    }

    for (i = 0; i < n; ++i) {
        printf("%d", list[i]);
        if (i < n-1)
            printf(", ");
    }

    free(list);

    printf("\n");
    return 0;
}

void lzero_usage(const char *prog) {
    fprintf(stderr, "Usage: %s lzero [<list of integers>]\n" \
            "Partitions an array such that zeroes are shifted to the left of the array.\n\n" \
            "    Example: $ echo 2 3 0 4 0 7 8 0 | %s lzero\n", prog, prog);
}
