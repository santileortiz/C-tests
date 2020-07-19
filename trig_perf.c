//gcc -g -Og trig_perf.c -o trig_perf -lm
/*
 * Copiright (C) 2018 Santiago León O.
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// Performance test of how long it takes to build a rotation matrix.

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

typedef union {
    float E[16];
    float M[4][4]; // [down][right]
} mat4f;

mat4f rotation_y (float angle_r)
{
    //float s,c;
    //sincosf(angle_r, &s, &c);
    float s = sinf(angle_r);
    float c = cosf(angle_r);
    mat4f res = {{
         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1
    }};
    return res;
}

int main (int argc, char *argv[])
{
    srand (10);
    float avg = 0, res_avg = 0;

    int iterations = 100000, i;
    for (i=0; i<iterations; i++) {
        static float angle = 0;
        uint64_t start = rdtsc();
        //res_avg += sinf (val);
        mat4f model = rotation_y (angle);
        uint64_t end = rdtsc();
        printf ("%lu cycles\n", end-start);

        angle += 0.5*2*M_PI*16.77/1000;
        avg += end - start;
        res_avg += model.E[0];
    }
    printf ("(%f) cycles: %f\n", res_avg/(float)iterations, avg/(float)iterations);
}
