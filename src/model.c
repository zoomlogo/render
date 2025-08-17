#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "object.h"
#include "model.h"

struct face_t {
    usize vertex_count;
    usize *vertex_indicies;
};

model_t *load_model(FILE *file, material_t *material) {
    model_t *model = (model_t *) malloc(sizeof(model_t));
    model->N_vertices = 0;
    model->N_triangles = 0;
    // load the model
    // NOTE only supports 'v', 'o', 'f'.
    bool o_flag = false;
    bool v_flag = false;
    bool f_flag = false;

    // NOTE name cannot be more than 15 characters long
    // (last character being reserved for the null byte)
    char name[16];
    usize i = 0;

    // we need a dynamic buffer for vertices
    vec3 vertex;
    usize v_alloc = 16;
    vec3 *vbuf = (vec3 *) malloc(sizeof(vec3) * v_alloc);

    // bounding box
    f32 ax = 999, ay = 999, az = 999;  // minima
    f32 bx = -999, by = -999, bz = -999;  // maxima

    // reading faces is slightly complicated
    // theoretically we need 2 dynamic buffers but 1 is enough for us
    // NOTE there cannot be more than 16 vertices per face
    usize vertex_indicies[16];
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
            // NOTE only 3d verticies are supported (no x y z w)
            v_flag = false;
            fscanf(file, "%f %f %f", &(vertex.x), &(vertex.y), &(vertex.z));

            // bounding box
            ax = min(ax, vertex.x); ay = min(ay, vertex.y); az = min(az, vertex.z);
            bx = max(bx, vertex.x); by = max(by, vertex.y); bz = max(bz, vertex.z);

            memcpy(&vbuf[model->N_vertices++], &vertex, sizeof(vec3));
            // expand buffer if we exceed limits
            if (model->N_vertices >= v_alloc) {
                v_alloc *= 2;
                vbuf = realloc(vbuf, sizeof(vec3) * v_alloc);
            }
        } else v_flag = false;

        // read face
        if (c == 'f') f_flag = true;
        if (f_flag && c == ' ') {
            f_flag = false;
            j = 0;
            // read the vertex indicies into vertex_indicies
            while (fscanf(file, "%d/%d/%d", &vindex, &vx, &vxx) == 3)
                // NOTE negative vertex indicies are not handled.
                // other formats are not handled (x//x or only x)
                vertex_indicies[j++] = vindex - 1;
            face.vertex_count = j;
            model->N_triangles += j - 2;
            face.vertex_indicies = (usize *) malloc(sizeof(usize) * j);
            memcpy(face.vertex_indicies, &vertex_indicies, sizeof(usize) * j);

            // expand the fbuf array with face
            memcpy(&fbuf[f_num++], &face, sizeof(struct face_t));
            if (f_num >= f_alloc) {
                f_alloc *= 2;
                fbuf = realloc(fbuf, sizeof(struct face_t) * f_alloc);
            }
            // NOTE why that memory corruption?
            // 1. realloc was earlier called with new_size = f_alloc
            // which is not enough memory at all.
            // 2. the bounds (the if statement: if (f_num > f_alloc))
            // was wrong. it should be >= not >
            // 3. same mistakes were make in the vertex processing code
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
        usize pivot_index = face.vertex_indicies[0];
        for (j = 1; j < face.vertex_count - 1; j++)
            model->triangles[k++] = (triangle_t) {
                &model->vertices[pivot_index],
                &model->vertices[face.vertex_indicies[j]],
                &model->vertices[face.vertex_indicies[j + 1]],
                material
            };
        free(face.vertex_indicies);
    }
    free(fbuf);

    // generate bounds
    ax -= FEPS; ay -= FEPS; az -= FEPS;
    bx += FEPS; by += FEPS; bz += FEPS;
    model->bounds = (aabb_t) {
        (vec3) { ax, ay, az },
        (vec3) { bx, by, bz }
    };

    return model;
}

void destroy_model(model_t *model) {
    free(model->triangles);
    free(model->vertices);
    free(model);
}

void translate_model(model_t *model, vec3 pos) {
    for (usize i = 0; i < model->N_vertices; i++)
        model->vertices[i] = vadd3(model->vertices[i], pos);
}

void scale_model(model_t *model, vec3 scale) {
    for (usize i = 0; i < model->N_vertices; i++)
        model->vertices[i] = vmul3(model->vertices[i], scale);
}

void rotate_model(model_t *model, vec3 axis, f32 angle) {
    f32 cs = cosf(angle * M_PI / 180);
    f32 sn = sinf(angle * M_PI / 180);
    f32 x = axis.x, y = axis.y, z = axis.z;
    mat3 matrix = {{
        x*x*(1-cs)+cs, y*x*(1-cs)-z*sn, z*x*(1-cs)+y*sn,
        x*y*(1-cs)+z*sn, y*y*(1-cs)+cs, z*y*(1-cs)-x*sn,
        x*z*(1-cs)-y*sn, y*z*(1-cs)+x*sn, z*z*(1-cs)+cs
    }};
    for (usize i = 0; i < model->N_vertices; i++)
        model->vertices[i] = mvmul3(matrix, model->vertices[i]);
}
