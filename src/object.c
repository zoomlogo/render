#include "object.h"
#include "vec.h"


bool ray_aabb_intersection(ray_t ray, aabb_t aabb) {
    // setup ray and box
    vec3 box_size = vabs3(vsub3(aabb.a, aabb.b));
    vec3 box_origin = fmul3(0.5f, vadd3(aabb.a, aabb.b));
    ray_t transformed_ray = { vsub3(ray.pos, box_origin), ray.dir };

    // from inigo quilez
    vec3 m = { 1 / transformed_ray.dir.x, 1 / transformed_ray.dir.y, 1 / transformed_ray.dir.z };
    vec3 n = vmul3(m, transformed_ray.pos);
    vec3 k = vmul3(vabs3(m), box_size);

    vec3 t1 = fmul3(-1, vadd3(n, k));
    vec3 t2 = fmul3(-1, vsub3(n, k));

    f32 tN = max(max(t1.x, t1.y), t1.z);
    f32 tF = min(min(t2.x, t2.y), t2.z);

    if (tN > tF || tF < 0.0) return false; // did not hit the box

    return true;
}

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
        } else if (t1 > 0 && t2 <= 0) {
            // inside/on sphere
            t1 = fabsf(t1);
        } else if (t1 <= 0 && t2 <= 0) {
            // missed sphere
            hitinfo.did_hit = false;
        }

        hitinfo.dst = t1;

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

hitinfo_t ray_triangle_intersection(ray_t *ray, triangle_t *triangle) {
    // Möller-Trumbore intersection algorithm
    hitinfo_t hitinfo = { false, INFINITY };
    const f32 eps = FEPS;

    vec3 edge1 = vsub3(*triangle->v2, *triangle->v1);
    vec3 edge2 = vsub3(*triangle->v3, *triangle->v1);
    vec3 crossed2 = cross3(ray->dir, edge2);
    f32 det = dot3(edge1, crossed2);

    if (det > -eps && det < eps) return hitinfo;
    f32 idet = 1 / det;
    vec3 s = vsub3(ray->pos, *triangle->v1);
    f32 u = idet * dot3(s, crossed2);

    if ((u < 0 && fabsf(u) > eps) || (u > 1 && fabsf(u-1) > eps)) return hitinfo;
    vec3 crossed1 = cross3(s, edge1);
    f32 v = idet * dot3(ray->dir, crossed1);

    if ((v < 0 && fabsf(v) > eps) || (u + v > 1 && fabsf(u + v - 1) > eps)) return hitinfo;
    f32 t = idet * dot3(edge2, crossed1);
    if (t > eps) {
        hitinfo.did_hit = true;
        hitinfo.dst = t;
        hitinfo.material = triangle->material;
        hitinfo.point = vadd3(ray->pos, fmul3(t, ray->dir));
        vec3 normal = normalize3(cross3(edge1, edge2));
        hitinfo.normal = dot3(normal, ray->dir) > 0 ? fmul3(-1, normal) : normal;
    }
    return hitinfo;
}

