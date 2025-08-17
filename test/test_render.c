#include "util.h"
#include "../src/types.h"
#include "../src/model.h"
#include "../src/vec.h"
#include "../src/ppm.h"
#include "../src/render.h"
#include <string.h>
INIT_TEST();

static void test_scene_setup(void) {
    scene_t *scene = new_scene();
    scene_setup_sun(scene, (vec3) {0,1,0}, WHITE, 8, 10);

    ASSERT(eql3(scene->sun.colour, WHITE));
    ASSERT(eql3(scene->sun.dir, ((vec3) {0,1,0})));
    ASSERT(scene->sun.focus == 8);
    ASSERT(scene->sun.intensity == 10);

    del_scene(scene);
}

static void test_scene_add_object(void) {
    scene_t *scene = new_scene();
    scene_add_sphere(scene, (sphere_t) { (vec3) {-3, 1, 1}, 1, &(material_t) { BLUE } });

    ASSERT(scene->num_objects == 1);
    ASSERT(scene->_o_alloc == 4);
    ASSERT(scene->objects[0].sphere.r == 1);

    scene_add_triangle(scene, (triangle_t) {
        &(vec3) {0,0,0},
        &(vec3) {0,1,0},
        &(vec3) {0,1,1},
        &(material_t) { BLUE },
    });

    ASSERT(scene->num_objects == 2);
    ASSERT(scene->_o_alloc == 4);
    ASSERT(eql3(*scene->objects[1].triangle.v3, ((vec3) {0,1,1})));

    del_scene(scene);
}

static void test_scene_add_source(void) {
    scene_t *scene = new_scene();
    scene_add_sphere(scene, (sphere_t) { (vec3) {-3, 1, 1}, 1, &(material_t) { BLUE } });
    scene_add_sphere(scene, (sphere_t) { (vec3) {-3, 3, 1}, 2, &(material_t) { BLUE } });
    scene_add_sphere(scene, (sphere_t) { (vec3) {-3, 1, 4}, 1, &(material_t) { BLUE } });
    scene_add_sphere(scene, (sphere_t) { (vec3) {-3, -2, 1}, 5, &(material_t) { BLUE } });

    ASSERT(scene->num_objects == 4);
    ASSERT(scene->_o_alloc == 8);
    ASSERT(scene->objects[3].sphere.r == 5);

    scene_add_triangle(scene, (triangle_t) {
        &(vec3) {0,0,0},
        &(vec3) {0,1,0},
        &(vec3) {0,1,1},
        &(material_t) { WHITE, 5, WHITE },
    });

    ASSERT(scene->num_objects == 5);
    ASSERT(scene->_o_alloc == 8);
    ASSERT(eql3(*scene->objects[0].triangle.v3, ((vec3) {0,1,1})));

    del_scene(scene);
}


void test_scene_add_model(void) {
    scene_t *scene = new_scene();
    FILE *file = fopen("modal/Cube.obj", "r");
    model_t *model = load_model(file, &(material_t) { WHITE });
    fclose(file);
    scene_add_model(scene, *model);

    ASSERT(scene->num_models == 1);
    ASSERT(scene->_m_alloc == 4);
    ASSERT(strcmp(scene->models[0].name, "Cube") == 0);
    ASSERT(scene->models[0].N_triangles == 12);

    del_scene(scene);
    destroy_model(model);
}

void test_render(void) {
    BEGIN_TEST();

    test_scene_setup();
    test_scene_add_object();
    test_scene_add_source();
    test_scene_add_model();

    END_TEST();
}
