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

void ppm(FILE *file, vec3 *pixels, usize width, usize height) {
    usize buf_len = width * height;
    fprintf(file, "P3\n%lu %lu\n255\n", (unsigned long) width, (unsigned long) height);
    for (usize i = 0; i < buf_len; i++) {
        rgb_t col = rgb(pixels[i]);
        fprintf(file, "%d %d %d\n", col.r, col.g, col.b);
    }
}
