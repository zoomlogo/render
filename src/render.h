#pragma once
#include "types.h"
#include "vec.h"
#include "ppm.h"

typedef struct ray_t {
    vec3 pos, dir;
} ray_t;

typedef struct material_t {
    rgb_t colour;
} material_t;

typedef struct sphere_t {
    vec3 pos; f32 r;
    material_t material;
} sphere_t;

typedef struct hitinfo_t {
    bool did_hit;
    f32 dstA, dstB;
    vec3 normal; // surface normal at the point
} hitinfo_t;

hitinfo_t ray_sphere_intersection(ray_t ray, sphere_t sphere);
