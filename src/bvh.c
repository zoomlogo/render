#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "object.h"
#include "bvh.h"

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

    float z_centre = (parent->box.a.z + parent->box.b.z) / 2;
    for (usize i = 0; i < parent->n; i++) {
        // divide based on z coord
        triangle_t *tri = &parent->triangles[i];
        float z_tri_centre = (tri->v1->z + tri->v2->z + tri->v3->z) / 3;
        bvh_t *child = z_tri_centre < z_centre ? parent->left : parent->right;
        add_triangle(child, tri);
        grow_to_include_triange(&child->box, tri);
    }

    split_bvh(parent->left, depth - 1);
    split_bvh(parent->right, depth - 1);

    parent->n = parent->_n_alloc = 0;
    free(parent->triangles);
}

bvh_t *new_bvh(void) {
    bvh_t *root = malloc(sizeof(bvh_t));
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
    out->triangles = triangles;
    split_bvh(out, max_depth);
}

void del_bvh(bvh_t *root) {
    if (root->left)
        del_bvh(root->left);
    if (root->right)
        del_bvh(root->right);
    free(root);
}
