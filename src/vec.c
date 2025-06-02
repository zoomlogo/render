#include <stdlib.h>
#include "vec.h"

static inline vec3 *rows(mat3 a) {
    vec3 *r = (vec3 *) malloc(3 * sizeof(vec3));
    r[0] = (vec3) { a.v[0], a.v[1], a.v[2] };
    r[1] = (vec3) { a.v[3], a.v[4], a.v[5] };
    r[2] = (vec3) { a.v[6], a.v[7], a.v[8] };
    return r;
}

static inline vec3 *cols(mat3 a) {
    vec3 *r = (vec3 *) malloc(3 * sizeof(vec3));
    r[0] = (vec3) { a.v[0], a.v[3], a.v[6] };
    r[1] = (vec3) { a.v[1], a.v[4], a.v[7] };
    r[2] = (vec3) { a.v[2], a.v[5], a.v[8] };
    return r;
}

mat3 mmul(mat3 a, mat3 b) {
    mat3 c;
    vec3 *ar = rows(a), *bc = cols(b);
    for (u8 i = 0; i < 9; i++) {
        c.v[i] = dot(ar[i/3], bc[i%3]);
    }
    free(ar); free(bc); return c;
}

vec3 mvmul(mat3 a, vec3 u) {
    vec3 v, *ar = rows(a);
    v.x = dot(ar[0], u);
    v.y = dot(ar[1], u);
    v.z = dot(ar[2], u);
    free(ar); return v;
}
