#include "cena.h"
#include "objeto3d.h"

#include <cstdio>

int main(int argc, int **argv)
{
	Objeto3d forma = Objeto3d("obj.3d");
	Camera cam = Camera();
	Cena cena = Cena(&forma, &cam);
	cena.draw();
	getchar();
}
