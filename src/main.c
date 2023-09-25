#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
	SDLK_0, SDLK_1, SDLK_2,  SDLK_3, SDLK_4, SDLK_5, 
	SDLK_6, SDLK_7, SDLK_8,  SDLK_9, SDLK_a, SDLK_b, 
	SDLK_c, SDLK_d, SDLK_e,  SDLK_f
};

int main(int argc, char** argv) 
{

	struct chip8 chip8;

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
			switch(event.type)
			{
				case SDL_QUIT:
					goto out;
				break;

				case SDL_KEYDOWN:
				{
					char key = event.key.keysym.sym;	
					int vkey = chip8_keyboard_map(keyboard_map, key);
					if (vkey != -1)
					{
						chip8_keyboard_down(&chip8.keyboard, vkey);
					}
				}
				break;

				case SDL_KEYUP:
				{
					char key = event.key.keysym.sym;	
					int vkey = chip8_keyboard_map(keyboard_map, key);
					if (vkey != -1)
					{
						chip8_keyboard_up(&chip8.keyboard, vkey);
					}
				}
				break;
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
