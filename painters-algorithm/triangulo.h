#ifndef __TRIANGULO_H__
#define __TRIANGULO_H__

#include "vetor.h"
#include "ponto.h"
#include "cor.h"
#include "primitives.h"
#include "cena.h"
class Triangulo {
	Ponto mBaricentro;
	Vetor mNormal;
	Vetor L;
	Vetor V;
	Vetor R;
	Cor cor;
	Ponto* vertices[3];
	Ponto projetados[3];
public:
	Triangulo(Ponto*, Ponto*, Ponto*);
	void calcBaricentro();
	Ponto baricentro() { return mBaricentro; }
	void calcNormal();
	Vetor normal() { return mNormal; }
	void calcLVR(Ponto&);
	void calcCor(Vetor&,Vetor&,Vetor&,Vetor&,float,float,float);
	void projeta(float,float,float);
	void blit();
	friend class Primitives; 	
};

#endif

