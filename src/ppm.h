#pragma once
#include <stdio.h>
#include "types.h"
#include "vec.h"

typedef struct { u8 r, g, b; } rgb_t;
#define cprint(c) \
    ({ const rgb_t _c = (c); \
       printf("rgb(%d, %d, %d)\n", _c.r, _c.g, _c.b); })

// some common colours
#define Z 1 / (f32) 51
#define BLACK ((vec3) {Z, Z, Z})
#define WHITE ((vec3) {1, 1, 1})
#define RED ((vec3) {1, Z, Z})
#define GREEN ((vec3) {Z, 1, Z})
#define BLUE ((vec3) {Z, Z, 1})
#define CYAN ((vec3) {Z, 1, 1})
#define YELLOW ((vec3) {1, 1, Z})
#define VIOLET ((vec3) {1, Z, 1})
#undef Z

rgb_t rgb(vec3 v);
void ppm(FILE *file, vec3 *pixels, usize width, usize height);

