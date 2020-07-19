//gcc -o c_const c_const.c
/*
 * Copiright (C) 2018 Santiago León O.
 */

#include <stdio.h>

void foo1 (const char *c) {
    printf ("%c\n", *c);
}

void foo2 (const char **c) {
    printf ("%c\n", **c);
}

int main (void)
{
    char *s1 = "a";
    // char* casts implicitly to const char*
    foo1 (s1);

    // char** does NOT cast implicitly to const char**
    char **ptr = &s1;
    foo2 (ptr);
}
