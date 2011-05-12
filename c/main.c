#include <stdio.h>
#include <string.h>

#include "powers_of/power.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <algorithm> [<args>]\n" \
            "Runs the given algorithm with its required args.\n", prog);
}

int main(int argc, char **argv) {
    int i;
    char *algo;

    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }

    algo = argv[1];
    for (i = 0; i < strlen(algo); ++i) {
        algo[i] = tolower(algo[i]);
    }

    if (strcmp(algo, "power") == 0)
        do_power(argc, argv);

    return 0;
}
