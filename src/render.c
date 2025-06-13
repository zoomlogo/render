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
        // find distances (basically t, XXX hope n̄ is normalized)
        f32 t1 = (-b - sqrtf(D)) / (2 * a);
        f32 t2 = (-b + sqrtf(D)) / (2 * a);

        if (t1 <= 0 && t2 > 0) {
            // inside/on sphere
            t1 = fabsf(t2);
            t2 = INFINITY;
        } else if (t1 > 0 && t2 <= 0) {
            // inside/on sphere
            t1 = fabsf(t1);
            t2 = INFINITY;
        } else if (t1 <= 0 && t2 <= 0) {
            // missed sphere
            hitinfo.did_hit = false;
        }

        hitinfo.dstA = t1;
        hitinfo.dstB = t2;

        // calculate surface normal
        if (t1 != INFINITY) {
            vec3 intersection_point = vadd3(ray.pos, fmul3(t1, ray.dir));
            hitinfo.point = intersection_point;
            hitinfo.normal = normalize3(vsub3(intersection_point, sphere.pos));
        }

        hitinfo.material = sphere.material;
    } else {
        hitinfo.did_hit = false;
    }

    return hitinfo;
}
