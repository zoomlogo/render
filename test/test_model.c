#include <stdio.h>

#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/model.h"
INIT_TEST();

static void test_model_loading(void) {
    printf("> loading model...\n");
    FILE *file = fopen("modal/Knight.obj", "r");
    model_t *model = load_model(file);
    fclose(file);

    ASSERT(strcmp(model->name, "Knight") == 0);
    ASSERT(model->N_vertices == 235);
    ASSERT(eql3(model->vertices[0], ((vec3) {0, 0.012435f, -0.735253f})));
    ASSERT(eql3(model->vertices[234], ((vec3) {-0.140138f, 1.191317f, 0.750610f})));
    destroy_model(model);
}

void test_model(void) {
    BEGIN_TEST();

    test_model_loading();

    END_TEST();
}
