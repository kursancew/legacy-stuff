#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "matriz.h"
#include "vetor.h"
#include "ponto.h"

class Parametros;

class Camera {
	Matriz* Mtrans;
	Parametros* p;
public:
	Camera(Parametros*);
	~Camera();
	void mudaCoord(Ponto&);
	void rotx(float);
	void roty(float);
	void rotz(float);
};

#endif

