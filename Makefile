INCLUDES= -I ./include
FLAGS= -g # Debugging symbol
all:
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -o ./bin/main
