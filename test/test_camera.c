#include "util.h"
#include "../src/types.h"
#include "../src/vec.h"
#include "../src/camera.h"
INIT_TEST();

void test_camera(void) {
    BEGIN_TEST();

    camera_t cam = setup_camera(
        (vec3) {1, 1, 0},
        (vec3) {0, 0, 2},
        (vec3) {2, 0, 0},
        90, 4,
        300, 200
    );
    ASSERT(eql3(cam.pos, ((vec3) {1, 1, 0})));
    ASSERT(eql3(cam.dir, ((vec3) {0, 0, 1})));
    ASSERT(eql3(cam.screen_x_dir, ((vec3) {1, 0, 0})));
    ASSERT(cam.fov == 90);
    ASSERT(cam.zNear == 4 && cam.zFar == INFINITY);
    ASSERT(cam.aspect_ratio == 1.5);
    ASSERT(cam.width == 12);
    ASSERT(cam.height == 8);

    // check if camera's matrix is correct or not
    bool assertion = true;
    f32 expected_mat[] = {
        0.04, 0, 0, -5,
        0, -0.04, 0, 5,
        0, 0, 0, 4,
        0, 0, 0, 1
    };
    for (u8 i = 0; i < 16; i++)
        assertion &= expected_mat[i] == cam.screen_to_world_matrix.v[i];
    ASSERT(assertion);

    // test screen to world
    ASSERT(eql3(screen_to_world_coords(cam, (vec3) {0, 0, 0}), ((vec3) {-4.98, 4.98, 4})));
    ASSERT(eql3(screen_to_world_coords(cam, (vec3) {100, 0, 0}), ((vec3) {-0.98, 4.98, 4})));
    ASSERT(eql3(screen_to_world_coords(cam, (vec3) {0, 100, 0}), ((vec3) {-4.98, 0.98, 4})));

    END_TEST();
}
