#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "vec.h"
#include "random.h"

f32 frand(void) {
    return (f32) rand() / RAND_MAX;
}

// generate a random point on a sphere
vec3 rand_sphere(void) {
    // choose u, v
    f32 u = frand();
    f32 v = frand();

    // get θ, ϕ
    f32 theta = 2 * M_PI * u;
    f32 phi = acosf(2*v - 1);

    // return vec3
    return (vec3) {
        cosf(theta) * sinf(phi),
        sinf(theta) * sinf(phi),
        cosf(phi)
    };
}

// cosine brdf
vec3 rand_sphere_cosine(vec3 normal) {
    return normalize3(vadd3(normal, rand_sphere()));
}

vec3 rand_sphere_cosine2(vec3 normal, f32 w) {
    return normalize3(vadd3(fmul3(w, normal), rand_sphere()));
}

// for generating diffuse reflections
vec3 rand_sphere_diffuse(vec3 normal) {
    vec3 dir = rand_sphere();
    return dot3(normal, dir) > 0 ? dir : fmul3(-1, dir);
}
