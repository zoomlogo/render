#pragma once
#include "types.h"
#include "vec.h"

typedef struct ray_t {
    vec3 pos, dir;
} ray_t;

typedef struct aabb_t {
    vec3 a, b;  // a: min, b: max
} aabb_t;

typedef struct material_t {
    vec3 colour;
    f32 emission_strength;
    vec3 emission_colour;
    f32 smoothness;
    f32 specular_probability;
    vec3 specular_colour;
} material_t;

typedef struct sphere_t {
    vec3 pos; f32 r;
    material_t *material;
} sphere_t;

typedef struct triangle_t {
    vec3 *v1, *v2, *v3;
    material_t *material;
} triangle_t;

typedef struct object_t {
    bool is_triangle;
    union {
        sphere_t sphere;
        triangle_t triangle;
    };
} object_t;

typedef struct hitinfo_t {
    bool did_hit;
    f32 dst;
    vec3 normal; // surface normal at the point
    vec3 point; // point of intersection
    material_t *material;
} hitinfo_t;

// constructor for hitinfo_t
void new_hitinfo(hitinfo_t *out);

// aabb_t stuff:
void new_aabb(aabb_t *out);
void grow_to_include_point(aabb_t *box, vec3 *point);
void grow_to_include_triange(aabb_t *box, triangle_t *triangle);

// ray-object intersection code
bool ray_aabb_intersection(ray_t *ray, aabb_t *aabb);
void ray_sphere_intersection(ray_t *ray, sphere_t *sphere, hitinfo_t *out);
void ray_triangle_intersection(ray_t *ray, triangle_t *triangle, hitinfo_t *out);

