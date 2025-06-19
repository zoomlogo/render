#include <stdio.h>

#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/ppm.h"
#include "../src/model.h"
INIT_TEST();

static void test_model_loading_cube(void) {
    FILE *file = fopen("modal/Cube.obj", "r");
    model_t *model = load_model(file, (material_t) { WHITE });
    fclose(file);

    ASSERT(strcmp(model->name, "Cube") == 0);
    ASSERT(model->N_vertices == 8);
    ASSERT(eql3(model->vertices[0], ((vec3) {0, 0, 0})));
    ASSERT(eql3(model->vertices[7], ((vec3) {1, 1, 1})));
    ASSERT(eql3(model->vertices[4], ((vec3) {1, 0, 0})));

    ASSERT(model->N_triangles == 12);
    triangle_t t1 = model->triangles[0];
    ASSERT(eql3(*t1.v1, ((vec3) {0, 0, 0})));
    ASSERT(eql3(*t1.v2, ((vec3) {0, 1, 0})));
    ASSERT(eql3(*t1.v3, ((vec3) {0, 1, 1})));
    t1 = model->triangles[1];
    ASSERT(eql3(*t1.v1, ((vec3) {0, 0, 0})));
    ASSERT(eql3(*t1.v2, ((vec3) {0, 1, 1})));
    ASSERT(eql3(*t1.v3, ((vec3) {0, 0, 1})));
    destroy_model(model);
}

static void test_model_loading_knight(void) {
    FILE *file = fopen("modal/Knight.obj", "r");
    model_t *model = load_model(file, (material_t) { GREEN });
    fclose(file);

    ASSERT(strcmp(model->name, "Knight") == 0);
    ASSERT(model->N_vertices == 235);
    ASSERT(model->N_triangles == 456);
    ASSERT(eql3(model->triangles[0].material.colour, GREEN));
    destroy_model(model);
}

void test_translate_model(void) {
    FILE *file = fopen("modal/Cube.obj", "r");
    model_t *model = load_model(file, (material_t) { WHITE });
    fclose(file);

    translate_model(model, (vec3) {5, 3, 4});

    vec3 *vertices = model->vertices;
    ASSERT(eql3(vertices[0], ((vec3) {5, 3, 4})));
    ASSERT(eql3(vertices[3], ((vec3) {5, 4, 5})));
    ASSERT(eql3(vertices[4], ((vec3) {6, 3, 4})));
    ASSERT(eql3(vertices[7], ((vec3) {6, 4, 5})));
    destroy_model(model);
}

void test_scale_model(void) {
    FILE *file = fopen("modal/Cube.obj", "r");
    model_t *model = load_model(file, (material_t) { WHITE });
    fclose(file);

    scale_model(model, (vec3) {1, 3, 0.5f});

    vec3 *vertices = model->vertices;
    ASSERT(eql3(vertices[0], ((vec3) {0, 0, 0})));
    ASSERT(eql3(vertices[3], ((vec3) {0, 3, 0.5})));
    ASSERT(eql3(vertices[4], ((vec3) {1, 0, 0})));
    ASSERT(eql3(vertices[7], ((vec3) {1, 3, 0.5})));
    destroy_model(model);
}

void test_rotate_model(void) {
    FILE *file = fopen("modal/Cube.obj", "r");
    model_t *model = load_model(file, (material_t) { WHITE });
    fclose(file);

    rotate_model(model, (vec3) {0,0,1}, 90);

    vec3 *vertices = model->vertices;
    ASSERT(eql3(vertices[0], ((vec3) {0, 0, 0})));
    ASSERT(length3(vsub3(vertices[3], ((vec3) {-1, 0, 1}))) < 0.000001 );
    ASSERT(length3(vsub3(vertices[4], ((vec3) {0, 1, 0}))) < 0.000001 );
    ASSERT(length3(vsub3(vertices[7], ((vec3) {-1, 1, 1}))) < 0.000001 );
    destroy_model(model);
}

void test_model(void) {
    BEGIN_TEST();

    test_model_loading_cube();
    test_model_loading_knight();
    test_translate_model();
    test_scale_model();
    test_rotate_model();

    END_TEST();
}
