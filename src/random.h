#pragma once
#include "types.h"
#include "vec.h"

u32 pcg(void);
void pcg_init(u64 seed);

f32 frand(void);
vec3 rand_triangle(vec3 v1, vec3 v2, vec3 v3);
vec3 rand_sphere(void);
vec3 rand_sphere_cosine(vec3 normal);
vec3 rand_sphere_cosine2(vec3 normal, f32 w);
vec3 rand_sphere_diffuse(vec3 normal);
