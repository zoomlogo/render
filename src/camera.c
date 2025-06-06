#include <math.h>

#include "types.h"
#include "vec.h"
#include "camera.h"

camera_t setup_camera(vec3 pos, vec3 dir, f32 fov, f32 d, f32 ang, usize sw, usize sh) {
    camera_t cam;
    cam = (camera_t) { pos, dir, fov, d, ang };
    cam.aspect_ratio = sw / (f32) sh;
    cam.vp_width = sw; cam.vp_height = sh;
    cam.height = 2 * d * tanf(fov * M_PI / 360);
    cam.width = cam.height * cam.aspect_ratio;
    return cam;
}

