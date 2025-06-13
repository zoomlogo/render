#pragma once
#include <math.h>
#include "types.h"

// math
#define min(a,b) \
    ({ const typeof(a) _a = (a); \
       const typeof(b) _b = (b); \
       _a > _b ? _b : _a; })
#define max(a,b) \
    ({ const typeof(a) _a = (a); \
       const typeof(b) _b = (b); \
       _a < _b ? _b : _a; })
#define hclamp(m,M,x) \
    ({ const typeof(x) _m = (m); \
       const typeof(x) _M = (M); \
       const typeof(x) _x = (x); \
       max(min(_M, _x), _m); })

// vec3, mat3:
typedef struct { f32 x, y, z; } vec3;
typedef struct { f32 v[9]; } mat3;
#define vprint3(v...) \
    ({ const vec3 _v = (v); \
       printf("vec3(%f, %f, %f)\n", _v.x, _v.y, _v.z); })
#define eql3(a,b) \
    ({ const vec3 _a = (a), _b = (b); \
       (_a.x == _b.x) && (_a.y == _b.y) && (_a.z == _b.z); })
#define dot3(a,b) \
    ({ const vec3 _a = (a), _b = (b); \
       _a.x*_b.x + _a.y*_b.y + _a.z*_b.z; })
#define cross3(a,b) \
    ({ const vec3 _a = (a), _b = (b); \
       (vec3) { _a.y*_b.z - _a.z*_b.y, \
                _a.z*_b.x - _a.x*_b.z, \
                _a.x*_b.y - _a.y*_b.x }; })
#define fmul3(X,a) \
    ({ const f32 _x = (X); const vec3 _a = (a); \
       (vec3) { _x*_a.x, _x*_a.y, _x*_a.z }; })
#define vmul3(a,b) \
    ({ const vec3 _a = (a), _b = (b); \
       (vec3) { _a.x * _b.x, _a.y * _b.y, _a.z * _b.z}; })
#define vadd3(a,b) \
    ({ const vec3 _a = (a), _b = (b); \
       (vec3) { _a.x + _b.x, _a.y + _b.y, _a.z + _b.z}; })
#define vsub3(a,b) vadd3(a, fmul3(-1, b))
#define length3(v) \
    ({ const vec3 _v = (v); \
       sqrtf(dot3(_v, _v)); })
#define normalize3(v) \
    ({ const vec3 _v = (v); \
       f32 _l = sqrtf(dot3(_v, _v)); \
       (vec3) { _v.x/_l, _v.y/_l, _v.z/_l }; })

#define eye3() \
    ((mat3) { {1, 0, 0, 0, 1, 0, 0, 0, 1} })
#define one3() \
    ((mat3) { {1, 1, 1, 1, 1, 1, 1, 1, 1} })
#define zero3() \
    ((mat3) { {0, 0, 0, 0, 0, 0, 0, 0, 0} })

mat3 mmul3(mat3 a, mat3 b);
vec3 mvmul3(mat3 a, vec3 u);

// vec4, mat4
typedef struct { f32 x, y, z, w; } vec4;
typedef struct { f32 v[16]; } mat4;
#define vprint4(v...) \
    ({ const vec4 _v = (v); \
       printf("vec4(%f, %f, %f, %f)\n", _v.x, _v.y, _v.z, _v.w); })
#define eql4(a,b) \
    ({ const vec4 _a = (a), _b = (b); \
       (_a.x == _b.x) && (_a.y == _b.y) && (_a.z == _b.z) && (_a.w == _b.w); })
#define dot4(a,b) \
    ({ const vec4 _a = (a), _b = (b); \
       _a.x*_b.x + _a.y*_b.y + _a.z*_b.z + _a.w*_b.w; })
#define length4(v) \
    ({ const vec4 _v = (v); \
       sqrtf(dot4(_v, _v)); })
#define normalize4(v) \
    ({ const vec4 _v = (v); \
       f32 _l = sqrtf(dot4(_v, _v)); \
       (vec4) { _v.x/_l, _v.y/_l, _v.z/_l, _v.w/_l }; })

#define eye4() \
    ((mat4) { {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} })
#define one4() \
    ((mat4) { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} })
#define zero4() \
    ((mat4) { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} })

mat4 mmul4(mat4 a, mat4 b);
vec4 mvmul4(mat4 a, vec4 u);
