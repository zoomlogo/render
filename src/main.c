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

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    rgb_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    // set random
    srand((u32) time(NULL));

    // camera setup
    camera_t camera = setup_camera(
        (vec3) {0, 0, 10},  // pos
        (vec3) {0, 0, -1},  // dir
        (vec3) {1, 0, 0},  // screen_x_dir
        90,  // fov
        4,  // zNear
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    printf("width & height: ");
    vprint3((vec3) {camera.width, camera.height, 0});

    // scene setup
    sphere_t spheres[] = {
        (sphere_t) { (vec3) {-6, 4, 0}, 1, (material_t) { RED } },
        (sphere_t) { (vec3) {0, 0, 0}, 2, (material_t) { GREEN } },
        (sphere_t) { (vec3) {3, 5, 0}, 3, (material_t) { BLUE } },
        (sphere_t) { (vec3) {6, 0, -5}, 2, (material_t) { CYAN } },
        (sphere_t) { (vec3) {-5, -3, 1}, 2, (material_t) { YELLOW } },
    };

    // populate the buffer
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        buffer[i] = BLACK;
        // raycast
        vec3 coords = i2v(i);
        vec3 world_coords = screen_to_world_coords(camera, coords);
        vec3 dir = normalize3(vsub3(world_coords, camera.pos));
        ray_t ray = { world_coords, dir };

        // sphere checking
        hitinfo_t closest_hit = { false, INFINITY };
        for (usize j = 0; j < sizeof(spheres) / sizeof(sphere_t); j++) {
            sphere_t sphere = spheres[j];
            hitinfo_t hitinfo = ray_sphere_intersection(ray, sphere);

            // depth checking
            if (hitinfo.did_hit && min(hitinfo.dstA, closest_hit.dstA) != closest_hit.dstA)
                closest_hit = hitinfo;
        }

        // write colour to buffer
        if (closest_hit.did_hit)
            buffer[i] = closest_hit.material.colour;
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    return 0;
}
