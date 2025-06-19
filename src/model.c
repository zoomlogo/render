#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "render.h"
#include "model.h"

struct face_t {
    usize vertex_count;
    usize *vertex_indicies;
};

model_t *load_model(FILE *file, material_t material) {
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
    usize v_alloc = 16; // number of vec3 allocated for vbuf
    vec3 *vbuf = (vec3 *) malloc(sizeof(vec3) * v_alloc);

    // reading faces is slightly complicated
    usize vertex_indicies[16]; // pray no more than 16 vertices per face
    usize j;
    i32 vindex, vx, vxx;
    struct face_t face;

    usize f_alloc = 32;
    usize f_num = 0;
    struct face_t *fbuf = (struct face_t *) malloc(sizeof(struct face_t) * f_alloc);

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
            memcpy(&vbuf[model->N_vertices++], &vertex, sizeof(vec3));
            // expand buffer if we exceed limits
            if (model->N_vertices > v_alloc) {
                v_alloc *= 2;
                vbuf = realloc(vbuf, v_alloc);
            }
        } else v_flag = false;

        // read face
        if (c == 'f') f_flag = true;
        if (f_flag && c == ' ') {
            f_flag = false;
            j = 0;
            // read the vertex indicies into vertex_indicies
            while (fscanf(file, "%d/%d/%d", &vindex, &vx, &vxx) == 3)
                vertex_indicies[j++] = vindex - 1; // XXX what about negative indicies?
            face.vertex_count = j;
            if (face.vertex_count == 0) printf("WHAT THE FUCK!");
            model->N_triangles += j - 2;
            face.vertex_indicies = (usize *) malloc(sizeof(usize) * j);
            memcpy(face.vertex_indicies, &vertex_indicies, sizeof(usize) * j);

            // expand the fbuf array with face
            memcpy(&fbuf[f_num++], &face, sizeof(struct face_t));
            if (f_num > f_alloc) {
                f_alloc *= 2;
                fbuf = realloc(fbuf, f_alloc);
            }
        }
    }

    model->name = strdup(name);
    model->vertices = (vec3 *) malloc(sizeof(vec3) * model->N_vertices);
    memcpy(model->vertices, vbuf, sizeof(vec3) * model->N_vertices);
    free(vbuf);

    // generate triangles
    model->triangles = (triangle_t *) malloc(sizeof(triangle_t) * model->N_triangles);
    usize k = 0;
    for (i = 0; i < f_num; i++) {
        face = fbuf[i];
    printf("dbg i=%lu\n", i);
    printf("dbg face(vc=%lu, viptr=%lu)\n", face.vertex_count, face.vertex_indicies);
    // XXX MEMORY CORRUPTION IS HAPPENING SOMEWHERE HERE??
        usize pivot_index = face.vertex_indicies[0];
        for (j = 1; j < face.vertex_count - 1; j++) {
    printf("> dbg j=%lu\n", j);
            model->triangles[k++] = (triangle_t) {
                &model->vertices[pivot_index],
                &model->vertices[face.vertex_indicies[j]],
                &model->vertices[face.vertex_indicies[j + 1]],
                material
            };
        }
        free(face.vertex_indicies);
    }
    free(fbuf);

    return model;
}

void destroy_model(model_t *model) {
    free(model->triangles);
    free(model->vertices);
    free(model);
}
