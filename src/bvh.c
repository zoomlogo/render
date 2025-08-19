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

    float x_centre = (parent->box.a.x + parent->box.b.x) / 2;
    for (usize i = 0; i < parent->n; i++) {
        // divide based on x coord
        // TODO split along longest axis
        triangle_t *tri = &parent->triangles[i];
        float x_tri_centre = (tri->v1->x + tri->v2->x + tri->v3->x) / 3;
        bvh_t *child = x_tri_centre < x_centre ? parent->left : parent->right;
        add_triangle(child, tri);
        grow_to_include_triange(&child->box, tri);
    }

    split_bvh(parent->left, depth - 1);
    split_bvh(parent->right, depth - 1);

    parent->n = parent->_n_alloc = 0;
    free(parent->triangles);
    parent->triangles = NULL;
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
    // TODO make this iterative
    if (!ray_aabb_intersection(ray, &bvh->box)) return;

    hitinfo_t hit;
    if (bvh->triangles) {
        for (usize i = 0; i < bvh->n; i++) {
            ray_triangle_intersection(ray, &bvh->triangles[i], &hit);

            // depth checking
            if (hit.did_hit && min(hit.dst, out->dst) != out->dst)
                *out = hit;
        }
    }

    // no fear of numerical error here
    if (out->did_hit && out->dst == hit.dst) return;

    // recurse
    if (bvh->left)
        ray_bvh_intersection(ray, bvh->left, out);
    if (bvh->right)
        ray_bvh_intersection(ray, bvh->right, out);
}

void del_bvh(bvh_t *root) {
    if (root->left)
        del_bvh(root->left);
    if (root->right)
        del_bvh(root->right);
    free(root);
}


void bvh_debug(ray_t *ray, bvh_t *bvh, hitinfo_t *out) {
    new_hitinfo(out);

    if (ray_aabb_intersection(ray, &bvh->box)) {
        if (ray_aabb_intersection(ray, &bvh->right->right->right->right->left->right->box))
            out->did_hit = true;
    }
}
