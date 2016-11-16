#include "vetor.h"

#include <math.h>
#include <iostream>
#include "matriz.h"
Vetor::Vetor()
{
	componentes[0] = 0.0;
	componentes[1] = 0.0;
	componentes[2] = 0.0;
}

Vetor::Vetor(float x, float y, float z)
{
	componentes[0] = x;
	componentes[1] = y;
	componentes[2] = z;
}

Matriz Vetor::asMatriz()
{
	Matriz m(3,1);
	m.setAt(0, 0, componentes[0]);
	m.setAt(1, 0, componentes[1]);
	m.setAt(2, 0, componentes[2]);
	return m;
}

void Vetor::normalizar()
{
	float n = this->modulo();
	for(int i = 0; i < 3; ++i) {
		componentes[i] = (componentes[i]/n);
	}
}

Vetor Vetor::pv(Vetor& v2) {
	Vetor& v1 = *this;
	return Vetor(v1[1]*v2[2]-v1[2]*v2[1], v1[2]*v2[0]-v1[0]*v2[2], v1[0]*v2[1]-v1[1]*v2[0]);
}

void Vetor::orto(Vetor& v2)
{
	Vetor& v1 = *this;
	float k = v1.pi(v2)/v1.pi(v1);
	v2 = v2 - (v1*k);
}
void Vetor::orto(Vetor& v3, Vetor& v2)
{
	Vetor& v1 = *this;
	v1.orto(v2);
	float k1 = v1.pi(v3)/v1.pi(v1);
	float k2 = v2.pi(v3)/v2.pi(v2);

	v3 = v3 - (v2*k2) - (v1*k1);
}

float Vetor::pi(Vetor& v2)
{
	float r = 0;
	for(int i = 0; i < 3; ++i) {
		r += this->componentes[i] * v2[i];
	}
	return r;
}

float Vetor::modulo()
{
	float s = 0;
	for(int i = 0; i < 3; ++i) {
		s += componentes[i]*componentes[i];
	}
	return sqrt(s);
}
