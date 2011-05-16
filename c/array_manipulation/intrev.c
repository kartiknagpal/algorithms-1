/*
 * intrev.c
 * - reverse the digits in an integer: 123 -> 321
 */

#include <stdio.h>
#include <stdlib.h>

#include "../shared.h"
#include "intrev.h"

static int intreverse(int n);

int do_intrev(int argc, char **argv) {
    int n = 0;

    if (argc > 2) {
        n = atoi(argv[2]);
    } else {
        int c;
        skipc(stdin, ' ');

        while ((c = getc(stdin)) != EOF) {
            if (c == '\n')
                break;

            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c-'0';
            }
        }
    }

    if (n <= 0) {
        intrev_usage(argv[0]);
        return -1;
    }

    printf("%d\n", intreverse(n));
    return 0;
}

static int intreverse(int n) {
    int r = 0;
    while (n) {
        r *= 10;
        r += n % 10;
        n /= 10;
    }
    return r;
}

void intrev_usage(const char *prog) {
    fprintf(stderr, "Usage: %s intrev [<integer>]\n" \
            "Reverses the digits in the integer argument. Optionally accepts stdin.\n\n" \
            "    Example: $ echo 123 | %s intrev\n" \
            "    321\n\n", prog, prog);
}

/*
int main(int argc, char **argv) {
    int n, r;
    n = 123456;

    r = intreverse(n);
    printf("%d\n", r);
    return 0;
}
*/
