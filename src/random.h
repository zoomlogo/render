#pragma once
#include "types.h"
#include "vec.h"

f32 frand(void);
vec3 rand_sphere(void);
vec3 rand_sphere_cosine(vec3 normal);
vec3 rand_sphere_cosine2(vec3 normal, f32 w);
vec3 rand_sphere_diffuse(vec3 normal);
