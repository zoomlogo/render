#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/render.h"
INIT_TEST();

static void test_ray_sphere_intersection(void) {
    sphere_t sphere;
    ray_t ray;
    hitinfo_t hitinfo;
    // ray doesn't intersect sphere:
    // test 1a
    sphere = (sphere_t) { (vec3) {0, 2, 0}, 1 };
    ray = (ray_t) { (vec3) {0, 0, 0}, (vec3) {1, 0, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == false);
    // test 1b
    sphere = (sphere_t) { (vec3) {-1, 2, 0}, 2 };
    ray = (ray_t) { (vec3) {1, 0, 0}, (vec3) {1, 1, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == false);
    // ray is tangent to sphere:
    // test 2
    sphere = (sphere_t) { (vec3) {0, 0, 4}, 1 };
    ray = (ray_t) { (vec3) {1, 0, 4}, (vec3) {0, 1, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == true);
    // ray hits sphere at 2 distinct points:
    // test 3
    sphere = (sphere_t) { (vec3) {2, 1, 4}, 5 };
    ray = (ray_t) { (vec3) {1, 0, 3}, (vec3) {0, 1, 4} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == true);
}

void test_render(void) {
    BEGIN_TEST();

    test_ray_sphere_intersection();

    END_TEST();
}
