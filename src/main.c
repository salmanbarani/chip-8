#include <stdio.h>
#include "SDL2/SDL.h"


int main(int argc, char** argv) 
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(
		"Chip8 Window",  		// Title of the window
		SDL_WINDOWPOS_UNDEFINED,	// X position of the window 
		SDL_WINDOWPOS_UNDEFINED,	// Y position of the window
		640 , 320,			// Width and Heights of the window
		SDL_WINDOW_SHOWN		// for the flag
	);
	
	printf("Hello world!\n");
	SDL_DestroyWindow(window);
	return 0;
}
