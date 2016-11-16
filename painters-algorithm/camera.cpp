#include "camera.h"
#include <fstream>
#include <math.h>
#include "matriz.h"
#include "parametros.h"
Camera::Camera(Parametros* pp)
{
	p = pp;
	Mtrans = p->Mtrans;
}

void Camera::mudaCoord(Ponto& pt)
{
	Matriz r(3,1), r2(3,1);
	Vetor v = pt - p->posicao;
	r2 = v.asMatriz();
	r = (*Mtrans) * r2;
	pt = r.asPonto();
}

Camera::~Camera()
{
}

void Camera::rotx(float s)
{
	Matriz t(3,3);
	t.setAt(0,0,1);
	t.setAt(0,1,0);
	t.setAt(0,2,0);
	t.setAt(1,0,0);
	t.setAt(1,1,cosf(s));
	t.setAt(1,2,-sinf(s));
	t.setAt(2,0,0);
	t.setAt(2,1,sinf(s));
	t.setAt(2,2,cosf(s));
	*Mtrans = (*Mtrans)*t; 
}

void Camera::roty(float s)
{
	Matriz t(3,3);
	t.setAt(0,0,cosf(s));
	t.setAt(0,1,0);
	t.setAt(0,2,sinf(s));
	
	t.setAt(1,0,0);
	t.setAt(1,1,1);
	t.setAt(1,2,0);
	t.setAt(2,0,-sinf(s));
	t.setAt(2,1,0);
	t.setAt(2,2,cosf(s));
	*Mtrans = (*Mtrans)*t; 
}

void Camera::rotz(float s)
{
	Matriz t(3,3);
	t.setAt(0,0,cosf(s));
	t.setAt(0,1,-sinf(s));
	t.setAt(0,2,0);
	
	t.setAt(1,0,sinf(s));
	t.setAt(1,1,cosf(s));
	t.setAt(1,2,0);
	t.setAt(2,0,0);
	t.setAt(2,1,0);
	t.setAt(2,2,1);
	*Mtrans = (*Mtrans)*t; 
}
