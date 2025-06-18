#include <stdio.h>

#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/ppm.h"
#include "../src/model.h"
INIT_TEST();

static void test_model_loading_cube(void) {
    printf("> loading model...\n");
    FILE *file = fopen("modal/Cube.obj", "r");
    model_t *model = load_model(file);
    fclose(file);

    ASSERT(strcmp(model->name, "Cube") == 0);
    ASSERT(model->N_vertices == 8);
    ASSERT(eql3(model->vertices[0], ((vec3) {0, 0, 0})));
    ASSERT(eql3(model->vertices[7], ((vec3) {1, 1, 1})));
    destroy_model(model);
}

void test_model(void) {
    BEGIN_TEST();

    test_model_loading_cube();

    END_TEST();
}
