#include <stdio.h>

#include "types.h"
#include "vec.h"
#include "ppm.h"

#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 20

i32 main(void) {
    rgb_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        buffer[i] = (rgb_t) { 255, 210, 120 };
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    return 0;
}
