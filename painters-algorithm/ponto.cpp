#include "ponto.h"

Ponto::Ponto()
{
	coordenadas[0] = coordenadas[1] = coordenadas[2] = 0.0;
}

Ponto::Ponto(float x, float y, float z)
{
	coordenadas[0] = x;
	coordenadas[1] = y;
	coordenadas[2] = z;
}

Ponto Ponto::somabar(Ponto& P2, float alpha, float beta)
{
	if(alpha + beta > 1.00001 || alpha+beta < 0.99999) {
		std::cerr << "soma baricentrica invalida: " << alpha+beta << std::endl;
		exit(1);
	}
	Ponto& P1 = *this;
	float bx,by,bz;
	bx = alpha*P1.x() + beta*P2.x();
	by = alpha*P1.y() + beta*P2.y();
	bz = alpha*P1.z() + beta*P2.z();
	return Ponto(bx, by, bz);
}

Ponto Ponto::somabar(Ponto& P2, Ponto& P3, float alpha, float beta, float gamma)
{
	if(alpha + beta + gamma > 1.00001 || alpha + beta + gamma < 0.99999) {
		std::cerr << "soma baricentrica invalida: " << alpha+beta+gamma << std::endl;
		exit(1);
	}
	Ponto& P1 = *this;
	float bx,by,bz;
	bx = alpha*P1.x() + beta*P2.x() + gamma*P3.x();
	by = alpha*P1.y() + beta*P2.y() + gamma*P3.y();
	bz = alpha*P1.z() + beta*P2.z() + gamma*P3.z();
	return Ponto(bx, by, bz);
}
