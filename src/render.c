#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "bvh.h"
#include "object.h"
#include "types.h"
#include "vec.h"
#include "render.h"
#include "random.h"
#include "ppm.h"

// scene handling
scene_t *new_scene(void) {
    scene_t *scene = (scene_t *) malloc(sizeof(scene_t));
    scene->_m_alloc = 4;
    scene->_o_alloc = 4;

    scene->num_models = 0;
    scene->num_objects = 0;
    scene->num_sources = 0;

    scene->models = (model_t *) malloc(sizeof(model_t) * scene->_m_alloc);
    scene->objects = (object_t *) malloc(sizeof(object_t) * scene->_o_alloc);

    return scene;
}

void scene_setup_sun(scene_t *scene, vec3 dir, vec3 colour, f32 focus, f32 intensity) {
    scene->sun = (sun_t) {normalize3(dir), colour, focus, intensity};
}

void scene_add_sphere(scene_t *scene, sphere_t sphere) {
    object_t object = {false, .sphere = sphere};
    scene->objects[scene->num_objects++] = object;
    if (scene->num_objects >= scene->_o_alloc) {
        scene->_o_alloc *= 2;
        scene->objects = realloc(scene->objects, sizeof(object_t) * scene->_o_alloc);
    }

    if (sphere.material->emission_strength > 0) {
        // put the light sources first
        object_t temp = scene->objects[scene->num_sources];
        scene->objects[scene->num_sources++] = scene->objects[scene->num_objects - 1];
        scene->objects[scene->num_objects - 1] = temp;
    }
}

void scene_add_triangle(scene_t *scene, triangle_t triangle) {
    object_t object = {true, .triangle = triangle};
    scene->objects[scene->num_objects++] = object;
    if (scene->num_objects >= scene->_o_alloc) {
        scene->_o_alloc *= 2;
        scene->objects = realloc(scene->objects, sizeof(object_t) * scene->_o_alloc);
    }

    if (triangle.material->emission_strength > 0) {
        // put the light sources first
        object_t temp = scene->objects[scene->num_sources];
        scene->objects[scene->num_sources++] = scene->objects[scene->num_objects - 1];
        scene->objects[scene->num_objects - 1] = temp;
    }
}

void scene_add_model(scene_t *scene, model_t model) {
    scene->models[scene->num_models++] = model;
    if (scene->num_models >= scene->_m_alloc) {
        scene->_m_alloc *= 2;
        scene->models = realloc(scene->models, sizeof(model_t) * scene->_m_alloc);
    }
}

void del_scene(scene_t *scene) {
    free(scene->models);
    free(scene->objects);
    free(scene);
}

// ray tracing functions
static vec3 lerp(vec3 a, vec3 b, f32 t) {
    return vadd3(fmul3(1 - t, a), fmul3(t, b));
}

void get_closest_hit(ray_t *ray, scene_t *scene, hitinfo_t *out) {
    // XXX need to optimize this
    // loop over all objects in scene and return the closest one
    new_hitinfo(out);
    hitinfo_t hitinfo;

    // objects
    for (usize j = 0; j < scene->num_objects; j++) {
        object_t object = scene->objects[j];
        if (object.is_triangle)
            ray_triangle_intersection(ray, &object.triangle, &hitinfo);
        else
            ray_sphere_intersection(ray, &object.sphere, &hitinfo);

        // depth checking
        if (hitinfo.did_hit && min(hitinfo.dst, out->dst) != out->dst)
            *out = hitinfo;
    }

    // models
    for (usize i = 0; i < scene->num_models; i++) {
        model_t model = scene->models[i];

        if (model.bvh) {
            ray_bvh_intersection(ray, model.bvh, out);
        } else {
            if (!ray_aabb_intersection(ray, &model.bounds))
                continue;  // skip if we completely miss the model
            // loop over all triangles in model
            for (usize j = 0; j < model.N_triangles; j++) {
                ray_triangle_intersection(ray, &model.triangles[j], &hitinfo);

                // depth checking
                if (hitinfo.did_hit && min(hitinfo.dst, out->dst) != out->dst)
                    *out = hitinfo;
            }
        }
    }
}

vec3 get_environment_light(ray_t *ray, sun_t sun) {
    const vec3 horizon_colour = WHITE;
    const vec3 sky_colour = { 0.08f, 0.37f, 0.73f };

    f32 sun_light = powf(max(0, dot3(ray->dir, sun.dir)), sun.focus) * sun.intensity;

    vec3 net_light = lerp(horizon_colour, sky_colour, powf(fabsf(ray->dir.y), 0.38));
    net_light = vadd3(net_light, fmul3(sun_light, sun.colour));
    return net_light;
}

static inline vec3 random_source_point(scene_t *scene) {
    usize random_source_index = pcg() % scene->num_sources;
    object_t source = scene->objects[random_source_index];

    vec3 point_on_source;
    if (source.is_triangle) {
        point_on_source = rand_triangle(
            *source.triangle.v1,
            *source.triangle.v2,
            *source.triangle.v3
        );
    } else {
        point_on_source = vadd3(source.sphere.pos, rand_sphere());
    }

    return point_on_source;
}

vec3 trace(ray_t original_ray, scene_t *scene, usize num_bounces) {
    vec3 ray_colour = { 1, 1, 1 };
    vec3 incoming_light = { 0, 0, 0 };
    ray_t ray = original_ray;
    const f32 eps = FEPS;

    hitinfo_t hit;
    for (usize i = 0; i < num_bounces + 1; i++) {
        get_closest_hit(&ray, scene, &hit);
        if (hit.did_hit) {
            material_t material = *hit.material;
            // bounce
            ray.pos = hit.point;

            // skip calculation if hit point is same as ray origin
            // to account for numerical error
            if (hit.dst < eps) continue;

            // reflection
            vec3 diffuse_dir = rand_sphere_cosine(hit.normal);
            vec3 specular_dir = vadd3(ray.dir, fmul3(2, hit.normal));
            bool is_specular_bounce = material.specular_probability >= frand();
            ray.dir = lerp(diffuse_dir, specular_dir, material.smoothness * is_specular_bounce);

            // incoming light calculation
            vec3 emitted_light = fmul3(material.emission_strength, material.emission_colour);
            if (i != 0)
                emitted_light = fmul3(0.5, incoming_light);
            incoming_light = vadd3(incoming_light, vmul3(emitted_light, ray_colour));
            ray_colour = vmul3(ray_colour, lerp(material.colour, material.specular_colour, is_specular_bounce));

            // dls for sun
            if (!eql3(scene->sun.dir, ((vec3) {0, 0, 0}))) {
                hitinfo_t ghost_hit;
                ray_t ghost_ray = { hit.point, rand_sphere_cosine2(scene->sun.dir, 4) };
                get_closest_hit(&ghost_ray, scene, &ghost_hit);
                if (!ghost_hit.did_hit)
                    incoming_light = vadd3(incoming_light, vmul3(get_environment_light(&ray, scene->sun), ray_colour));
            }

            // dls for other objects
            if (scene->num_sources > 0) {
                hitinfo_t ghost_hit;

                vec3 target = random_source_point(scene);
                vec3 pv = vsub3(target, hit.point);
                ray_t ghost_ray = { hit.point, normalize3(pv) };
                get_closest_hit(&ghost_ray, scene, &ghost_hit);
                if (ghost_hit.did_hit && ghost_hit.dst - length3(pv) < eps) {
                    vec3 ghost_emitted_light = fmul3(ghost_hit.material->emission_strength, ghost_hit.material->emission_colour);
                    incoming_light = vadd3(incoming_light, vmul3(ghost_emitted_light, ray_colour));
                }
            }
        } else {
            incoming_light = vadd3(incoming_light, vmul3(get_environment_light(&ray, scene->sun), ray_colour));
            if (i != 0)
                incoming_light = fmul3(0.5, incoming_light);
            break;
        }
    }

    return incoming_light;
}
