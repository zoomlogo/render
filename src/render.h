#pragma once
#include "types.h"
#include "vec.h"
#include "ppm.h"

typedef struct ray_t {
    vec3 pos, dir;
} ray_t;

typedef struct material_t {
    vec3 colour;
    f32 emission_strength;
    vec3 emission_colour;
    f32 smoothness;
} material_t;

typedef struct sphere_t {
    vec3 pos; f32 r;
    material_t material;
} sphere_t;

typedef struct sun_t {
    vec3 dir, colour;
    f32 focus, intensity;
} sun_t;

typedef struct hitinfo_t {
    bool did_hit;
    f32 dstA, dstB;
    vec3 normal; // surface normal at the point
    vec3 point; // point of ray hitting sphere
    material_t material;
} hitinfo_t;

hitinfo_t ray_sphere_intersection(ray_t ray, sphere_t sphere);
hitinfo_t get_closest_hit(ray_t ray, sphere_t *spheres, usize N);
vec3 get_environment_light(ray_t ray, sun_t sun);
vec3 trace(ray_t original_ray, sphere_t *spheres, usize N, sun_t sun, usize num_bounces);
