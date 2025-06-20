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
    usize _o_alloc, num_objects;
    object_t *objects;
    usize _m_alloc, num_models;
    model_t *models;

    sun_t sun;
    camera_t camera;
} scene_t;

// scene related functions
scene_t *new_scene(void);
void scene_setup_camera(scene_t *scene /**/);
void scene_setup_sun(scene_t *scene /**/);
void scene_add_sphere(scene_t *scene /**/);
void scene_add_triangle(scene_t *scene /**/);
void scene_add_model(scene_t *scene /**/);
void del_scene(scene_t *scene);

// raytracing
hitinfo_t get_closest_hit(ray_t ray, object_t *objects, usize N);
vec3 get_environment_light(ray_t ray, sun_t sun);
vec3 trace(ray_t original_ray, object_t *objects, usize N, sun_t sun, usize num_bounces);
