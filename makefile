CC=gcc
# compiler flags
FLAGS=-Wall -Ddebug -std=c23

render: obj/main.o
	$(CC) $(FLAGS) obj/main.o -o render

obj/main.o: src/main.c
	$(CC) -c $(FLAGS) src/main.c -o obj/main.o

clean:
	rm render

cleanall:
	rm -rf obj render

$(shell mkdir -p obj)
