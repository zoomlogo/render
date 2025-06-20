#pragma once
#include "types.h"
#include "vec.h"

typedef struct ray_t {
    vec3 pos, dir;
} ray_t;

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
    material_t material;
} sphere_t;

typedef struct triangle_t {
    vec3 *v1, *v2, *v3;
    material_t material;
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
    material_t material;
} hitinfo_t;


// ray-object intersection code
hitinfo_t ray_sphere_intersection(ray_t ray, sphere_t sphere);
hitinfo_t ray_triangle_intersection(ray_t ray, triangle_t triangle);

