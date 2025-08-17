#pragma once
#include <stdio.h>

#include "vec.h"
#include "object.h"

typedef struct model_t {
    char *name;
    usize N_vertices;
    vec3 *vertices;
    usize N_triangles;
    triangle_t *triangles;
    aabb_t bounds;
} model_t;

model_t *load_model(FILE *file, material_t *material);
void destroy_model(model_t *model);

void translate_model(model_t *model, vec3 pos);
void scale_model(model_t *model, vec3 scale);
void rotate_model(model_t *model, vec3 axis, f32 angle);
