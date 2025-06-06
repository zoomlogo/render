#pragma once
#include "types.h"
#include "vec.h"

typedef struct {
    vec3 pos, dir;
    f32 fov, d,  // fov is vertical fov, d is viewport distance
        aspect_ratio;  // width to height ratio
} camera_t;


