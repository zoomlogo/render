#include <stdio.h>
#include "util.h"
#include "../src/types.h"
#include "test_vec.h"
#include "test_ppm.h"
#include "test_camera.h"
#include "test_render.h"
#include "test_random.h"
#include "test_model.h"
#include "test_object.h"

usize test_num = 0;
usize passed = 0;

i32 main(void) {
    test_vec();
    test_ppm();
    test_random();
    test_camera();
    test_object();
    test_render();
    test_model();
    return 0;
}
