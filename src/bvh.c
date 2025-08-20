#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "object.h"
#include "bvh.h"

#include <stdio.h>

static void add_triangle(bvh_t *bvh, triangle_t *triangle) {
    memcpy(&bvh->triangles[bvh->n++], triangle, sizeof(triangle_t));
    if (bvh->n >= bvh->_n_alloc) {
        bvh->_n_alloc += 8;
        bvh->triangles = realloc(bvh->triangles, sizeof(triangle_t) * bvh->_n_alloc);
    }
}

static void split_bvh(bvh_t *parent, usize depth) {
    if (depth == 0) return;

    parent->left = new_bvh();
    parent->right = new_bvh();

    vec3 centre = fmul3(0.5, vadd3(parent->box.a, parent->box.b));
    usize max_i = centre.x > centre.y ? (centre.x > centre.z ? 0 : 2) : 1;
    float axis_centre = centre.x > centre.y ? (centre.x > centre.z ? centre.x : centre.z) : centre.y;
    for (usize i = 0; i < parent->n; i++) {
        // split along longest axis
        triangle_t *tri = &parent->triangles[i];
        vec3 tri_centre = fmul3(1 / 3.f, vadd3(*tri->v1, vadd3(*tri->v2, *tri->v3)));
        float tri_centre_axis = max_i == 0 ? tri_centre.x : (max_i == 1 ? tri_centre.y : tri_centre.z);
        bvh_t *child = tri_centre_axis < axis_centre ? parent->left : parent->right;
        add_triangle(child, tri);
        grow_to_include_triange(&child->box, tri);
    }

    split_bvh(parent->left, depth - 1);
    split_bvh(parent->right, depth - 1);

    if (parent->left && parent->right) {
        parent->n = parent->_n_alloc = 0;
        free(parent->triangles);
        parent->triangles = NULL;
    }
}

bvh_t *new_bvh(void) {
    bvh_t *root = (bvh_t *) malloc(sizeof(bvh_t));
    root->left = root->right = NULL;
    root->_n_alloc = 8;
    root->triangles = (triangle_t *) malloc(sizeof(triangle_t) * root->_n_alloc);
    root->n = 0;
    new_aabb(&root->box);
    return root;
}

void make_bvh(triangle_t *triangles, usize n, usize max_depth, bvh_t *out) {
    aabb_t box;
    out->left = out->right = NULL;
    new_aabb(&box);
    for (usize i = 0; i < n; i++) {
        grow_to_include_triange(&box, &triangles[i]);
    }

    out->box = box;
    out->n = n;
    out->_n_alloc = n;
    free(out->triangles);
    out->triangles = (triangle_t *) malloc(sizeof(triangle_t) * n);
    memcpy(out->triangles, triangles, n * sizeof(triangle_t));
    split_bvh(out, max_depth);
}

void ray_bvh_intersection(ray_t *ray, bvh_t *bvh, hitinfo_t *out) {
    if (!ray_aabb_intersection(ray, &bvh->box)) return;

    hitinfo_t hit;
    bvh_t *stack[16];
    usize index = 0;

    stack[index++] = bvh;

    while (index > 0) {
        bvh_t *this = stack[--index];

        if (ray_aabb_intersection(ray, &this->box)) {
            if (this->triangles) { // childless
                for (usize i = 0; i < this->n; i++) {
                    ray_triangle_intersection(ray, &this->triangles[i], &hit);
                    if (hit.did_hit && min(hit.dst, out->dst) != out->dst)  // depth
                        *out = hit;
                }
            } else {
                stack[index++] = this->left;
                stack[index++] = this->right;
            }
        }
    }
}

void del_bvh(bvh_t *root) {
    if (root->left)
        del_bvh(root->left);
    if (root->right)
        del_bvh(root->right);
    free(root);
}
