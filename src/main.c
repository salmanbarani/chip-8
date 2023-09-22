#include <stdio.h>
#include "SDL2/SDL.h"
#include "chip8.h"

int main(int argc, char** argv) 
{

	struct chip8 chip8;

	chip8.registers.SP = 0;
	
	chip8_stack_push(&chip8, 0xff);
	chip8_stack_push(&chip8, 0xaa);
	
	printf("%x\n", chip8_stack_pop(&chip8));
	printf("%x\n", chip8_stack_pop(&chip8));

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(
		EMULATOR_WINDOW_TITLE,		// Title of the window
		SDL_WINDOWPOS_UNDEFINED,	// X position of the window 
		SDL_WINDOWPOS_UNDEFINED,	// Y position of the window
		CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,	// Width of the window 
		CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,	// Heights of the window
		SDL_WINDOW_SHOWN		// for the flag
	);
	

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

	while (1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				goto out;
			}
		}


		SDL_SetRenderDrawColor(renderer, 0,0,0,0); // all render functions rely on this color
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255,0);
		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = 40;
		r.h = 40;
		SDL_RenderFillRect(renderer, &r);
		SDL_RenderPresent(renderer);
	}
	out:
		SDL_DestroyWindow(window); 
		return 0;
}
