#include "triangulo.h"
#include "vetor.h"
#include "cor.h"
#include "ponto.h"
#include "primitives.h"
#include "config.h"
#include <cmath>

Triangulo::Triangulo(Ponto* a, Ponto* b, Ponto* c)
{
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;
}

void Triangulo::calcNormal()
{
	Vetor v1 = *(vertices[1]) - *(vertices[0]);
	Vetor v2 = *(vertices[2]) - *(vertices[0]);
	mNormal = v1.pv(v2);
	mNormal = mNormal;
	mNormal.normalizar();
}

void Triangulo::calcBaricentro()
{
	mBaricentro = vertices[0]->somabar(*(vertices[1]), *(vertices[2]), 1.0/3, 1.0/3, 1.0/3);
}

void Triangulo::calcLVR(Ponto& luz)
{
	L = luz - mBaricentro;
	V = Vetor(mBaricentro.x(), mBaricentro.y(), mBaricentro.z()) * (-1);
	V.normalizar();
	if(mNormal.pi(V) < 0) {
		mNormal = mNormal*(-1);
	}
	Vetor tmp = mNormal*(2*mNormal.pi(L));
	R = tmp - L;
	L.normalizar();
	R.normalizar();
	
}

void Triangulo::projeta(float d, float hx, float hy)
{
	for(int i = 0; i < 3; ++i) {
		projetados[i].x() = floor((RES_X/2)*( vertices[i]->x() * (d+vertices[i]->z()) )/( vertices[i]->z() * hx )+400);
		projetados[i].y() = floor((RES_Y/2)*( vertices[i]->y() * (d+vertices[i]->z()) )/( vertices[i]->z() * hy )+300);
		//std::cout << std::endl << projetados[i].x() << ", " << projetados[i].y();
	}

}

void Triangulo::blit()
{
	if(mBaricentro.z() >= 0)
		Primitives::triangulo(*this);
}

void Triangulo::calcCor(Vetor& ia, Vetor& kd, Vetor& od, Vetor& il, float eta, float ka, float ks)
{
	Vetor am = ia*ka;
	Vetor N = mNormal;

	float pinl = L.pi(N);
	
	Vetor di;
	Vetor sp;
	if(pinl >= 0) {
		di = od * il * (kd * pinl);
		float pivr = V.pi(R);
		if(pivr >= 0)
			sp = il * (ks * pow(pivr, eta));
	}
	Vetor c = di;
	c = c + sp;
	c = c + am;
	cor.r() = (int)c.x() > 255?255:(int)c.x();
	cor.g() = (int)c.y() > 255?255:(int)c.y();
	cor.b() = (int)c.z() > 255?255:(int)c.z();
}
