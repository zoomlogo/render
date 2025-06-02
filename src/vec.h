#pragma once
#include <math.h>
#include "types.h"

typedef struct { f32 x, y, z; } vec3;
#define vprint(v) \
    ({ const vec3 _v = (v); \
       printf("%f %f %f\n", _v.x, _v.y, _v.z); })
#define dot(a,b) \
    ({ const vec3 _a = (a), _b = (b); \
       _a.x*_b.x + _a.y*_b.y + _a.z*_b.z; })
#define length(v) \
    ({ const vec3 _v = (v); \
       sqrtf(dot(_v, _v)); })
#define normalize(v) \
    ({ const vec3 _v = (v); \
       f32 _l = sqrtf(dot(_v, _v)); \
       (vec3) { _v.x/_l, _v.y/_l, _v.z/_l }; })

typedef struct { f64 v[9]; } mat3;
#define eye3() \
    ((mat3) { {1, 0, 0, 0, 1, 0, 0, 0, 1} })
#define one3() \
    ((mat3) { {1, 1, 1, 1, 1, 1, 1, 1, 1} })
#define zero3() \
    ((mat3) { {0, 0, 0, 0, 0, 0, 0, 0, 0} })

mat3 mmul(mat3 a, mat3 b);
vec3 mvmul(mat3 a, vec3 u);

