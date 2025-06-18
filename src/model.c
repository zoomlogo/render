#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "render.h"
#include "model.h"

model_t *load_model(FILE *file) {
    model_t *model = (model_t *) malloc(sizeof(model_t));
    model->N_vertices = 0;
    model->N_triangles = 0;
    // load the model
    // only support 'v', 'o', 'f'.
    bool o_flag = false;
    bool v_flag = false;
    bool f_flag = false;

    char name[16];
    usize i = 0;

    // we need a dynamic buffer for vertices
    vec3 vertex;  // write here, copy to mem later
    usize vbytes_alloc = 16; // number of bytes allocated for vbuf
    vec3 *vbuf = (vec3 *) malloc(sizeof(vec3) * vbytes_alloc);

    // reading faces is more complicated

    i32 c;
    while ((c = getc(file)) != EOF) {
        // read name, easy to understand
        if (c == 'o') o_flag = true;
        if (o_flag && c == ' ' && i == 0) {
            while ((c = getc(file)) != '\n') name[i++] = c;
            name[i] = '\0';
            o_flag = false;
        }

        // read vertex
        if (c == 'v') v_flag = true;
        else if (v_flag && c == ' ') {
            v_flag = false;
            fscanf(file, "%f %f %f", &(vertex.x), &(vertex.y), &(vertex.z));
            memcpy(&(vbuf[model->N_vertices++]), &vertex, sizeof(vec3));
            // expand buffer if we exceed limits
            if (model->N_vertices > vbytes_alloc) {
                vbytes_alloc *= 2;
                vbuf = realloc(vbuf, vbytes_alloc);
            }
        } else v_flag = false;

        // read face
        if (c == 'f') f_flag = true;
    }

    model->name = strdup(name);
    model->vertices = (vec3 *) malloc(sizeof(vec3) * model->N_vertices);
    memcpy(model->vertices, vbuf, sizeof(vec3) * model->N_vertices);
    free(vbuf);

    return model;
}

void destroy_model(model_t *model) {
    free(model->triangles);
    free(model->vertices);
    free(model);
}
