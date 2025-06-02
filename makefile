CC=clang
# compiler flags
FLAGS=-Wall -Ddebug -std=c23

render: obj/main.o obj/vec.o obj/ppm.o
	$(CC) $(FLAGS) obj/main.o obj/vec.o obj/ppm.o -o render

obj/main.o: src/main.c
	$(CC) -c $(FLAGS) src/main.c -o obj/main.o

obj/vec.o: src/vec.c src/vec.h
	$(CC) -c $(FLAGS) src/vec.c -o obj/vec.o

obj/ppm.o: src/ppm.c src/ppm.h
	$(CC) -c $(FLAGS) src/ppm.c -o obj/ppm.o

clean:
	rm render

cleanall:
	rm -rf obj render out.ppm

$(shell mkdir -p obj)
