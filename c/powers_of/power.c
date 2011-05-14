#include <stdio.h>
#include <stdlib.h>

#include "power.h"

/*
 * Compute a^n for positive integer exponents. Exploit the relationship
 * between a solution to an instance of size n and a solution to an instance
 * of size n-1 (a^n = a^n-1 * a)
 *
 * Recursive definition of the algorithm:
 *
 * f(n) = a^n = { f(n-1) * a    if n > 1,
 *                a             if n = 1 }
 */

static double power(int a, unsigned int n);
static int power2(int a, unsigned int n);

int do_power(int argc, char **argv) {
    int a, n;

    /* requires two integer args, either from command line or stdin */

    if (argc >= 4) {
        /* read from command line */
        a = atoi(argv[2]);
        n = atoi(argv[3]);
    } else {
        /* read from stdin */
        int c;
        int args[2] = {0, 0};

        while ((c = getc(stdin))) {
            if (c == ' ' || c == EOF)
                break;

            args[0] *= 10;
            args[0] += c-'0';
        }

        while ((c = getc(stdin))) {
            while (c != EOF && c == ' ')
                c = getc(stdin);
            if (c == EOF || c == '\n')
                break;

            args[1] *= 10;
            args[1] += c-'0';
        }

        a = args[0];
        n = args[1];
    }

    if (a == 0) {
        printf("0\n");
        return 0;
    }

    if (n <= 0) {
        power_usage(argv[0]);
        return -1;
    }

    printf("%.0lf\n", power(a, n));
    return 0;
}

static double power(int a, unsigned int n) {
    /* recursive */
    if (n == 1)
        return a;
    return a*power(a, n-1);
}

/* bottom-up solution just multiplies a by itself n-1 times */
static int power2(int a, unsigned int n) {
    /* iterative */
    int i, res = a;
    for (i = 1; i < n; ++i) {
        res *= a;
    }
    return res;
}

void power_usage(const char *prog) {
    fprintf(stderr, "Usage: %s power [a n]\n" \
            "power function will read from stdin if no integer args are given.\n\n" \
            "    Example: $ echo 2 3 | %s power\n" \
            "    8\n\n" \
            "Computes a^n where `a` is an integer" \
            " and `n` is a positive integer exponent.\n", prog, prog);
}
