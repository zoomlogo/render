#include <math.h>
#include <float.h>

#include "types.h"
#include "vec.h"
#include "render.h"
#include "random.h"
#include "ppm.h"

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
            bool is_specular_bounce = material.specular_probability >= frand();
            ray.dir = lerp(diffuse_dir, specular_dir, material.smoothness * is_specular_bounce);

            // incoming light calculation
            vec3 emitted_light = fmul3(material.emission_strength, material.emission_colour);
            incoming_light = vadd3(incoming_light, vmul3(emitted_light, ray_colour));
            ray_colour = vmul3(ray_colour, lerp(material.colour, material.specular_colour, is_specular_bounce));
        } else {
            incoming_light = vadd3(incoming_light, vmul3(get_environment_light(ray, sun), ray_colour));
            break;
        }
    }

    // TODO speed this code up by implementiny
    // direct light sampling
    return incoming_light;
}
