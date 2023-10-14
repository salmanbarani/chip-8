#include <stdio.h>
#include <stdbool.h>
// If running in Windows
#ifdef _WIN32
#include <Windows.h>
#endif
#include <unistd.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
	SDLK_0, SDLK_1, SDLK_2,  SDLK_3, SDLK_4, SDLK_5, 
	SDLK_6, SDLK_7, SDLK_8,  SDLK_9, SDLK_a, SDLK_b, 
	SDLK_c, SDLK_d, SDLK_e,  SDLK_f
};

void beep(int frequency, int duration) {
    printf("\033[10;%d]%c", frequency, 7);  // 7 is the ASCII code for the Bell character
    fflush(stdout);
    usleep(duration * 1000);  // usleep is used to sleep for a specified number of microseconds
    printf("\033[10;0]");  // Reset the terminal to default settings
    fflush(stdout);
}

int main(int argc, char** argv) 
{

	struct chip8 chip8;
	chip8_init(&chip8);
	chip8.registers.sound_timer= 255;	

	chip8_screen_draw_sprite(&chip8.screen, 5 , 0, &chip8.memory.memory[0x00], 5);

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
			};
		}
	
		SDL_SetRenderDrawColor(renderer, 0,0,0,0); // all render functions rely on this color
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255,0);

		for (int x = 0; x < CHIP8_WIDTH; x++)
		{
			for (int y = 0; y < CHIP8_HEIGHT; y++)
			{
				if (chip8_screen_is_set(&chip8.screen, x, y))
				{
					SDL_Rect r;
					r.x = x * CHIP8_WINDOW_MULTIPLIER;
					r.y = y * CHIP8_WINDOW_MULTIPLIER;
					r.w = CHIP8_WINDOW_MULTIPLIER;
					r.h = CHIP8_WINDOW_MULTIPLIER;
					SDL_RenderFillRect(renderer, &r);
				}
			}
		}
		SDL_RenderPresent(renderer);
		

		if (chip8.registers.delay_timer > 0)
		{
			printf("Delay!!!\n");
			sleep(0.1); // sleep for 100 ms
			chip8.registers.delay_timer -= 1;
		}
		
		if (chip8.registers.sound_timer > 0)
		{
			// Sound
			beep(8000, 100 ); // TODO: replace it with Beep(frequency, duration) in Windows
			chip8.registers.sound_timer -= 1;
		}

	}
	out:
		SDL_DestroyWindow(window); 
		return 0;
}



