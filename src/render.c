#include "types.h"
#include "vec.h"
#include "render.h"

hitinfo_t ray_sphere_intersection(ray_t ray, sphere_t sphere) {
    // suppose: ray ≡ z̄ = r̄ + t·n̄
    //        & sphere ≡ |z̄ - p̄| = r
    // which we can solve for t by solving:
    // t² + 2·t·(r̄ - p̄)·n̄ + |r̄|² + |p̄|² - 2·r̄·p̄ - r² = 0
    hitinfo_t hitinfo;

    // coefficients of the quadratic
    f32 a = 1;
    f32 b = 2 * dot3(vsub3(ray.pos, sphere.pos), ray.dir);
    f32 c = dot3(ray.pos, ray.pos) + dot3(sphere.pos, sphere.pos)
        - 2 * dot3(ray.pos, sphere.pos) - sphere.r * sphere.r;

    // discriminant
    f32 D = b * b - 4 * a * c;
    if (D >= 0) {
        hitinfo.did_hit = true;
        // FIXME pray to God no one puts ray origin inside/on sphere
        // find distances (basically t, XXX hope n̄ is normalized)
        // XXX these distances can be negative (see FIXME)
        hitinfo.dstA = (-b - sqrtf(D)) / (2 * a);
        hitinfo.dstB = (-b + sqrtf(D)) / (2 * a);

        // TODO add normal calc...
    } else {
        hitinfo.did_hit = false;
        hitinfo.dstA = hitinfo.dstB = INFINITY;
    }

    return hitinfo;
}
