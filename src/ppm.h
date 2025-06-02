#pragma once
#include <stdio.h>
#include "types.h"
#include "vec.h"

#define min(a,b) \
    ({ const __typeof__(a) _a = (a); \
       const __typeof__(b) _b = (b); \
       _a > _b ? _b : _a; })
#define max(a,b) \
    ({ const __typeof__(a) _a = (a); \
       const __typeof__(b) _b = (b); \
       _a < _b ? _b : _a; })
#define hclamp(m,M,x) \
    ({ const __typeof__(x) _m = (m); \
       const __typeof__(x) _M = (M); \
       const __typeof__(x) _x = (x); \
       max(min(_M, _x), _m); })

typedef struct { u8 r, g, b; } rgb_t;
#define cprint(c) \
    ({ const rgb_t _c = (c); \
       printf("rgb(%d, %d, %d)\n", c.r, c.g, c.b); })

rgb_t rgb(vec3 v);
void ppm(FILE *file, rgb_t *pixels, usize width, usize height);

