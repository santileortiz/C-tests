//gcc -o bounded_delta bounded_delta.c

#include <stdio.h>
typedef enum {false, true} bool;
// This function simplifies the task of increasing a value that has some
// boundary before (or after) which the update should stop (or start) happening.
//
// Let ov (old value), nv (new value) and b (boundary) be 3 floating point
// values, assuming (for now) that ov != nv != b. Define the active region as
// the region of the number line less than b. This function works in such a
// way that if the change from ov to nv happens inside the active region, then
// the change between these values will be returned. If instead, the change
// happens outside the active region, 0 is returned. If the change in the value
// crosses the boundary into the active region, the returned value is equal to
// the distance between the bounday and the new value (nv - b). If the change in
// value crosses the boundary out of the active region, the returned value is the
// value necessary to move the old value into the boundary (b - ov).
//
//                                retval (positive)
//                              |------->|
//                  +++ov+++++++b--------nv---------
//                      ----------------->
//                           change
//                                                       - inactive region
//                                                       + active region
//                                retval (negative)
//                              |<-------|
//                  +++nv+++++++b--------ov---------
//                      <-----------------
//                           change
//
// Even though the explanation above assumes all values are different, care has
// been taken to correctly handle all cases where equality happens. This, in
// fact, is what makes the implementation tricky, and what motivated abstracting
// it into a documented function.
//
// The macro bnd_delta_update_inv() is a version of the same function where the
// active region, is the region of the number line grater than the boundary b.
//
#define bnd_delta_update_inv(old,new,bnd) (-bnd_delta_update(-(old),-(new),-(bnd)))
static inline
float bnd_delta_update (float old_val, float new_val, float boundary)
{
    if (old_val == new_val) return 0;

    float adjustment = 0;

    bool was_after = old_val < boundary;
    bool is_after = new_val < boundary;
    if (was_after || is_after) {
        if (was_after && is_after) {
            adjustment = new_val - old_val;
        } else {
            if (old_val < new_val) {
                adjustment = boundary - old_val;
            } else {
                adjustment = new_val - boundary;
            }
        }
    }

    return adjustment;
}

void test (float ov, float nv, float b, float expected)
{
    float res = bnd_delta_update(ov, nv, b);
    printf ("ov: %f, nv: %f, b: %f -> %f", ov, nv, b, expected);
    if (res == expected) {
        printf ("\t(passed)\n");
    } else {
        printf ("\t(failed: %f)\n", res);
    }
}

void test_inv (float ov, float nv, float b, float expected)
{
    float res = bnd_delta_update_inv(ov, nv, b);
    printf ("ov: %f, nv: %f, b: %f -> %f", ov, nv, b, expected);
    if (res == expected) {
        printf ("\t(passed)\n");
    } else {
        printf ("\t(failed: %f)\n", res);
    }
}

void main ()
{
    float b = 5;
    printf ("----NORMAL----\n");
    test (2, 1, b, -1);
    test (1, 2, b,  1);
    test (6, 7, b,  0);
    test (7, 6, b,  0);
    test (3, 8, b,  2);
    test (8, 3, b, -2);

    test (8, b, b,  0);
    test (b, 8, b,  0);
    test (3, b, b,  2);
    test (b, 3, b, -2);

    test (3, 3, b,  0);
    test (8, 8, b,  0);

    test (b, b, b,  0);

    printf ("\n----INVERSE----\n");
    test_inv (2, 1, b,  0);
    test_inv (1, 2, b,  0);
    test_inv (6, 7, b,  1);
    test_inv (7, 6, b, -1);
    test_inv (3, 8, b,  3);
    test_inv (8, 3, b, -3);
            
    test_inv (8, b, b, -3);
    test_inv (b, 8, b,  3);
    test_inv (3, b, b,  0);
    test_inv (b, 3, b,  0);
            
    test_inv (3, 3, b,  0);
    test_inv (8, 8, b,  0);
            
    test_inv (b, b, b,  0);

}
