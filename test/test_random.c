#include <stdlib.h>
#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/random.h"
INIT_TEST();

static void test_frand(void) {
    ASSERT(fabsf(frand() - 0.840188f) < 0.0001);
    ASSERT(fabsf(frand() - 0.394383f) < 0.0001);
    ASSERT(fabsf(frand() - 0.783099f) < 0.0001);
    ASSERT(fabsf(frand() - 0.798440f) < 0.0001);
}

static void test_rand_sphere(void) {
    ASSERT(length3(rand_sphere()) == 1);
    ASSERT(length3(rand_sphere()) == 1);
    ASSERT(length3(rand_sphere()) == 1);
    ASSERT(length3(rand_sphere()) == 1);
}

void test_random(void) {
    BEGIN_TEST();

    srand(1);
    test_frand();
    test_rand_sphere();

    END_TEST();
}
