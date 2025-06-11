#include "../src/types.h"
#include "../src/vec.h"
#include "../src/ppm.h"
#include "test_ppm.h"
INIT_TEST();

static void test_min(void) {
    ASSERT(min(0, 0) == 0);
    ASSERT(min(0, 1) == 0);
    ASSERT(min(0, -1) == -1);
    ASSERT(min(1.5, -1) == -1);
}

static void test_max(void) {
    ASSERT(max(0, 0) == 0);
    ASSERT(max(0, 1) == 1);
    ASSERT(max(0, -1) == 0);
    ASSERT(max(1.5, -1) == 1.5);
}

static void test_hclamp(void) {
    ASSERT(hclamp(0, 1, 0.5) == 0.5);
    ASSERT(hclamp(0, 1, 1.5) == 1);
    ASSERT(hclamp(0, 1, -0.5) == 0);
}

#define rgb_eq(A,B) \
    ({ const rgb_t _a = (A), _b = (B); \
    (_a.r == _b.r) && (_a.g == _b.g) && (_a.b == _b.b); })

static void test_rgb(void) {
    ASSERT(rgb_eq(rgb((vec3) {0, 0, 0}), ((rgb_t) {0, 0, 0})));
    ASSERT(rgb_eq(rgb((vec3) {1, 0, 0}), ((rgb_t) {255, 0, 0})));
    ASSERT(rgb_eq(rgb((vec3) {0, 1, 0}), ((rgb_t) {0, 255, 0})));
    ASSERT(rgb_eq(rgb((vec3) {0, 0, 1}), ((rgb_t) {0, 0, 255})));
    ASSERT(rgb_eq(rgb((vec3) {0.5, 0.5, 0.5}), ((rgb_t) {127, 127, 127})));
}

#undef rgb_eq

void test_ppm(void) {
    BEGIN_TEST();

    test_min();
    test_max();
    test_hclamp();
    test_rgb();

    END_TEST();
}
