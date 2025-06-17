#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "camera.h"
#include "render.h"
#include "vec.h"
#include "ppm.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SETUP_SCENE_MODE true

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    vec3 *buffer = (vec3 *) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(vec3));

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
    object_t objects[] = {
        (object_t) { true, .triangle = {
            &((vec3) {100, 0, 100}),
            &((vec3) {-100, 0, 100}),
            &((vec3) {100, 0, -100}),
            (material_t) { (vec3) {0.4, 0.5, 0.6 } }}},
        (object_t) { true, .triangle = {
            &((vec3) {-100, 0, 100}),
            &((vec3) {100, 0, -100}),
            &((vec3) {-100, 0, -100}),
            (material_t) { (vec3) {0.4, 0.5, 0.6 } }}},
        (object_t) { false, .sphere = {
            (vec3) {0, 2, 0}, 2, .material = { GREEN, .smoothness = 0.6, .specular_probability = 0.8, .specular_colour = WHITE } }},
        (object_t) { false, {(sphere_t) { (vec3) {-3, 1, 1}, 1, (material_t) { BLUE } }}},
        (object_t) { false, {(sphere_t) { (vec3) {3, 1, 1}, 1, (material_t) { CYAN } }}},
        (object_t) { true, .triangle = {
            &((vec3) {5, 1, 0}),
            &((vec3) {5, 4, -4}),
            &((vec3) {10, 1, 3}),
            (material_t) { CYAN } }},
    };

    sun_t sun = { normalize3(((vec3) {-3, 4, -8})), WHITE, 100, 60 };

    // populate the buffer
    const usize RAYS_PER_PIXEL = 10;
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        vec3 coords = i2v(i);
        vec3 world_coords = screen_to_world_coords(camera, coords);
        vec3 dir = normalize3(vsub3(world_coords, camera.pos));
        ray_t ray = { world_coords, dir };

        if (SETUP_SCENE_MODE) {
            hitinfo_t hit = get_closest_hit(ray, objects, sizeof(objects) / sizeof(object_t));
            buffer[i] = hit.did_hit ? hit.material.colour : (vec3) { 0, 0, 0 };
            continue;
        }

        vec3 colour = { 0, 0, 0 };
        for (usize j = 0; j < RAYS_PER_PIXEL; j++)
            colour = vadd3(colour,
                trace(ray, objects, sizeof(objects) / sizeof(object_t), sun, 4));

        buffer[i] = fmul3(1 / (f32) RAYS_PER_PIXEL, colour);
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);
    free(buffer);

    return 0;
}
