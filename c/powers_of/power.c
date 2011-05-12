#include <stdio.h>

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
static void power_usage(const char *prog);
static void parse_args(char **argv, int *a, int *n);

int do_power(int argc, char **argv) {
    int a, n;
    /* requires two integer args */
    if (argc < 4) {
        power_usage(argv[0]);
        return -1;
    }

    a = atoi(argv[2]);
    n = atoi(argv[3]);

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

static void power_usage(const char *prog) {
    fprintf(stderr, "Usage: %s a n\n" \
            "Computes a^n where `a` is an integer" \
            " and `n` is a positive integer exponent.\n", prog);
}
