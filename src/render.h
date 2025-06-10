#pragma once
#include "types.h"
#include "vec.h"

typedef struct ray_t {
    vec3 pos;
    vec3 dir;
} ray_t;

typedef struct sphere_t {
    vec3 pos; f32 r;
} sphere_t;

typedef struct hitinfo_t {
    bool did_hit;
} hitinfo_t;

hitinfo_t ray_sphere_intersection(ray_t ray, sphere_t sphere);
