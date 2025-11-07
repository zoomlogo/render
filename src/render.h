#pragma once
#include "types.h"
#include "vec.h"

#include "object.h"
#include "scene.h"

// raytracing
void get_closest_hit(ray_t *ray, scene_t *scene, hitinfo_t *out);
vec3 get_environment_light(ray_t *ray, sun_t sun);
vec3 trace(ray_t original_ray, scene_t *scene, usize num_bounces);
