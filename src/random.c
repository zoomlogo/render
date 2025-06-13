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
