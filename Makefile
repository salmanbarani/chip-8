INCLUDES= -I ./include
FLAGS= -g # Debugging symbol


OBJECTS=./build/chip8memory.o
all: $(OBJECTS)
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -o ./bin/main -lSDL2 

run:
	./bin/main

./build/chip8memory.o:src/chip8memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8memory.c -c -o ./build/chip8memory.o

clean:
	rm -v build/*
