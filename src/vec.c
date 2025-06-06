#include <stdlib.h>
#include "vec.h"

// vec3, mat3
static inline vec3 *rows3(mat3 a) {
    vec3 *r = (vec3 *) malloc(3 * sizeof(vec3));
    r[0] = (vec3) { a.v[0], a.v[1], a.v[2] };
    r[1] = (vec3) { a.v[3], a.v[4], a.v[5] };
    r[2] = (vec3) { a.v[6], a.v[7], a.v[8] };
    return r;
}

static inline vec3 *cols3(mat3 a) {
    vec3 *r = (vec3 *) malloc(3 * sizeof(vec3));
    r[0] = (vec3) { a.v[0], a.v[3], a.v[6] };
    r[1] = (vec3) { a.v[1], a.v[4], a.v[7] };
    r[2] = (vec3) { a.v[2], a.v[5], a.v[8] };
    return r;
}

mat3 mmul3(mat3 a, mat3 b) {
    mat3 c;
    vec3 *ar = rows3(a), *bc = cols3(b);
    for (u8 i = 0; i < 9; i++) {
        c.v[i] = dot3(ar[i/3], bc[i%3]);
    }
    free(ar); free(bc); return c;
}

vec3 mvmul3(mat3 a, vec3 u) {
    vec3 v, *ar = rows3(a);
    v.x = dot3(ar[0], u);
    v.y = dot3(ar[1], u);
    v.z = dot3(ar[2], u);
    free(ar); return v;
}

// vec4, mat4
static inline vec4 *rows4(mat4 a) {
    vec4 *r = (vec4 *) malloc(4 * sizeof(vec4));
    r[0] = (vec4) { a.v[0], a.v[1], a.v[2], a.v[3] };
    r[1] = (vec4) { a.v[4], a.v[5], a.v[6], a.v[7] };
    r[2] = (vec4) { a.v[8], a.v[9], a.v[10], a.v[11] };
    r[3] = (vec4) { a.v[12], a.v[13], a.v[14], a.v[15] };
    return r;
}

static inline vec4 *cols4(mat4 a) {
    vec4 *r = (vec4 *) malloc(4 * sizeof(vec4));
    r[0] = (vec4) { a.v[0], a.v[4], a.v[8], a.v[12] };
    r[1] = (vec4) { a.v[1], a.v[5], a.v[9], a.v[13] };
    r[2] = (vec4) { a.v[2], a.v[6], a.v[10], a.v[14] };
    r[3] = (vec4) { a.v[3], a.v[7], a.v[11], a.v[15] };
    return r;
}

mat4 mmul4(mat4 a, mat4 b) {
    mat4 c;
    vec4 *ar = rows4(a), *bc = cols4(b);
    for (u8 i = 0; i < 16; i++) {
        c.v[i] = dot4(ar[i/4], bc[i%4]);
    }
    free(ar); free(bc); return c;
}

vec4 mvmul4(mat4 a, vec4 u) {
    vec4 v, *ar = rows4(a);
    v.x = dot4(ar[0], u);
    v.y = dot4(ar[1], u);
    v.z = dot4(ar[2], u);
    v.w = dot4(ar[3], u);
    free(ar); return v;
}
