#include <math.h>

#include "types.h"
#include "vec.h"
#include "camera.h"

static inline mat4 screen_to_world_matrix(camera_t cam, usize screen_width, usize screen_height) {
    // where do the basis vectors go?
    vec3 x = cam.screen_x_dir;
    vec3 z = cam.dir;
    vec3 y = cross3(x, z);

    // find point corresponding to (0, 0)
    vec3 centre = vadd3(cam.pos, fmul3(cam.zNear, cam.dir));
    vec3 origin = vadd3(centre, fmul3(-cam.width / 2, x));
    origin = vadd3(origin, fmul3(-cam.height / 2, y));

    // “normalize” x, y
    x = fmul3(cam.width / screen_width, x);
    y = fmul3(cam.height / screen_height, y);

    // build the matrix
    mat4 matrix = {
        x.x, y.x, 0, origin.x,
        x.y, y.y, 0, origin.y,
        x.z, y.z, 0, origin.z,
        0, 0, 0, 1
    };

    return matrix;
}

camera_t setup_camera(vec3 pos, vec3 dir, vec3 screen_x_dir, f32 fov, f32 zNear, usize screen_width, usize screen_height) {
    // basic setup
    camera_t cam = (camera_t) { pos, normalize3(dir), normalize3(screen_x_dir), fov };

    // by default, set zFar to INFINITY
    cam.zNear = zNear;
    cam.zFar = INFINITY;

    cam.aspect_ratio = screen_width / (f32) screen_height;
    cam.height = 2 * zNear * tanf(fov * M_PI / 360);
    cam.width = cam.height * cam.aspect_ratio;

    // setup the transformation matrix for screen to world
    cam.screen_to_world_matrix = screen_to_world_matrix(cam,
        screen_width, screen_height);

    return cam;
}

vec3 screen_to_world_coords(camera_t cam, vec3 coords) {
    vec4 in = { coords.x + 0.5 , coords.y + 0.5 , 0, 1 };
    vec4 out = mvmul4(cam.screen_to_world_matrix, in);
    return (vec3) { out.x, out.y, out.z };
}
