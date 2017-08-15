#include <stdlib.h>

#include "SDL2/SDL.h"

#define W 1024
#define H 768

SDL_Surface  *surface  = NULL;
SDL_Window   *window   = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture  *texture  = NULL;

uint8_t pixels[W/8*H];

void initialise_sdl(void)
{
	SDL_Init(SDL_INIT_VIDEO);

	window   = SDL_CreateWindow("Teletype", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, W, H);

	surface  = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);

	SDL_UpdateTexture(texture, NULL, surface->pixels, W*sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}


void update(void)
{
	SDL_UpdateTexture(texture, NULL, surface->pixels, W*sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}


void dd(uint32_t *pixels)
{
	int x = W/2-8;
	int y = H/2-8;

	for(int r=0; r<8; r++){
		pixels[(y+r)*W+x]   = 0xFFFFFFFF;
		pixels[(y+r)*W+x+1] = 0x00000000;
		pixels[(y+r)*W+x+2] = 0xFFFFFFFF;
		pixels[(y+r)*W+x+3] = 0x00000000;
		pixels[(y+r)*W+x+4] = 0xFFFFFFFF;
		pixels[(y+r)*W+x+5] = 0x00000000;
		pixels[(y+r)*W+x+6] = 0x00000000;
		pixels[(y+r)*W+x+7] = 0xFF000000;
	}
	update();
}


int main(void)
{
	initialise_sdl();
	dd(surface->pixels);
	SDL_Delay(4000);

	return 0;
}
