#include <float.h>

#include "types.h"
#include "vec.h"
#include "render.h"
#include "random.h"
#include "ppm.h"

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

hitinfo_t ray_triangle_intersection(ray_t ray, triangle_t triangle) {
    // Möller-Trumbore intersection algorithm
    hitinfo_t hitinfo = { false, INFINITY };

    vec3 edge1 = vsub3(triangle.v2, triangle.v1);
    vec3 edge2 = vsub3(triangle.v3, triangle.v1);
    vec3 crossed2 = cross3(ray.dir, edge2);
    f32 det = dot3(edge1, crossed2);

    if (det > -FLT_EPSILON && det < FLT_EPSILON) return hitinfo;
    f32 idet = 1 / det;
    vec3 s = vsub3(ray.pos, triangle.v1);
    f32 u = idet * dot3(s, crossed2);

    if ((u < 0 && fabsf(u) > FLT_EPSILON) || (u > 1 && fabsf(u-1) > FLT_EPSILON)) return hitinfo;
    vec3 crossed1 = cross3(s, edge1);
    f32 v = idet * dot3(ray.dir, crossed1);

    if ((v < 0 && fabsf(v) > FLT_EPSILON) || (u + v > 1 && fabsf(u + v - 1) > FLT_EPSILON)) return hitinfo;
    f32 t = idet * dot3(edge2, crossed1);
    if (t > FLT_EPSILON) {
        hitinfo.did_hit = true;
        hitinfo.dst = t;
        hitinfo.material = triangle.material;
        hitinfo.point = vadd3(ray.pos, fmul3(t, ray.dir));
        vec3 normal = normalize3(cross3(edge1, edge2));
        hitinfo.normal = dot3(normal, ray.dir) > 0 ? fmul3(-1, normal) : normal;
    }
    return hitinfo;
}

hitinfo_t get_closest_hit(ray_t ray, object_t *objects, usize N) {
    // loop over all objects and return the closest one
    hitinfo_t closest_hit = { false, INFINITY };
    hitinfo_t hitinfo;
    for (usize j = 0; j < N; j++) {
        object_t object = objects[j];
        if (object.is_triangle)
            hitinfo = ray_triangle_intersection(ray, object.triangle);
        else
            hitinfo = ray_sphere_intersection(ray, object.sphere);

        // depth checking
        if (hitinfo.did_hit && min(hitinfo.dst, closest_hit.dst) != closest_hit.dst)
            closest_hit = hitinfo;
    }
    return closest_hit;
}

static vec3 lerp(vec3 a, vec3 b, f32 t) {
    return vadd3(fmul3(1 - t, a), fmul3(t, b));
}

vec3 get_environment_light(ray_t ray, sun_t sun) {
    const vec3 horizon_colour = WHITE;
    const vec3 sky_colour = { 0.08f, 0.37f, 0.73f };

    f32 sun_light = powf(max(0, dot3(ray.dir, sun.dir)), sun.focus) * sun.intensity;

    vec3 net_light = lerp(horizon_colour, sky_colour, powf(fabsf(ray.dir.y), 0.38));
    net_light = vadd3(net_light, fmul3(sun_light, sun.colour));
    return net_light;
}

vec3 trace(ray_t original_ray, object_t *objects, usize N, sun_t sun, usize num_bounces) {
    vec3 ray_colour = { 1, 1, 1 };
    vec3 incoming_light = { 0, 0, 0 };
    ray_t ray = original_ray;

    for (usize i = 0; i < num_bounces + 1; i++) {
        hitinfo_t hit = get_closest_hit(ray, objects, N);
        if (hit.did_hit) {
            material_t material = hit.material;
            // bounce
            ray.pos = hit.point;
            // reflection
            vec3 diffuse_dir = normalize3(vadd3(hit.normal, rand_sphere()));
            vec3 specular_dir = vadd3(ray.dir, fmul3(2, hit.normal));
            ray.dir = lerp(diffuse_dir, specular_dir, material.smoothness);

            // incoming light calculation
            vec3 emitted_light = fmul3(material.emission_strength, material.emission_colour);
            incoming_light = vadd3(incoming_light, vmul3(emitted_light, ray_colour));
            ray_colour = vmul3(ray_colour, material.colour);
        } else {
            incoming_light = vadd3(incoming_light, vmul3(get_environment_light(ray, sun), ray_colour));
            break;
        }
    }

    return incoming_light;
}
