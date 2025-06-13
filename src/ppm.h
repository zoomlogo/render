#pragma once
#include <stdio.h>
#include "types.h"
#include "vec.h"

typedef struct { u8 r, g, b; } rgb_t;
#define cprint(c) \
    ({ const rgb_t _c = (c); \
       printf("rgb(%d, %d, %d)\n", _c.r, _c.g, _c.b); })

// some common colours
#define BLACK ((vec3) {0, 0, 0})
#define WHITE ((vec3) {1, 1, 1})
#define RED ((vec3) {1, 0, 0})
#define GREEN ((vec3) {0, 1, 0})
#define BLUE ((vec3) {0, 0, 1})
#define CYAN ((vec3) {0, 1, 1})
#define YELLOW ((vec3) {1, 1, 0})
#define VIOLET ((vec3) {1, 0, 1})

rgb_t rgb(vec3 v);
void ppm(FILE *file, vec3 *pixels, usize width, usize height);

