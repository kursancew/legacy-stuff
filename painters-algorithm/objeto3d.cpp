#include "objeto3d.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "matriz.h"
#include "cena.h"
#include "triangulo.h"
#include "camera.h"
#include "primitives.h"
#include "parametros.h"

Objeto3d::Objeto3d(std::string f, Parametros *pp)
{
	p = pp;
	std::ifstream IN(f.c_str(), std::ios::in);
	int nPontos;
	int nTriangulos;
	IN >> nPontos;
	IN >> nTriangulos;
	for (int i = 0; i < nPontos; ++i) {
		float x,y,z;
		IN >> x >> y >> z;
		pontos.push_back(new Ponto(x,y,z));
	}
	for (int i = 0; i < nTriangulos; ++i) {
		int a,b,c;
		IN >> a >> b >> c;
		poligonos.push_back(new Triangulo(pontos[a-1], pontos[b-1], pontos[c-1]) );
	}
	
	IN.close();
}

bool compz(Triangulo* a, Triangulo* b)
{
	return a->baricentro().z() < b->baricentro().z();
}

void Objeto3d::mudaCoord(Camera &cam)
{
	for(unsigned int i = 0; i < pontos.size(); ++i) {
		cam.mudaCoord(*pontos[i]);
	}
	
	for(unsigned int i = 0; i < poligonos.size(); ++i) {
		poligonos[i]->calcNormal();
		poligonos[i]->calcBaricentro();
	}
	sort(poligonos.begin(), poligonos.end(), compz);
}

void Objeto3d::blit(Cena& cena)
{
	for(unsigned int i = 0; i < poligonos.size(); ++i) {
		poligonos[i]->projeta(p->D_plano, p->H_x, p->H_y);
		poligonos[i]->calcLVR(cena.mLuz);
		poligonos[i]->calcCor(p->I_a, p->K_d, p->O_d, p->I_l, p->eta, p->K_a, p->K_s);
		poligonos[i]->blit();
	}
}
