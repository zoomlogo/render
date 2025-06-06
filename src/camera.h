#pragma once
#include "types.h"
#include "vec.h"

typedef struct {
    vec3 pos, dir;  // look direction
    f32 fov, d,  // fov is vertical fov in degree, d is viewport distance
        ang,  // angle to rotate camera in dir
        aspect_ratio;  // width to height ratio
    usize vp_width, vp_height;
    f32 width, height;  // calculate using trig
} camera_t;

camera_t setup_camera(vec3 pos, vec3 dir, f32 fov, f32 d, f32 ang, usize sw, usize sh);
vec3 screen_to_world_coords(camera_t cam, vec3 coords);  // no idea how to impl
