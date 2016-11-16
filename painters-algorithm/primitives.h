#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include <SDL/SDL.h>
#include <string>
#include "cor.h"
#include "triangulo.h"
class Triangulo;
class Primitives {

public:
	Primitives();
	static void linha(const float, const float, const float, const float, Cor&);
	static void ponto(const float, const float, Cor&);
	static void fill(const float, const float, Cor&);
	static void texto(std::string&, const float, const float, Cor&);
	static void triangulo(Triangulo&);
	static void flip();
	static void clean();
	static void write(int, int, const char*);
};

#endif

