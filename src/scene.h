#pragma once
#include "types.h"
#include "vec.h"

#include "object.h"
#include "model.h"
#include "camera.h"

typedef struct sun_t {
    vec3 dir, colour;
    f32 focus, intensity;
} sun_t;

typedef struct scene_t {
    usize _o_alloc, num_objects, num_sources;
    object_t *objects;
    usize _m_alloc, num_models;
    model_t *models;

    sun_t sun;
    camera_t camera;
    bool enable_sky;
} scene_t;

// scene related functions
scene_t *new_scene(bool enable_sky);
void scene_setup_sun(scene_t *scene, vec3 dir, vec3 colour, f32 focus, f32 intensity);
void scene_add_sphere(scene_t *scene, sphere_t sphere);
void scene_add_triangle(scene_t *scene, triangle_t triangle);
void scene_add_model(scene_t *scene, model_t model);
void del_scene(scene_t *scene);
