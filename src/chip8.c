#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"



const char chip8_default_character_set[] = {
	0xf0, 0x90, 0x90, 0x90, 0xf0, // 0 
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2	
	0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
	0x90, 0x90, 0xf0, 0x10, 0x10, // 4
	0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
	0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
	0xf0, 0x10, 0x20, 0x40, 0x40, // 7
	0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
	0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
	0xf0, 0x90, 0xf0, 0x90, 0x90, // A
	0xe0, 0x90, 0xe0, 0x90, 0xe0, // B
	0xf0, 0x80, 0x80, 0x80, 0xf0, // C 
	0xe0, 0x90, 0x90, 0x90, 0xe0, // D
	0xf0, 0x80, 0xf0, 0x80, 0xf0, // E	
	0xf0, 0x80, 0xf0, 0x80, 0x80, // F
};

void chip8_init(struct chip8* chip8)
{
	memset(chip8, 0, sizeof(struct chip8));
	memcpy(&chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}


void chip8_load(struct chip8* chip8, const char* buf, size_t size)
{
	assert(size+CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
	memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size); 
	chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static void chip8_exec_extended_eight(struct chip8* chip8, unsigned short opcode)
{
	unsigned char x = (opcode >> 8) & 0x000f;
	unsigned char y = (opcode >> 4) & 0x000f;
	unsigned char final_four_bits = opcode & 0x000f;
	unsigned short temp = 0;

	switch(final_four_bits)
	{
		case 0x00: // 8xy0 - LD Vx, Vy, Vx = Vy
			chip8->registers.V[x] = chip8->registers.V[y];
		break;
		
		case 0x01: // 8xy1 - OR Vx, Vy, Performs a bitwise OR on Vx and Vy & stores the result in Vx
			chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
		break;

		case 0x02: // 8xy2 And Vx, Vy, Performs a bitwise And on Vx and Vy & stores the result in Vx
			chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];

		break;

		case 0x03: // 8xy3 XOR Vx, Vy, Performs a bitwise Xor on Vx and Vy  stores the result in Vx

			chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
		break;

		case 0x04: // 8xy4 ADD Vx, Vy, Set Vx = Vx + Vy, Set Vf = carry
			temp = chip8->registers.V[x] + chip8->registers.V[y];
			
			chip8->registers.V[0x0f] = temp > 0xff ? true : false;
			chip8->registers.V[x] = temp;
		break;
		
		case 0x05: // *xy5 - SUB Vx, Vy, Set vx = Vx - Vy, set VF = NOT borrow
			chip8->registers.V[0x0f] = chip8->registers.V[x] > chip8->registers.V[y] ? true: false;
			chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
		break;

		case 0x06: // 8xy^ - SHR Vx {,Vy}
			chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;	
			chip8->registers.V[x] /= 2;
		break;

		case 0x07: // 8xy7 - SUBN Vx, Vy
			chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
			chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
		break;

		case 0x0E: // BxyE - SHL Vx {, Vy}
			chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
			chip8->registers.V[x] = chip8->registers.V[x] * 2;
		break;

	}
}

static char chip8_wait_for_key_press(struct chip8* chip8)
{
	SDL_Event event;
	while(SDL_WaitEvent(&event))
	{
		if (event.type != SDL_KEYDOWN)
			continue;
		char c = event.key.keysym.sym;
		
	}

}

static void chip8_exec_extended_F(struct chip8* chip8, unsigned short opcode)
{
	unsigned char x = (opcode >> 8) & 0x000f;
	switch(opcode & 0x0ff)
	{
		case 0x07: // fx07 - LD Vx, DT, Set Vx to the delay timer value
			chip8->registers.V[x] = chip8->registers.delay_timer;
		break;	


	}

}

static void chip8_exec_extended(struct chip8* chip8, unsigned short opcode)
{
	unsigned short nnn = opcode & 0x0fff; 
	unsigned char x = (opcode >> 8) & 0x000f;
	unsigned char y = (opcode >> 4) & 0x000f;
	unsigned char kk = opcode & 0x00ff;
	unsigned char n = opcode & 0x000f;
 
	switch(opcode & 0xf000)
	{
		case 0x1000: // JP address, 1nnn Jump to location nnn
			chip8->registers.PC = nnn;	
		break;

		case 0x2000: // CALL address, 2nnn Call subroutine at location nnn
			chip8_stack_push(chip8, chip8->registers.PC);	
			chip8->registers.PC = nnn;
		break;

		case 0x3000: // SE Vx, byte - 3xkk Skip next instruction if Vx=kk
			if (chip8->registers.V[x] ==kk)
			{
				chip8->registers.PC += 2;
			}
		break;

		case 0x4000: // SNE Vx, byte - 3xkk Skip next instruction if Vx!=kk
			if (chip8->registers.V[x] !=kk)
			{
				chip8->registers.PC += 2;
			}
		break;

		case 0x5000: // 5xy0 - SE, Vx, Vy, skip next instruction if Vx=Vy
			if (chip8->registers.V[x] == chip8->registers.V[y])
			{
				chip8->registers.PC += 2;
			}
		break;

		case 0x6000: // 6xkk - LD Vx, byte,  Vx = kk
			chip8->registers.V[x] = kk;
		break;

		case 0x7000: // 7xkk - ADD Vx, byte. Set Vx = Vx + kk
			chip8->registers.V[x] += kk;
		break;

		case 0x8000:
			chip8_exec_extended_eight(chip8, opcode);
		break;

		case 0x9000: // 9xy0 - SNE Vx, Vy, Skip next instruction if Vx != Vy
			if (chip8->registers.V[x] != chip8->registers.V[y])
			{
				chip8->registers.PC += 2;
			}
		break;

		case 0xA000: // Annn - LD I, addr. Sets the I register to nnn
			chip8->registers.I = nnn;	
		break;

		case 0xB000: // Bnnn - Jump to location nnn + V0
			chip8->registers.PC = nnn + chip8->registers.V[0x00];
		break;

		case 0xC000: // Cxkk - RND Vx, byte
		 	srand(clock());
			chip8->registers.V[x] = (rand() % 255) & kk;
		break;
		// NOTE: Drawing upcodes

		case 0xD000: // Dxyn - DRW V, Vy, nibble, Draws sprites to the screen
		{
			const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
			chip8->registers.V[0x0f] = chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
		}
		break;
		
		// Keyboard operations
		case 0xE000:  
		{
			switch(opcode  0x00ff)
			{
				// Ex9e - SKP vx, Skp the next instruction if the key with the value of Vx is pressed

				case 0x9e:
					if (chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])
					{
						chip8->registers.PC += 2;
					}
				break;

				// Exa1 - SKNP Vx - Skip the next instruction if the key with the value of Vx is not pressed
				case 0xa1:
					if (!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]) 
					{
						chip8->registers.PC += 2;

					}
				break;
			}
		}

		break;

		case 0xF000:
			chip8_exec_extended_F(chip8, opcode);
		break;



	}

}

void chip8_exec(struct chip8* chip8, unsigned short opcode) // opcode is 2 bytes long 
{
	switch(opcode)
	{
		case 0x00E0: // CLS: clear the display opcode
			chip8_screen_clear(&chip8->screen);
		break;
		
		case 0x00EE: // RET: return from subrutine
			chip8->registers.PC = chip8_stack_pop(chip8);	
		break;


		default:
			chip8_exec_extended(chip8, opcode);

	}
}
