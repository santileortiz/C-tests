//gcc -Wall -O3 -o no_warn no_warn.c
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

enum format_t {
    FMT = 1,
    FMT_1 = 2
};

int main ()
{
    uint32_t val = rand ()%10;
    int num_graphs = 5;

    int res = 0;

    uint64_t *count;
    if (val & FMT) {
        count = malloc (sizeof(uint64_t)*num_graphs);
    }

    int i;
    for (i=0; i<num_graphs; i++) {
        res += 1;
        if (val & FMT) {
            count[i] = 1;
        }
    }
}
