#include <stdlib.h>
#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/random.h"
INIT_TEST();

static void test_frand(void) {
    ASSERT(fabsf(frand() - 0.157556f) < 0.0001);
    ASSERT(fabsf(frand() - 0.097849f) < 0.0001);
    ASSERT(fabsf(frand() - 0.795962f) < 0.0001);
    ASSERT(fabsf(frand() - 0.837166f) < 0.0001);
}

static void test_rand_sphere(void) {
    ASSERT(fabs(length3(rand_sphere()) - 1) < 0.0001);
    ASSERT(fabs(length3(rand_sphere()) - 1) < 0.0001);
    ASSERT(fabs(length3(rand_sphere()) - 1) < 0.0001);
    ASSERT(fabs(length3(rand_sphere()) - 1) < 0.0001);
}

void test_random(void) {
    BEGIN_TEST();

    srand(1);
    test_frand();
    test_rand_sphere();

    END_TEST();
}
