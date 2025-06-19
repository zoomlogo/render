#pragma once
#include <stdio.h>

#include "vec.h"
#include "render.h"

typedef struct model_t {
    char *name;
    usize N_vertices;
    vec3 *vertices;
    usize N_triangles;
    triangle_t *triangles;
} model_t;

model_t *load_model(FILE *file, material_t material);
// void transform_model(model_t *model, mat4 mat);
void destroy_model(model_t *model);
