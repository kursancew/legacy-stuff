#ifndef __CENA_H__
#define __CENA_H__

class Objeto3d;
class Vetor;
class Parametros;
#include "camera.h"
#include "ponto.h"

class Cena {
public:
	Objeto3d* mObjeto;
	Camera* mCamera;
	Parametros *p;
	Ponto mLuz;
	Cena(Objeto3d*, Camera*, Parametros*);
	void draw();
};

#endif

