#ifndef __VETOR_H__
#define __VETOR_H__

#include <vector>
#include <iostream>
#include <cstdlib>

class Matriz;
class Vetor {
	float componentes[3];
public:
	Vetor();
	Vetor(float, float, float);

	void normalizar();
	void orto(Vetor&);
	void orto(Vetor&, Vetor&);
	float pi(Vetor&);
	Vetor pv(Vetor&);
	
	float modulo();

	float& x() { return componentes[0]; }
	float& y() { return componentes[1]; }
	float& z() { return componentes[2]; }

	Matriz asMatriz();
	
	//OPERADORES
	float& operator[](int i) {
		return componentes[i];
	}

	void print() {
		std::cout << "( " << x() << ", " << y() << ", " << z() << ") " << std::endl;
	}

	Vetor operator+(Vetor b) {
		Vetor r;
		for(int i = 0; i < 3; ++i) 
			r[i] = componentes[i] + b[i];
		return r;
	}
	Vetor operator-(Vetor b) {
		Vetor r;
		for(int i = 0; i < 3; ++i) 
			r[i] = componentes[i] - b[i];
		return r;
	}
	Vetor operator*(float f) {
		Vetor r(f*componentes[0], f*componentes[1], f*componentes[2]);
		return r;
	}
	Vetor operator*(Vetor b) {
		Vetor r(b.x()*componentes[0], b.y()*componentes[1], b.z()*componentes[2]);
		return r;
	}
};

#endif

