#pragma once
#include "types.h"
#include "object.h"

typedef struct bvh_t {
    aabb_t box;
    triangle_t *triangles;
    usize n;
    usize _n_alloc;
    struct bvh_t *left;
    struct bvh_t *right;
} bvh_t;

bvh_t *new_bvh(void);
void make_bvh(triangle_t *triangles, usize n, usize max_depth, bvh_t *out);
void del_bvh(bvh_t *root);

void ray_bvh_intersection(ray_t *ray, bvh_t *bvh, hitinfo_t *out);
void bvh_debug(ray_t *ray, bvh_t *bvh, hitinfo_t *out);
