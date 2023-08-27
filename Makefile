INCLUDES= -I ./include
FLAGS= -g # Debugging symbol
all:
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -L ./lib -lSDL2 -o ./bin/main
