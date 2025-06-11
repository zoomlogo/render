CC=clang
# compiler flags
FLAGS=-Wall -std=c23

# object file dependencies for target render:
RENDER_DEPS := obj/main.o obj/vec.o obj/ppm.o
RENDER_DEPS += obj/camera.o obj/render.o

# unit testing object file dependencies
TEST_DEPS := obj/vec.o obj/ppm.o
TEST_DEPS += obj/camera.o obj/render.o
TEST_DEPS += obj/test_all.o obj/test_vec.o obj/test_ppm.o

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

# test:
obj/test_all.o: test/test_all.c
	$(CC) -c $(FLAGS) test/test_all.c -o obj/test_all.o

obj/test_vec.o: test/test_vec.c test/test_vec.h
	$(CC) -c $(FLAGS) test/test_vec.c -o obj/test_vec.o

obj/test_ppm.o: test/test_ppm.c test/test_ppm.h
	$(CC) -c $(FLAGS) test/test_ppm.c -o obj/test_ppm.o

# misc:
clean:
	rm render test_all

cleanall:
	rm -rf obj render out.ppm test_all

# make obj/ if it doesnt exist as soon as the file is done parsing
$(shell mkdir -p obj)
