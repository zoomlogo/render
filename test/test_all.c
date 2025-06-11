#include <stdio.h>
#include "util.h"
#include "../src/types.h"
#include "test_vec.h"
#include "test_ppm.h"
#include "test_camera.h"
#include "test_render.h"

usize test_num = 0;
usize passed = 0;

i32 main(void) {
    test_vec();
    test_ppm();
    test_camera();
    test_render();

    return 0;
}
