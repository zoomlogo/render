#include <stdio.h>

#include "types.h"
#include "camera.h"
#include "render.h"
#include "vec.h"
#include "ppm.h"

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 5

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    rgb_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    // camera setup
    camera_t camera = setup_camera(
        (vec3) {0,0,0},  // pos
        (vec3) {1,0,0},  // dir
        (vec3) {0,0,1},  // screen_x_dir
        90,  // fov
        2,  // zNear
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    printf("width & height: ");
    vprint3((vec3) {camera.width, camera.height, 0});

    // scene setup

    // populate the buffer
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        // TODO: ray calculation

        buffer[i] = (rgb_t) { 255, 210, 120 };
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    return 0;
}
