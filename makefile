CC=clang
# compiler flags
FLAGS=-Wall -std=c23 -O3

# object file dependencies for target render:
RENDER_DEPS := obj/main.o obj/vec.o obj/ppm.o obj/model.o obj/bvh.o obj/pfor.o
RENDER_DEPS += obj/camera.o obj/render.o obj/random.o obj/object.o

# unit testing object file dependencies
TEST_DEPS := obj/vec.o obj/ppm.o obj/model.o obj/object.o
TEST_DEPS += obj/camera.o obj/render.o obj/random.o obj/bvh.o obj/pfor.o
TEST_DEPS += obj/test_all.o obj/test_vec.o obj/test_ppm.o obj/test_model.o obj/test_bvh.o
TEST_DEPS += obj/test_camera.o obj/test_render.o obj/test_random.o obj/test_object.o

render: $(RENDER_DEPS)
	$(CC) $(FLAGS) -lm $(RENDER_DEPS) -o render

test: $(TEST_DEPS)
	$(CC) $(FLAGS) -lm $(TEST_DEPS) -o test_all

# render:
obj/main.o: src/main.c
	$(CC) -c $(FLAGS) src/main.c -o obj/main.o

obj/vec.o: src/vec.c src/vec.h
	$(CC) -c $(FLAGS) src/vec.c -o obj/vec.o

obj/ppm.o: src/ppm.c src/ppm.h
	$(CC) -c $(FLAGS) src/ppm.c -o obj/ppm.o

obj/camera.o: src/camera.c src/camera.h
	$(CC) -c $(FLAGS) src/camera.c -o obj/camera.o

obj/render.o: src/render.c src/render.h
	$(CC) -c $(FLAGS) src/render.c -o obj/render.o

obj/random.o: src/random.c src/random.h
	$(CC) -c $(FLAGS) src/random.c -o obj/random.o

obj/bvh.o: src/bvh.c src/bvh.h
	$(CC) -c $(FLAGS) src/bvh.c -o obj/bvh.o

obj/model.o: src/model.c src/model.h
	$(CC) -c $(FLAGS) src/model.c -o obj/model.o

obj/object.o: src/object.c src/object.h
	$(CC) -c $(FLAGS) src/object.c -o obj/object.o

obj/pfor.o: src/pfor.c src/pfor.h
	$(CC) -c $(FLAGS) src/pfor.c -o obj/pfor.o

# test:
obj/test_all.o: test/test_all.c
	$(CC) -c $(FLAGS) test/test_all.c -o obj/test_all.o

obj/test_vec.o: test/test_vec.c test/test_vec.h
	$(CC) -c $(FLAGS) test/test_vec.c -o obj/test_vec.o

obj/test_ppm.o: test/test_ppm.c test/test_ppm.h
	$(CC) -c $(FLAGS) test/test_ppm.c -o obj/test_ppm.o

obj/test_camera.o: test/test_camera.c test/test_camera.h
	$(CC) -c $(FLAGS) test/test_camera.c -o obj/test_camera.o

obj/test_render.o: test/test_render.c test/test_render.h
	$(CC) -c $(FLAGS) test/test_render.c -o obj/test_render.o

obj/test_random.o: test/test_random.c test/test_random.h
	$(CC) -c $(FLAGS) test/test_random.c -o obj/test_random.o

obj/test_bvh.o: test/test_bvh.c test/test_bvh.h
	$(CC) -c $(FLAGS) test/test_bvh.c -o obj/test_bvh.o

obj/test_model.o: test/test_model.c test/test_model.h
	$(CC) -c $(FLAGS) test/test_model.c -o obj/test_model.o

obj/test_object.o: test/test_object.c test/test_object.h
	$(CC) -c $(FLAGS) test/test_object.c -o obj/test_object.o

# misc:
clean:
	rm render test_all

cleanall:
	rm -rf obj render out.ppm test_all

# make obj/ if it doesnt exist as soon as the file is done parsing
$(shell mkdir -p obj)
