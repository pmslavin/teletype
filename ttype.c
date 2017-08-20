#include <stdlib.h>

#include "SDL2/SDL.h"

#define W 1024
#define H 768

#define _C(x) ((x-32)*8)

const int fps = 30;

SDL_Surface  *surface  = NULL;
SDL_Window   *window   = NULL;
SDL_Renderer *renderer = NULL;
//SDL_Texture  *texture  = NULL;

uint8_t charSet[] = {
#include "font.i"
};

uint8_t face[] = {60,90,219,255,129,66,60,0}; // Test glyph, 0x7F in font.i

void initialise_sdl(void)
{
	SDL_Init(SDL_INIT_VIDEO);

	window   = SDL_CreateWindow("Teletype", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	surface  = SDL_CreateRGBSurface(0, W, H, 8, 0,0,0,0);
//	texture  = SDL_CreateTextureFromSurface(renderer, surface);
//	texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_INDEX8, SDL_TEXTUREACCESS_STATIC, W, H);
//	surface  = SDL_CreateRGBSurfaceWithFormat(0, W, H, 8, SDL_PIXELFORMAT_INDEX8);

//	SDL_RenderSetLogicalSize(renderer, 640, 480);

	SDL_Color bg = {0x2e, 0x1d, 0x70, 0xFF};
	SDL_Color fg = {0xFF, 0xd2, 0x0a, 0xFF};
	surface->format->palette->colors[0x00] = bg;
	surface->format->palette->colors[0xFF] = fg;
}


void update(void)
{
	/* Meh. Pseudo indexed colour, fixed in 2.0.5? */
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
//	SDL_UpdateTexture(texture, NULL, surface->pixels, W*sizeof(uint8_t));
	SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
	SDL_DestroyTexture(tex);
//	SDL_Surface *s = SDL_GetWindowSurface(window);
//	SDL_BlitSurface(surface, NULL, s, NULL);
//	SDL_UpdateWindowSurface(window);
}


void renderChar(uint8_t *c, uint8_t *dest, int zoom)
{
	int x,y;
	uint8_t bits;

	SDL_LockSurface(surface);
	for(y=0; y<8; y++){
		bits = c[y>>(zoom?1:0)];
		for(x=0; x<8; x++){
			if(zoom==2)
				dest[y*W+2*x] = dest[y*W+2*x+1] = bits&128 ? 0xFF : 0x00;
			else
				dest[y*W+x] = bits&128 ? 0xFF : 0x00;
			bits <<= 1;
		}
	}

	if(zoom==1){
		for(y=0; y<8; y++){
			bits = c[(y+8)>>1];
			for(x=0; x<8; x++){
				dest[y*W+8*W+x] = bits&128 ? 0xFF : 0x00;
				bits <<= 1;
			}
		}
	}else if(zoom==2){
		for(y=0; y<8; y++){
			bits = c[4+(y>>1)];
			for(x=0; x<8; x++){
				dest[(y+8)*W+2*x] = dest[(y+8)*W+2*x+1] = bits&128 ? 0xFF : 0x00;
				bits <<= 1;
			}
		}
	}
	SDL_UnlockSurface(surface);
}


void demoChars(uint8_t *dest, int zoom)
{
	int lmargin = 48;
	int tmargin = 24*W;
	int rowstep = (24+4*zoom)*W;
	int colstep = 24+4*zoom;
	int cpr		= 24-2*zoom;

	for(size_t c=0; c<sizeof(charSet); c+=8){
		renderChar(&charSet[c], (uint8_t*)dest+(tmargin+(c/8)/cpr*rowstep+lmargin+((c/8)%cpr)*colstep), zoom);
	}
}


void writeLine(uint8_t *dest, char *s, int zoom)
{
	int offset = 0;
	while(*s){
		renderChar(&charSet[_C(*s++)], dest+offset, zoom);
		offset += 8<<(zoom>>1);
	}
}


void teletype(uint8_t *dest, char *s, int zoom, int p, int fuzz)
{
	uint8_t c;
	int i  = 0, offset = 0;
	while(*s){
		c = *s;
		if(i>=p){
			renderChar(&charSet[_C(' ')], dest+offset, zoom);
			break;
		}
		if(i+fuzz>=p)
			c ^= rand()&rand()&127;
		renderChar(&charSet[_C(c)], dest+offset, zoom);
		offset += 8<<(zoom>>1);
		i++;
		s++;
	}
	update();
}


void testCard(void)
{
	demoChars((uint8_t*)surface->pixels, 0);
	demoChars((uint8_t*)surface->pixels+200*W, 1);
	demoChars((uint8_t*)surface->pixels+400*W, 2);
	writeLine((uint8_t*)surface->pixels+(H-120)*W+32, "Here is the text to be written by writeLine()\x7f", 0);
	writeLine((uint8_t*)surface->pixels+(H-84)*W+32, "Here is the text to be written by writeLine()\x7f", 1);
	writeLine((uint8_t*)surface->pixels+(H-48)*W+32, "Here is the text to be written by writeLine()\x7f", 2);
	update();
}


int main(void)
{
	static unsigned int t = 0;
	static unsigned int p = 0;
	initialise_sdl();
	update();

	SDL_Event e;
	for(;;){
		SDL_PollEvent(&e);
		if(e.type == SDL_KEYDOWN)
			if(e.key.keysym.sym == SDLK_q)
				break;

		if(t<14*fps){
			teletype((uint8_t*)surface->pixels+40*W+40, "This is the first message to be displayed by the teletype writer.", 0, p, 4);
			teletype((uint8_t*)surface->pixels+60*W+40, "Here is another line rendered in a larger size.", 1, p, 4);
			teletype((uint8_t*)surface->pixels+88*W+40, "\x7f \x80 Quad mode enabled! \x80 \x7f", 2, p, 4);
			teletype((uint8_t*)surface->pixels+112*W+40, "Another line of text written in the largest size \x81\x82", 2, p, 4);
			if(t>5*fps) p--; else p++;
		}

		SDL_Delay(1000.0/fps);
		t++;
	}

	SDL_Quit();
	return 0;
}
