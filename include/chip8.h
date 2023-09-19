#ifndef CHIP8_H // part of the header file
#define CHIP8_H // part of the header file

#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"

// Everyting related to chip8 goes inside the struct like memory, keyboard, and so on
struct chip8
{
	struct chip8_memory memory;
	struct chip8_registers registers;
	struct chip8_stack stack;

};


#endif // part of the header file
