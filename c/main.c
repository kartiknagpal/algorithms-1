#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "powers_of/power.h"
#include "array_manipulation/strrev.h"

#define HASH_SIZE 32

struct algo_s {
    char *name;
    int (*cmd)(int, char **);
    void (*help_cmd)(const char *);
};

static int (*algo_hash_table[HASH_SIZE])(int, char **);
static void (*algo_help_table[HASH_SIZE])(const char *);
static int (*algo_lookup(const char *name))(int, char **);
static void (*algo_help_lookup(const char *name))(const char *);
static long hashString(const char *name);
static void init_hash_table(void);

static void do_help(int argc, char **argv) {
    void (*func)(const char *);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s help [<algorithm>]\n" \
                "Where <algorithm> is one of:\n\n" \
                "    'power'\n" \
                "    'strrev'\n" \
                "    'wordrev'\n", argv[0]);
        return;
    }

    func = algo_help_lookup(argv[2]);
    if (func)
        func(argv[0]);
}

static struct algo_s algorithms[] = {
    {"help", do_help, NULL},
    {"power", do_power, power_usage},
    {"strrev", do_strrev, strrev_usage},
    {"wordrev", do_wordrev, wordrev_usage}
};

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <algorithm> [<args>]\n" \
            "    To see available algorithms: $ %s help\n\n" \
            "Runs the given algorithm with its required args.\n", prog, prog);
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
        hash += (long)(c) * (i+119);
    }

    /* clamp the hash to HASH_SIZE-1 */
    hash &= HASH_SIZE-1;
    return hash;
}

static int (*algo_lookup(const char *name))(int, char **) {
    return algo_hash_table[hashString(name)];
}

static void (*algo_help_lookup(const char *name))(const char *) {
    return algo_help_table[hashString(name)];
}

static void init_hash_table(void) {
    int i, len;
    len = sizeof(algorithms) / sizeof(struct algo_s);

    for (i = 0; i < len; ++i) {
        if (algo_hash_table[hashString(algorithms[i].name)])
            fprintf(stderr, "hash table collision on key %s (%ld)\n",
                    algorithms[i].name, hashString(algorithms[i].name));
        else {
            algo_hash_table[hashString(algorithms[i].name)] = algorithms[i].cmd;
            algo_help_table[hashString(algorithms[i].name)] = algorithms[i].help_cmd;
        }
    }
}
