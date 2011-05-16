#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "powers_of/power.h"
#include "array_manipulation/strrev.h"
#include "array_manipulation/intrev.h"
#include "array_manipulation/lzero.h"
#include "shared.h"

#define HASH_SIZE 32

struct algo_s {
    char *name;
    int (*cmd)(int, char **);
    void (*help_cmd)(const char *);
};

static int do_help(int argc, char **argv);
static void usage(const char *prog);
static int (*algo_hash_table[HASH_SIZE])(int, char **);
static void (*algo_help_table[HASH_SIZE])(const char *);
static int (*algo_lookup(const char *name))(int, char **);
static void (*algo_help_lookup(const char *name))(const char *);
static long hashString(const char *name);
static void init_hash_table(void);

static struct algo_s algorithms[] = {
    {"help", do_help, usage},
    {"power", do_power, power_usage},
    {"intrev", do_intrev, intrev_usage},
    {"strrev", do_strrev, strrev_usage},
    {"wordrev", do_wordrev, wordrev_usage},
    {"lzero", do_lzero, lzero_usage}
};

static char *algo_names(void) {
    int i, n, bytes, size;
    char *buf;
    struct algo_s a;

    size = MAX_BUF;
    buf = malloc(size);

    bytes = 0;
    for (i = 0; i < sizeof(algorithms) / sizeof(struct algo_s); ++i) {
        a = algorithms[i];

        n = snprintf(buf+bytes, size-bytes, "    '%s'\n", a.name);
        if (n >= size-bytes) {
            if (resize_char_buffer(&buf, size*2) == -1) {
                free(buf);
                return NULL;
            }

            size *= 2;
            n = snprintf(buf+bytes, size-bytes, "    '%s'\n", a.name);
        }
        bytes += n;
    }
    buf[bytes-1] = '\0';
    return buf;
}

static int do_help(int argc, char **argv) {
    void (*func)(const char *);
    char *buf;

    if (argc < 3) {
        buf = algo_names();
        fprintf(stderr, "Usage: %s help [<algorithm>]\n" \
                "Where <algorithm> is one of:\n\n%s\n", argv[0], buf);
        free(buf);
        return -1;
    }

    func = algo_help_lookup(argv[2]);
    if (func)
        func(argv[0]);
    return 0;
}

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
    else
        usage(argv[0]);

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
    long hash;

    len = sizeof(algorithms) / sizeof(struct algo_s);

    for (i = 0; i < len; ++i) {
        hash = hashString(algorithms[i].name);
        if (algo_hash_table[hash]) {
            fprintf(stderr, "hash table collision on key %s (%ld)\n",
                    algorithms[i].name, hash);
        } else {
            algo_hash_table[hash] = algorithms[i].cmd;
            algo_help_table[hash] = algorithms[i].help_cmd;
        }
    }
}
