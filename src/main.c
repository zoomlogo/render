#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "camera.h"
#include "render.h"
#include "model.h"
#include "vec.h"
#include "ppm.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SETUP_SCENE_MODE false

// TODO BVH, DLS and more optimizations

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    vec3 *buffer = (vec3 *) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(vec3));

    // set random
    srand((u32) time(NULL));

    // scene setup
    scene_t *scene = new_scene();
    // camera setup
    scene_setup_sun(scene, (vec3) {-3, 4, -8}, WHITE, 100, 60);
    scene->camera = setup_camera(
        (vec3) {0, 5, 15},  // pos
        (vec3) {0, -0.3, -1},  // dir
        (vec3) {1, 0, 0},  // screen_x_dir
        60,  // fov
        6,  // zNear
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    printf("width & height: ");
    vprint3((vec3) {scene->camera.width, scene->camera.height, 0});

    scene_add_triangle(scene, (triangle_t) {
        &((vec3) {100, 0, 100}),
        &((vec3) {-100, 0, 100}),
        &((vec3) {100, 0, -100}),
        (material_t) { (vec3) {0.4, 0.5, 0.6 } }
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-100, 0, 100}),
            &((vec3) {100, 0, -100}),
            &((vec3) {-100, 0, -100}),
            (material_t) { (vec3) {0.4, 0.5, 0.6 } }
    });
    scene_add_sphere(scene, (sphere_t) {
        (vec3) {-3, 1, 1},
        1,
        (material_t) { BLUE }
    });
    scene_add_sphere(scene, (sphere_t) {
        (vec3) {3, 1, 1},
        1,
        (material_t) { CYAN }
    });
    scene_add_sphere(scene, (sphere_t) {
        (vec3) {0, 2, 0},
        2,
        (material_t) { GREEN }
    });

    // load knight
    FILE *knight_file = fopen("modal/Knight.obj", "r");
    model_t *knight = load_model(knight_file, (material_t) { RED });
    fclose(knight_file);

    scale_model(knight, (vec3) {1.5, 1.5, 1.5});
    rotate_model(knight, (vec3) {0, 1, 0}, 270);

    // scene_add_model(scene, *knight);


    // populate the buffer
    const usize RAYS_PER_PIXEL = 1;
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        vec3 coords = i2v(i);
        vec3 world_coords = screen_to_world_coords(scene->camera, coords);
        vec3 dir = normalize3(vsub3(world_coords, scene->camera.pos));
        ray_t ray = { world_coords, dir };

        if (SETUP_SCENE_MODE) {
            hitinfo_t hit = get_closest_hit(ray, *scene);
            buffer[i] = hit.did_hit ? hit.material.colour : (vec3) { 0, 0, 0 };
            continue;
        }

        vec3 colour = { 0, 0, 0 };
        for (usize j = 0; j < RAYS_PER_PIXEL; j++)
            colour = vadd3(colour, trace(ray, *scene, 4));

        buffer[i] = fmul3(1 / (f32) RAYS_PER_PIXEL, colour);
    }

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    // del
    free(buffer);
    del_scene(scene);
    destroy_model(knight);

    return 0;
}
