#include <math.h>
#include <stdio.h>

#include "types.h"
#include "vec.h"
#include "camera.h"

static inline mat4 screen_to_world_matrix(camera_t cam) {
    vec3 x = cam.screen_x_dir;
    vec3 z = cam.dir;
    vec3 y = cross3(x, z);
    // find point corresponding to (0, 0)
    vec3 origin = vadd3(cam.pos, fmul3(cam.zNear, cam.dir));
    origin = vadd3(origin, fmul3(-cam.width / 2, x));
    origin = vadd3(origin, fmul3(-cam.height / 2, y));

    mat4 matrix = {
        x.x, y.x, 0, origin.x,
        x.y, y.y, 0, origin.y,
        x.z, y.z, 0, origin.z,
        0, 0, 0, 1
    };

    return matrix;
}

camera_t setup_camera(vec3 pos, vec3 dir, vec3 screen_x_dir, f32 fov, f32 zNear, usize sw, usize sh) {
    // basic setup
    camera_t cam = (camera_t) { pos, dir, screen_x_dir, fov };
    // by default, set zFar to INFINITY
    cam.zNear = zNear;
    cam.zFar = INFINITY;
    cam.aspect_ratio = sw / (f32) sh;
    cam.vp_width = sw; cam.vp_height = sh;
    cam.height = 2 * zNear * tanf(fov * M_PI / 360);
    cam.width = cam.height * cam.aspect_ratio;

    // setup the transformation matrix for screen to world
    cam.screen_to_world_matrix = screen_to_world_matrix(cam);

    return cam;
}

vec3 screen_to_world_coords(camera_t cam, vec3 coords) {
    vec4 in = { coords.x, coords.y, 0, 1 };
    vec4 out = mvmul4(cam.screen_to_world_matrix, in);
    return (vec3) { out.x, out.y, out.z };
}
