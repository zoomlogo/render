#include "../src/types.h"
#include "../src/vec.h"
#include "test_vec.h"
INIT_TEST();

static void test_eql3(void) {
    ASSERTQ(((vec3) {1, 2, 3}), ((vec3) {1, 2, 3}));
    ASSERT(!eql3(((vec3) {3, 1, 2}), ((vec3) {1, 2, 3})));
}

static void test_dot3(void) {
    ASSERT(dot3(((vec3) {0, 1, 0}), ((vec3) {0, 0, 1})) == 0);
    ASSERT(dot3(((vec3) {1, 1, 0}), ((vec3) {0, 0, 1})) == 0);
    ASSERT(dot3(((vec3) {0, 1, 0}), ((vec3) {1, 0, 1})) == 0);
    ASSERT(dot3(((vec3) {1, 1, 0}), ((vec3) {1, 0, 1})) == 1);
    ASSERT(dot3(((vec3) {0, 1, 1}), ((vec3) {0, 0, 1})) == 1);
    ASSERT(dot3(((vec3) {0, 0, 0}), ((vec3) {0, 0, 0})) == 0);
    ASSERT(dot3(((vec3) {-1, 1, 1}), ((vec3) {1, 0, 1})) == 0);
}

static void test_cross3(void) {
    ASSERTQ(cross3(
        ((vec3) {0, 0, 1}),
        ((vec3) {0, 0, 1})),
        ((vec3) {0, 0, 0}));
    ASSERTQ(cross3(
        ((vec3) {1, 0, 0}),
        ((vec3) {0, 1, 0})),
        ((vec3) {0, 0, 1}));
    ASSERTQ(cross3(
        ((vec3) {0, 0, 1}),
        ((vec3) {1, 0, 0})),
        ((vec3) {0, 1, 0}));
    ASSERTQ(cross3(
        ((vec3) {0, 1, 0}),
        ((vec3) {0, 0, 1})),
        ((vec3) {1, 0, 0}));
    ASSERTQ(cross3(
        ((vec3) {1, 2, 3}),
        ((vec3) {2, 3, 1})),
        ((vec3) {-7, 5, -1}));
    ASSERTQ(cross3(
        ((vec3) {2, 3, 1}),
        ((vec3) {1, 2, 3})),
        ((vec3) {7, -5, 1}));
}

static void test_fmul3(void) {
    ASSERTQ(fmul3(0, ((vec3) {1, 2, 3})), ((vec3) {0, 0, 0}));
    ASSERTQ(fmul3(1, ((vec3) {1, 2, 3})), ((vec3) {1, 2, 3}));
    ASSERTQ(fmul3(-1, ((vec3) {1, 2, 3})), ((vec3) {-1, -2, -3}));
    ASSERTQ(fmul3(2, ((vec3) {1, 2, 3})), ((vec3) {2, 4, 6}));
}

static void test_vmul3(void) {
    ASSERTQ(vmul3(
        ((vec3) {2, 3, 1}),
        ((vec3) {1, 2, 3})),
        ((vec3) {2, 6, 3}));
    ASSERTQ(vmul3(
        ((vec3) {0, 3, 1}),
        ((vec3) {1, 6, 0})),
        ((vec3) {0, 18, 0}));
}

static void test_vadd3(void) {
    ASSERTQ(vadd3(
        ((vec3) {2, 3, 1}),
        ((vec3) {1, 2, 3})),
        ((vec3) {3, 5, 4}));
}

static void test_vsub3(void) {
    ASSERTQ(vsub3(
        ((vec3) {2, 3, 1}),
        ((vec3) {1, 3, 3})),
        ((vec3) {1, 0, -2}));
}

static void test_length3(void) {
    ASSERT(length3(((vec3) {0, 0, 0})) == 0);
    ASSERT(length3(((vec3) {1, 0, 0})) == 1);
    ASSERT(length3(((vec3) {3, 4, 0})) == 5);
    ASSERT(length3(((vec3) {2, 6, 3})) == 7);
}

static void test_normalize3(void) {
    // case of zero vec3 is not handled
    ASSERTQ(normalize3(((vec3) {1, 0, 0})), ((vec3) {1, 0, 0}));
    ASSERTQ(normalize3(((vec3) {2, 1, 0})), ((vec3) {2 / sqrtf(5), 1 / sqrtf(5), 0}));
}

static void test_min3(void) {
	ASSERTQ(min3(((vec3) {0, 0, 0}), ((vec3) {0, -1, 2})), ((vec3) {0, -1, 0}));
}

static void test_max3(void) {
	ASSERTQ(max3(((vec3) {0, 0, 0}), ((vec3) {0, -1, 2})), ((vec3) {0, 0, 2}));
}

static void test_mmul3(void) {
    mat3 a = {{
        0, 1, 2,
        3, 2, 1,
        1, 0, 2
    }};
    mat3 b = {{
        2, 1, 2,
        0, 2, 1,
        0, 0, 2
    }};
    mat3 c = mmul3(a, b);
    mat3 d = mmul3(b, a);
    bool assertion1 = true;
    bool assertion2 = true;

    f32 expected_c[] = {0, 2, 5, 6, 7, 10, 2, 1, 6};
    f32 expected_d[] = {5, 4, 9, 7, 4, 4, 2, 0, 4};
    for (u8 i = 0; i < 9; i++) {
        assertion1 &= c.v[i] == expected_c[i];
        assertion2 &= d.v[i] == expected_d[i];
    }
    ASSERT(assertion1);
    ASSERT(assertion2);
}

static void test_mvmul3(void) {
    mat3 a = {{
        0, 1, 2,
        3, 2, 1,
        1, 0, 2
    }};
    vec3 b = { 4, 7, -1 };
    ASSERTQ(mvmul3(a, b), ((vec3) { 5, 25, 2 }));
}

static void test_eql4(void) {
    ASSERT(eql4(((vec4) {1, 2, 3, 4}), ((vec4) {1, 2, 3, 4})));
    ASSERT(!eql4(((vec4) {4, 3, 1, 2}), ((vec4) {1, 2, 4, 3})));
}

static void test_dot4(void) {
    ASSERT(dot4(((vec4) {0, 1, 0, 1}), ((vec4) {0, 0, 1, 1})) == 1);
    ASSERT(dot4(((vec4) {1, 1, 0, 0}), ((vec4) {0, 0, 1, 1})) == 0);
    ASSERT(dot4(((vec4) {0, 1, 2, 0}), ((vec4) {1, 0, 1, 0})) == 2);
    ASSERT(dot4(((vec4) {1, 1, 0, 0}), ((vec4) {1, 0, 1, 0})) == 1);
    ASSERT(dot4(((vec4) {0, 1, 1, -1}), ((vec4) {0, 0, 1, 2})) == -1);
    ASSERT(dot4(((vec4) {0, 0, 0, 0}), ((vec4) {0, 0, 0, 0})) == 0);
    ASSERT(dot4(((vec4) {-1, 1, 1, 1}), ((vec4) {1, 0, 1, 3})) == 3);
}

static void test_length4(void) {
    ASSERT(length4(((vec4) {0, 0, 0, 0})) == 0);
    ASSERT(length4(((vec4) {1, 0, 1, 0})) == sqrtf(2));
    ASSERT(length4(((vec4) {3, 4, 0, 0})) == 5);
    ASSERT(length4(((vec4) {2, 6, 3, 1})) == sqrtf(50));
}

static void test_normalize4(void) {
    // case of zero vec3 is not handled
    ASSERT(eql4(normalize4(((vec4) {0, 0, 0, 1})), ((vec4) {0, 0, 0, 1})));
    ASSERT(eql4(normalize4(((vec4) {2, 0, 0, 1})), ((vec4) {2 / sqrtf(5), 0, 0, 1 / sqrtf(5)})));
}

static void test_mmul4(void) {
    mat4 a = {{
        1, 0, 2, 6,
        0, 3, 4, 5,
        9, 0, -1, 7,
        0, 1, 1, -1
    }};
    mat4 b = {{
        -1, 2, 3, 0,
        0, 2, 3, 7,
        0, 0, 4, -2,
        0, 0, 0, 6
    }};
    mat4 c = mmul4(a, b);
    mat4 d = mmul4(b, a);
    bool assertion1 = true;
    bool assertion2 = true;

    f32 expected_c[] = {-1, 2, 11, 32, 0, 6, 25, 43, -9, 18, 23, 44, 0, 2, 7, -1};
    f32 expected_d[] = {26, 6, 3, 25, 27, 13, 12, 24, 36, -2, -6, 30, 0, 6, 6, -6};
    for (u8 i = 0; i < 16; i++) {
        assertion1 &= c.v[i] == expected_c[i];
        assertion2 &= d.v[i] == expected_d[i];
    }
    ASSERT(assertion1);
    ASSERT(assertion2);
}

static void test_mvmul4(void) {
    mat4 a = {{
        1, 0, 2, 6,
        0, 3, 4, 5,
        9, 0, -1, 7,
        0, 1, 1, -1
    }};
    vec4 b = { -1, 3, 2, 1 };
    ASSERT(eql4(mvmul4(a, b), ((vec4) { 9, 22, -4, 4 })));
}

void test_vec(void) {
    BEGIN_TEST();

    // dim 3
    test_eql3();
    test_dot3();
    test_cross3();
    test_fmul3();
    test_vmul3();
    test_vadd3();
    test_vsub3();
    test_length3();
    test_normalize3();
	test_min3();
	test_max3();
    test_mmul3();
    test_mvmul3();

    // dim 4
    test_eql4();
    test_dot4();
    test_length4();
    test_normalize4();
    test_mmul4();
    test_mvmul4();

    END_TEST();
}
