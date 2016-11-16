#ifndef __PONTO_H__
#define __PONTO_H__

#include "vetor.h"

class Ponto {
	float coordenadas[3];
public:
	Ponto();
	Ponto(float, float, float);

	float& x() { return coordenadas[0]; }
	float& y() { return coordenadas[1]; }
	float& z() { return coordenadas[2]; }

	Ponto somabar(Ponto& P2, float alpha, float beta);
	Ponto somabar(Ponto& P2, Ponto& P3, float alpha, float beta, float gamma);
	void print() {
		std::cout << "( " << x() << ", " << y() << ", " << z() << ") " << std::endl;
	}
	
	Vetor operator-(Ponto& b) {
		Ponto &a = *this;
		return Vetor(a.x() - b.x(), a.y() - b.y(), a.z() - b.z()); 
	}
};

#endif
