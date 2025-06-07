#include <stdio.h>

#include "types.h"
#include "camera.h"
#include "vec.h"
#include "ppm.h"

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 5

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    rgb_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    // populate the buffer
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        buffer[i] = (rgb_t) { 255, 210, 120 };
    }

    // camera
    camera_t camera = setup_camera(
        (vec3) {0,0,0},
        (vec3) {1,0,0},
        (vec3) {0,0,1},
        90, 2, SCREEN_WIDTH, SCREEN_HEIGHT
    );
    printf("aspect ratio: %f\n", camera.aspect_ratio);
    printf("world screen width: %f\n", camera.width);
    printf("world screen height: %f\n", camera.height);
    printf("testing...\n");
    vec3 v;
    for (usize i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        printf("==================\n");
        printf("i = %lu\n", i);
        v = i2v(i);
        printf("v = ");
        vprint3(v);
        printf("w = ");
        vprint3(screen_to_world_coords(camera, v));
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    return 0;
}
