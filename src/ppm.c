#include <stdio.h>
#include "types.h"
#include "vec.h"
#include "ppm.h"

rgb_t rgb(vec3 v) {
    return (rgb_t) {
        (u8) (255 * hclamp(0, 1, v.x)),
        (u8) (255 * hclamp(0, 1, v.y)),
        (u8) (255 * hclamp(0, 1, v.z)),
    };
}

vec3 vec(rgb_t c) {
    return (vec3) {
        (f32) c.r / 255,
        (f32) c.g / 255,
        (f32) c.b / 255
    };
}

void ppm(FILE *file, rgb_t *pixels, usize width, usize height) {
    usize buf_len = width * height;
    fprintf(file, "P3\n%lu %lu\n255\n", (unsigned long) width, (unsigned long) height);
    for (usize i = 0; i < buf_len; i++)
        fprintf(file, "%d %d %d\n", pixels[i].r, pixels[i].g, pixels[i].b);
}
