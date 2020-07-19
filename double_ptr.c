//gcc -o double_ptr double_ptr.c
// A test of operator precedence of array subscripting operator [] and the
// dereference operator *. The functions t1 and t3 are equivalent and cause a
// segfault when trying to dereference a null pointer, t2 works fine.
#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

void t1 (char **arr, int len)
{
    *arr = malloc (sizeof(int)*len);
    char src[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    for (int i=0; i<len; i++) {
        *arr[i] = src[i%ARRAY_SIZE(src)];
    }
}

void t2 (char **arr, int len)
{
    *arr = malloc (sizeof(int)*len);
    char src[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    for (int i=0; i<len; i++) {
        (*arr)[i] = src[i%ARRAY_SIZE(src)];
    }
}

void t3 (char **arr, int len)
{
    *arr = malloc (sizeof(int)*len);
    char src[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    for (int i=0; i<len; i++) {
        *(arr[i]) = src[i%ARRAY_SIZE(src)];
    }
}

int main ()
{
    int len = 10;
    char *arr[len];
    for (int i=0; i<len; i++)
        arr[i] = NULL;

    t2 (&arr[0], len);

    printf ("Pointer array:\n");
    for (int i=0; i<len; i++)
        printf ("%p ", arr[i]);
    printf ("\n\n");

    printf ("First element as array:\n");
    for (int i=0; i<len; i++)
        printf ("%X ", (unsigned char)(*arr)[i]);
    printf ("\n");
}
