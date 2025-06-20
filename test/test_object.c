#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/object.h"
INIT_TEST();

static void test_ray_sphere_intersection(void) {
    sphere_t sphere;
    ray_t ray;
    hitinfo_t hitinfo;
    // ray doesn't intersect sphere:
    sphere = (sphere_t) { (vec3) {0, 2, 0}, 1 };
    ray = (ray_t) { (vec3) {0, 0, 0}, (vec3) {1, 0, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == false);
    // ray is tangent to sphere:
    sphere = (sphere_t) { (vec3) {0, 0, 0}, 1 };
    ray = (ray_t) { (vec3) {-1, -1, 0}, (vec3) {1, 0, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == true);
    ASSERT(hitinfo.dst == 1);
    // ray hits sphere at 2 distinct points:
    sphere = (sphere_t) { (vec3) {0, 4, 0}, 2 };
    ray = (ray_t) { (vec3) {0, 0, 0}, (vec3) {0, 1, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == true);
    ASSERT(hitinfo.dst == 2);
    // ray originates from inside sphere
    sphere = (sphere_t) { (vec3) {0, 0, 0}, 2 };
    ray = (ray_t) { (vec3) {0, 0, 0}, (vec3) {0, 1, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == true);
    ASSERT(hitinfo.dst == 2);
    // ray misses sphere (but lies on the line defined by ray)
    sphere = (sphere_t) { (vec3) {0, 2, 0}, 1 };
    ray = (ray_t) { (vec3) {0, 6, 0}, (vec3) {0, 1, 0} };
    hitinfo = ray_sphere_intersection(ray, sphere);
    ASSERT(hitinfo.did_hit == false);
}

void test_ray_triangle_intersection(void) {
    triangle_t triangle;
    ray_t ray;
    hitinfo_t hitinfo;
    // ray || triangle
    triangle = (triangle_t) { &((vec3) {0, 0, 0}), &((vec3) {0, 1, 0}), &((vec3) {1, 0, 0}) };
    ray = (ray_t) { (vec3) {0, 0, -1}, (vec3) {0, 1, 0} };
    hitinfo = ray_triangle_intersection(ray, triangle);
    ASSERT(hitinfo.did_hit == false);
    // ray hits plane but not triangle
    triangle = (triangle_t) { &((vec3) {0, 0, 0}), &((vec3) {0, 1, 0}), &((vec3) {1, 0, 0}) };
    ray = (ray_t) { (vec3) {1, 1, -1}, (vec3) {0, 0, 1} };
    hitinfo = ray_triangle_intersection(ray, triangle);
    ASSERT(hitinfo.did_hit == false);
    // ray hits triangle
    triangle = (triangle_t) { &((vec3) {0, 0, 0}), &((vec3) {0, 1, 0}), &((vec3) {1, 0, 0}) };
    ray = (ray_t) { (vec3) {0.2, 0.2, -1}, (vec3) {0, 0, 1} };
    hitinfo = ray_triangle_intersection(ray, triangle);
    ASSERT(hitinfo.did_hit == true);
    ASSERT(hitinfo.dst == 1);
    ASSERT(eql3(hitinfo.normal, ((vec3) {0, 0, -1})));
    ASSERT(eql3(hitinfo.point, ((vec3) {0.2, 0.2, 0})));
}

void test_object(void) {
    BEGIN_TEST();

    test_ray_sphere_intersection();
    test_ray_triangle_intersection();

    END_TEST();
}
