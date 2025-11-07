#include <stdlib.h>
#include <float.h>

#include "object.h"
#include "types.h"
#include "vec.h"
#include "scene.h"

// scene handling
scene_t *new_scene(bool enable_sky) {
    scene_t *scene = (scene_t *) malloc(sizeof(scene_t));
    scene->_m_alloc = 4;
    scene->_o_alloc = 4;

    scene->num_models = 0;
    scene->num_objects = 0;
    scene->num_sources = 0;

    scene->models = (model_t *) malloc(sizeof(model_t) * scene->_m_alloc);
    scene->objects = (object_t *) malloc(sizeof(object_t) * scene->_o_alloc);

    scene->enable_sky = enable_sky;

    return scene;
}

void scene_setup_sun(scene_t *scene, vec3 dir, vec3 colour, f32 focus, f32 intensity) {
    scene->sun = (sun_t) {normalize3(dir), colour, focus, intensity};
}

void scene_add_sphere(scene_t *scene, sphere_t sphere) {
    object_t object = {false, .sphere = sphere};
    scene->objects[scene->num_objects++] = object;
    if (scene->num_objects >= scene->_o_alloc) {
        scene->_o_alloc *= 2;
        scene->objects = realloc(scene->objects, sizeof(object_t) * scene->_o_alloc);
    }

    if (sphere.material->emission_strength > 0) {
        // put the light sources first
        object_t temp = scene->objects[scene->num_sources];
        scene->objects[scene->num_sources++] = scene->objects[scene->num_objects - 1];
        scene->objects[scene->num_objects - 1] = temp;
    }
}

void scene_add_triangle(scene_t *scene, triangle_t triangle) {
    object_t object = {true, .triangle = triangle};
    scene->objects[scene->num_objects++] = object;
    if (scene->num_objects >= scene->_o_alloc) {
        scene->_o_alloc *= 2;
        scene->objects = realloc(scene->objects, sizeof(object_t) * scene->_o_alloc);
    }

    if (triangle.material->emission_strength > 0) {
        // put the light sources first
        object_t temp = scene->objects[scene->num_sources];
        scene->objects[scene->num_sources++] = scene->objects[scene->num_objects - 1];
        scene->objects[scene->num_objects - 1] = temp;
    }
}

void scene_add_model(scene_t *scene, model_t model) {
    scene->models[scene->num_models++] = model;
    if (scene->num_models >= scene->_m_alloc) {
        scene->_m_alloc *= 2;
        scene->models = realloc(scene->models, sizeof(model_t) * scene->_m_alloc);
    }
}

void del_scene(scene_t *scene) {
    free(scene->models);
    free(scene->objects);
    free(scene);
}
