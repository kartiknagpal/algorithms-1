#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "powers_of/power.h"

#define HASH_SIZE 32

struct algo_s {
    char *name;
    int (*cmd)(int, char **);
};

static int (*algo_hash_table[HASH_SIZE])(int, char **);
static int (*algo_lookup(const char *name))(int, char **);
static long hashString(const char *name);
static void init_hash_table(void);

static struct algo_s algorithms[] = {
    {"power", do_power}
};

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <algorithm> [<args>]\n" \
            "Runs the given algorithm with its required args.\n", prog);
}

int main(int argc, char **argv) {
    int i;
    char *algo;
    int (*func)(int, char **);

    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }

    init_hash_table();

    algo = argv[1];
    for (i = 0; i < strlen(algo); ++i) {
        algo[i] = tolower(algo[i]);
    }

    func = algo_lookup(algo);
    if (func)
        func(argc, argv);

    return 0;
}

static long hashString(const char *name) {
    int i;
    char c;
    long hash;

    hash = 0;
    for (i = 0; i < strlen(name); ++i) {
        c = tolower(name[i]);
        hash = (hash << 4) ^ (hash >> 28) ^ c;
    }

    /* clamp the hash to HASH_SIZE-1 */
    hash &= HASH_SIZE-1;
    return hash;
}

static int (*algo_lookup(const char *name))(int, char **) {
    return algo_hash_table[hashString(name)];
}

static void init_hash_table(void) {
    int i, len;
    len = sizeof(algorithms) / sizeof(struct algo_s);

    for (i = 0; i < len; ++i) {
        if (algo_hash_table[hashString(algorithms[i].name)])
            fprintf(stderr, "hash table collision on key %s (%ld)\n",
                    algorithms[i].name, hashString(algorithms[i].name));
        else
            algo_hash_table[hashString(algorithms[i].name)] = algorithms[i].cmd;
    }
}