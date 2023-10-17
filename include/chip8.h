#ifndef CHIP8_H // part of the header file
#define CHIP8_H // part of the header file

#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"
#include <stddef.h>

// Everyting related to chip8 goes inside the struct like memory, keyboard, and so on
struct chip8
{
	struct chip8_memory memory;
	struct chip8_registers registers;
	struct chip8_stack stack;
	struct chip8_keyboard keyboard;
	struct chip8_screen screen;
};

void chip8_init(struct chip8* chip8);
void chip8_load(struct chip8* chip8, const char* buf, size_t size);
void chip8_exec(struct chip8* chip8, unsigned short opcode);

#endif // part of the header file
