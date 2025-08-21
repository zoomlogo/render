#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "vec.h"
#include "ppm.h"
#include "random.h"

#include "camera.h"
#include "object.h"
#include "render.h"
#include "model.h"
#include "bvh.h"

// #define SCREEN_WIDTH 1920
// #define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1200
#define SETUP_SCENE_MODE false

// buffer index to vec3 and vice versa
#define i2v(i) ({ const usize _i = (i); (vec3) { _i%SCREEN_WIDTH, SCREEN_HEIGHT - _i/SCREEN_WIDTH - 1, 0 }; })
#define v2i(v) ({ const vec3 _v = (v); (usize) ((SCREEN_HEIGHT - _v.y - 1) * SCREEN_WIDTH + _v.x); })

i32 main(void) {
    clock_t start_time, end_time;
    double duration;
    vec3 *buffer = (vec3 *) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(vec3));

    // set random
    pcg_init(time(NULL));

    // scene setup
    scene_t *scene = new_scene(false);
    if (SETUP_SCENE_MODE) printf("in setup scene mode\n");
    // camera setup
    scene->camera = setup_camera(
        (vec3) {0, 3.3, 10},  // pos
        (vec3) {0, 0, -1},  // dir
        (vec3) {1, 0, 0},  // screen_x_dir
        60,  // fov
        6,  // zNear
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    printf("(width, height): (%f, %f)\n", scene->camera.width, scene->camera.height);

    material_t floor_mat; mat_rough((vec3) { 0.6, 0.6, 0.6 }, &floor_mat);
    scene_add_triangle(scene, (triangle_t) {
        &((vec3) {4, 0, 4}),
        &((vec3) {-4, 0, 4}),
        &((vec3) {4, 0, -4}),
        &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 0, 4}),
            &((vec3) {4, 0, -4}),
            &((vec3) {-4, 0, -4}),
            &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, -4}),
            &((vec3) {4, 0, -4}),
            &((vec3) {-4, 0, -4}),
            &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, -4}),
            &((vec3) {4, 0, -4}),
            &((vec3) {4, 8, -4}),
            &floor_mat
    });

    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, -4}),
            &((vec3) {4, 8, -4}),
            &((vec3) {4, 8, 0}),
            &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, -4}),
            &((vec3) {4, 8, 0}),
            &((vec3) {-4, 8, 0}),
            &floor_mat
    });

    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, 4}),
            &((vec3) {4, 8, 4}),
            &((vec3) {4, 8, 2}),
            &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, 4}),
            &((vec3) {4, 8, 2}),
            &((vec3) {-4, 8, 2}),
            &floor_mat
    });

    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, 0}),
            &((vec3) {-1, 8, 0}),
            &((vec3) {-1, 8, 2}),
            &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, 0}),
            &((vec3) {-1, 8, 2}),
            &((vec3) {-4, 8, 2}),
            &floor_mat
    });

    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {4, 8, 0}),
            &((vec3) {1, 8, 0}),
            &((vec3) {1, 8, 2}),
            &floor_mat
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {4, 8, 0}),
            &((vec3) {1, 8, 2}),
            &((vec3) {4, 8, 2}),
            &floor_mat
    });

    material_t left_wall; mat_rough(RED, &left_wall);
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, -4}),
            &((vec3) {-4, 0, -4}),
            &((vec3) {-4, 0, 4}),
            &left_wall
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-4, 8, -4}),
            &((vec3) {-4, 8, 4}),
            &((vec3) {-4, 0, 4}),
            &left_wall
    });

    material_t right_wall; mat_rough(BLUE, &right_wall);
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {4, 8, -4}),
            &((vec3) {4, 0, -4}),
            &((vec3) {4, 0, 4}),
            &right_wall
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {4, 8, -4}),
            &((vec3) {4, 8, 4}),
            &((vec3) {4, 0, 4}),
            &right_wall
    });

    material_t light_source; mat_source(WHITE, 1.2, &light_source);
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-1, 8, 0}),
            &((vec3) {1, 8, 0}),
            &((vec3) {1, 8, 2}),
            &light_source
    });
    scene_add_triangle(scene, (triangle_t) {
            &((vec3) {-1, 8, 0}),
            &((vec3) {1, 8, 2}),
            &((vec3) {-1, 8, 2}),
            &light_source
    });

    // load knight
    FILE *knight_file = fopen("modal/Knight.obj", "r");
    material_t model_mat; mat_rough((vec3) { 0.3, 0.3, 0.4 }, &model_mat);
    model_t *knight = load_model(knight_file, &model_mat);
    fclose(knight_file);

    scale_model(knight, (vec3) {1.5, 1.5, 1.5});
    rotate_model(knight, (vec3) {0, 1, 0}, 270);

    bvh_t *bvh = new_bvh();

    start_time = clock();
    make_bvh(knight->triangles, knight->N_triangles, 6, bvh);
    end_time = clock();

    duration = 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;
    printf("bvh construction time: %.2f ms\n", duration);

    knight->bvh = bvh;

    scene_add_model(scene, *knight);


    // populate the buffer
    start_time = clock();
    const usize RAYS_PER_PIXEL = 1000;
    const usize BOUNCES = 5;
    hitinfo_t hit;
    for (usize i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        vec3 coords = i2v(i);
        vec3 world_coords = screen_to_world_coords(scene->camera, coords);
        vec3 dir = normalize3(vsub3(world_coords, scene->camera.pos));
        ray_t ray = { world_coords, dir };

        if (SETUP_SCENE_MODE) {
            get_closest_hit(&ray, scene, &hit);
            buffer[i] = hit.did_hit ? hit.material->colour : (vec3) { 0, 0, 0 };
            continue;
        }

        vec3 colour = { 0, 0, 0 };
        for (usize j = 0; j < RAYS_PER_PIXEL; j++)
            colour = vadd3(colour, trace(ray, scene, BOUNCES));

        buffer[i] = fmul3(1 / (f32) RAYS_PER_PIXEL, colour);
    }
    end_time = clock();

    duration = 1000.0 * (end_time - start_time) / CLOCKS_PER_SEC;
    printf("render time: %.2f ms\n", duration);

    // write to output image
    FILE *fp = fopen("out.ppm", "w");
    ppm(fp, buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    fclose(fp);

    // del
    free(buffer);
    del_scene(scene);
    destroy_model(knight);
    del_bvh(bvh);

    return 0;
}
