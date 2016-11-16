#include "primitives.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <math.h>
#include <iostream>
#include "config.h"
static 	SDL_Surface *tela = 0;

Primitives::Primitives()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD);
	tela = SDL_SetVideoMode(RES_X,RES_Y,24,SDL_ANYFORMAT|SDL_ASYNCBLIT|SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	
}

void Primitives::flip()
{
	::SDL_Flip(tela);
}

void Primitives::linha(const float x0, const float y0, const float x1, const float y1, Cor& c)
{
	
}

void Primitives::write(int x, int y, const char* s)
{
	::stringRGBA(tela, x, y, s, 255, 255, 255, 255);
}

void Primitives::clean()
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = RES_X;
	r.h = RES_Y;
	::SDL_FillRect(tela, &r, 0);
}

void Primitives::ponto(const float x, const float y, Cor &c)
{
	if(x >= RES_X || y >= RES_Y || x < 0 || y < 0) {
		std::cerr << "bad plot" << std::endl;
		return;
	}
	((unsigned int*)(tela->pixels))[ RES_X*(int)y + (int)x ] = c.asInt();
}

void Primitives::fill(const float x, const float y, Cor &c)
{
}

void Primitives::triangulo(Triangulo& t)
{
	if(!tela) {
		std::cout << "null pointer" << std::endl;
		exit(1);
	}
	filledTrigonRGBA
	//trigonRGBA
	(tela, 	(int)t.projetados[0].x(), (int)t.projetados[0].y(),
							(int)t.projetados[1].x(), (int)t.projetados[1].y(),
							(int)t.projetados[2].x(), (int)t.projetados[2].y(),
							t.cor.r(), t.cor.g(), t.cor.b(), 255);
							//255,255,255,255);
}
