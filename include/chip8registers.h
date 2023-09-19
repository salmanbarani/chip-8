#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include "config.h"

struct chip8_registers 
{
	unsigned char V[CHIP8_TOTAL_DATA_REGISTERS];
	unsigned char I; // 16-bit
	unsigned char delay_timer; // 8-bit
	unsigned char sound_timer; // 8-bit
	unsigned short PC; // Program Counter points to instruction memory
	unsigned char SP; // Stack Pointer
};

#endif
