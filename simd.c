//gcc -o simd simd.c -mavx -maes -lm
/*
 * Copiright (C) 2018 Santiago León O.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include "emmintrin.h"
#include "tmmintrin.h" 

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

void simd_print_u8 (char *str, __m128i n)
{
    uint8_t arr[16];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_u16 (char *str, __m128i n)
{
    uint16_t arr[8];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_u32 (char *str, __m128i n)
{
    uint32_t arr[4];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_u64 (char *str, __m128i n)
{
    uint64_t arr[2];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_s8 (char *str, __m128i n)
{
    int8_t arr[16];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_s16 (char *str, __m128i n)
{
    int16_t arr[8];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_s32 (char *str, __m128i n)
{
    int32_t arr[4];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void simd_print_s64 (char *str, __m128i n)
{
    int64_t arr[2];

    _mm_storeu_si128 ((__m128i*)arr, n);

    int i;
    for (i=ARRAY_SIZE(arr)-1; i>=0; i--) {
        printf (str, arr[i]);
    }
    printf ("\n");
}

void test_1()
{
    __m128i a = _mm_set1_epi32 (1);
    __m128i zero = _mm_set1_epi32 (0);
    a = _mm_hadd_epi32 (zero, a);
    a = _mm_hadd_epi32 (zero, a);
    simd_print_s32 ("%d ", a);
}

// When using n bits in two's complement, the value 2^(n-1) is not representable,
// but -2^n is. If we need to substract 2^(n-1) we have to be careful how we do it
// so that at any point we are trying to represent 2^(n-1) with a signed n bit
// representation. The unsigned representation of +2^(n-1) as a two's complement
// signed representation is equal to -2^(n-1), for x - 2^(n-1) to be correct we
// have to make sure there is never an implicit cast of the unsigned 2^(n-1) to
// a signed n bit representation.
//
// Normally when working with literals things "just work" because the compiler
// will puts these literal values directly into bigger registers that make the
// correct calculation. When using variables or SIMD this is not the case and
// it's possible we are accidentally making an implicit cast.
//
// One option (if we can afford it) is to just use bigger variables, everything
// will be representable and we will never have an implicit cast. But, if we
// want to work with the smallest representation possible, the right thing to do
// is to make sure we store store -2^(n-1) in the variable, then perform
// addition. Because -2^(n-1) is representable, things will work correctly. It's
// important to realize that -2^(n-1) is special, because it is representable
// with n bits in two's complement AND for any x in the range 0..(2^n)-1
// (UNsigned n bit numbers) the operation x+(-2^(n-1)) will always result in a
// value representable in two's complement with n bits. No other number
// satisfies both properties at the same time, so in general, when substracting
// any number other than -2^(n-1), we will most likely have to use more bits.
//
// Now the clever part!. This specific operation can be performed faster by
// using an XOR instead of an addition!. In general XOR is NOT equal to
// addition, because it doesn't account for the carries, BUT in the specific
// case of adding -2^(n-1) the only possible carry is the overflow, and this
// overflow is discarded in 2's complement addition. XOR assembly operations are
// faster than addition.
//
// On intel, this optimization makes the throughput 33% faster. But only in
// architectures older than Skylake, in Skylake XOR and addition have the same
// throughput.
void fast_subs_by_2_pow_n()
{
    srand(time(NULL));
    int8_t x = (int8_t)rand();
    printf ("   OK: %d - 128(literal) = %d\n", x, x - 128);
    printf ("   OK: %d + 128(literal) = %d\n", x, x + 128);

    // Implicit cast. Value of subs will actually be -128, then we get the
    // results of + and - operations flipped.
    int8_t subs = 128;
    printf ("WRONG: %d - 128(%d) = %d\n", x, subs, x - subs);
    printf ("WRONG: %d + 128(%d) = %d\n", x, subs, x + subs);

    // -128 is representable so here we don't have an implicit cast and things
    // work as expected.
    int8_t subs_no_cast = -128;
    printf ("   OK: %d + (%d) = %d\n", x, subs_no_cast, x + subs_no_cast);
    printf ("   OK: %d - (%d) = %d\n", x, subs_no_cast, x - subs_no_cast);

    uint8_t y[] = {255, 0, 128, 100, 21, 31, 40, 230};

    __m128i signflip = _mm_set1_epi8 (-128);
    __m128i packed_low = _mm_loadl_epi64 ((__m128i*) y);
    simd_print_s8 ("%d ", signflip);
    simd_print_u8 ("%4d ", packed_low);
    printf ("\n");

    simd_print_s8 ("%4d ", _mm_add_epi8 (packed_low, signflip)); // slow
    simd_print_s8 ("%4d ", _mm_xor_si128 (packed_low, signflip)); // fast
    printf ("\n");

    // I was so confused as to why XOR worked correctly, that I had to convince
    // myself that in general XOR is NOT equivalent to subtraction.
    for (int i = 0; i >= 0 && i <= 128; i++) {
        __m128i values_8 = _mm_loadl_epi64 ((__m128i*) y);
        __m128i signflip = _mm_set1_epi8 (-i);
        __m128i subs_8 = _mm_xor_si128 (values_8, signflip);

        // Longer precision correct implementation
        __m128i values_16 = _mm_set_epi16 (y[7], y[6], y[5], y[4], y[3], y[2], y[1], y[0]);
        __m128i i_16 = _mm_set1_epi16 (i);
        __m128i subs_16 = _mm_subs_epi16 (values_16, i_16);

        int8_t arr_8[16];
        _mm_storeu_si128 ((__m128i*)arr_8, subs_8);

        int16_t arr_16[8];
        _mm_storeu_si128 ((__m128i*)arr_16, subs_16);

        printf ("%3d: ", i);
        for (int j=ARRAY_SIZE(arr_16)-1; j>=0; j--) {
            if (arr_16[j] == arr_8[j]) {
                printf ("       OK ");
            } else {
                char buff[10];
                //sprintf (buff, "FAIL(%d)", arr_16[j] - arr_8[j]);
                sprintf (buff, "FAIL(%d)", y[j]);
                printf ("%9s ", buff);
            }
        }
        printf ("\n");
    }
}

int main (int argc, char **argv)
{
    fast_subs_by_2_pow_n();
}
