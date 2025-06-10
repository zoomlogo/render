#pragma once
#include "types.h"
#include "vec.h"

typedef struct {
    vec3 pos, dir, screen_x_dir;  // position, look direction
    f32 fov;  // vertical fov
    f32 zNear, zFar;  // clipping plane distance
    f32 aspect_ratio;  // width to height ratio
    f32 width, height;  // world space screen width/height
    mat4 screen_to_world_matrix;  // self-explanatory
} camera_t;

camera_t setup_camera(vec3 pos, vec3 dir, vec3 screen_x_dir, f32 fov, f32 zNear,
    usize screen_width, usize screen_height);
vec3 screen_to_world_coords(camera_t cam, vec3 coords);
