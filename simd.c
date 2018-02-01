//gcc -mssse3 -o simd simd.c
/*
 * Copiright (C) 2018 Santiago León O.
 */

#include <stdio.h>
#include <stdint.h>
#include "emmintrin.h"
#include "tmmintrin.h" 

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

void simd_print_8 (char *str, __m128i n)
{
    uint8_t arr[16];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_32 (char *str, __m128i n)
{
    uint32_t arr[4];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_64 (char *str, __m128i n)
{
    uint64_t arr[2];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

int main (void)
{
    __m128i a = _mm_set1_epi32 (1);
    __m128i zero = _mm_set1_epi32 (0);
    a = _mm_hadd_epi32 (zero, a);
    a = _mm_hadd_epi32 (zero, a);
    simd_print_32 ("%d ", a);
}
