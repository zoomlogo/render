#pragma once
#include <stdio.h>
#include "types.h"
#include "vec.h"

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

typedef struct { u8 r, g, b; } rgb_t;
#define cprint(c) \
    ({ const rgb_t _c = (c); \
       printf("rgb(%d, %d, %d)\n", _c.r, _c.g, _c.b); })

// some common colours
#define BLACK ((rgb_t) {0, 0, 0})
#define WHITE ((rgb_t) {255, 255, 255})
#define RED ((rgb_t) {255, 0, 0})
#define GREEN ((rgb_t) {0, 255, 0})
#define BLUE ((rgb_t) {0, 0, 255})
#define CYAN ((rgb_t) {0, 255, 255})
#define YELLOW ((rgb_t) {255, 255, 0})
#define VIOLET ((rgb_t) {255, 0, 255})

rgb_t rgb(vec3 v);
void ppm(FILE *file, rgb_t *pixels, usize width, usize height);

