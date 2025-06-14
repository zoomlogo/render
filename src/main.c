#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "camera.h"
#include "render.h"
#include "random.h"
#include "vec.h"
#include "ppm.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define SETUP_SCENE_MODE false

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    vec3 buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    // set random
    srand((u32) time(NULL));

    // camera setup
    camera_t camera = setup_camera(
        (vec3) {0, 5, 15},  // pos
        (vec3) {0, -0.3, -1},  // dir
        (vec3) {1, 0, 0},  // screen_x_dir
        60,  // fov
        6,  // zNear
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    printf("width & height: ");
    vprint3((vec3) {camera.width, camera.height, 0});

    // scene setup
    sphere_t spheres[] = {
        (sphere_t) { (vec3) {0, -1000, 0}, 1000, (material_t) { RED } },
        (sphere_t) { (vec3) {0, 2, 0}, 2, (material_t) { GREEN } },
        (sphere_t) { (vec3) {-3, 1, 1}, 1, (material_t) { BLUE } },
        (sphere_t) { (vec3) {3, 1, 1}, 1, (material_t) { CYAN } },
        (sphere_t) { (vec3) {-10, 8, -4}, 2, (material_t) { WHITE, 20, WHITE } },
    };

    // populate the buffer
    const usize RAYS_PER_PIXEL = 100;
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        vec3 coords = i2v(i);
        vec3 world_coords = screen_to_world_coords(camera, coords);
        vec3 dir = normalize3(vsub3(world_coords, camera.pos));
        ray_t ray = { world_coords, dir };

        if (SETUP_SCENE_MODE) {
            hitinfo_t hit = get_closest_hit(ray, spheres, sizeof(spheres) / sizeof(sphere_t));
            buffer[i] = hit.did_hit ? hit.material.colour : (vec3) { 0, 0, 0 };
            continue;
        }

        vec3 colour = { 0, 0, 0 };
        for (usize j = 0; j < RAYS_PER_PIXEL; j++)
            colour = vadd3(colour,
                trace(ray, spheres, sizeof(spheres) / sizeof(sphere_t), 5));

        buffer[i] = fmul3(1 / (f32) RAYS_PER_PIXEL, colour);
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    return 0;
}
