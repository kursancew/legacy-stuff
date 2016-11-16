#ifndef __OBJETO3D_H__
#define __OBJETO3D_H__


#include <vector>
#include "triangulo.h"
class Cena;
class Ponto;
class Camera;
class Vetor;
class Parametros;
class Objeto3d {
public:
	std::vector<Triangulo*> poligonos;
	std::vector<Ponto*> pontos;
	Parametros *p;

	Objeto3d(std::string, Parametros*);
	void mudaCoord(Camera&);
	void blit(Cena&);
};

#endif
