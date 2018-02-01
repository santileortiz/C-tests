//gcc -Wall -O3 -o warn warn.c
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct result {
    int res;
};

struct graph {
    int vertices[5];
};

void computational_search (struct graph *g, struct result *out) {
    out->res = 0;
    int i;
    for (i=0; i<5; i++) {
        out->res += g->vertices[i];
    }
}

void next_graph (struct graph *g)
{
    int i;
    for (i=0; i<5; i++) {
        g->vertices[i] += rand();
    }
}

enum format_t {
    FMT = 1,
    FMT_1 = 2
};

int main()
{
    int val = rand()%10;
    int num_graphs = 5;

    struct graph g;
    struct result res;

    uint64_t *count;
    if (val & FMT) {
        count = malloc(sizeof(uint64_t)*num_graphs);
    }

    int i;
    for (i=0; i<num_graphs; i++) {
        next_graph (&g);
        computational_search (&g, &res);
        if (val & FMT) {
            count[i] = res.res; /* ERROR HERE */
        }
    }

    return 0;
}
