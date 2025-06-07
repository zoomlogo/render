#pragma once
#include "types.h"
#include "vec.h"

typedef struct {
    vec3 pos, dir, screen_x_dir;  // position, look direction
    f32 fov;  // vertical fov
    f32 zNear, zFar;  // clipping plane distance
    f32 aspect_ratio;  // width to height ratio
    usize vp_width, vp_height;  // screen width height (do i need this?)
    f32 width, height;  // calculate using trig
    mat4 screen_to_world_matrix;
} camera_t;

camera_t setup_camera(vec3 pos, vec3 dir, vec3 screen_x_dir, f32 fov, f32 zNear, usize sw, usize sh);
vec3 screen_to_world_coords(camera_t cam, vec3 coords);
