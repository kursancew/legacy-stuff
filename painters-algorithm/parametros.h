#ifndef __PARAMETROS_H__
#define __PARAMETROS_H__

#include "vetor.h"
#include "ponto.h"
#include "matriz.h"

class Parametros {
public:
	float atual[3];
	char nomepar[20];
	Ponto luz;
	Vetor I_a;
	Vetor I_l;
	Vetor P_l;
	
	Vetor K_d;
	Vetor O_d;
	float K_a;
	float K_s;
	float eta;
	
	Ponto posicao;
	Vetor N,U,V;
	Vetor pN,pV;
	float H_x;
	float H_y;
	float D_plano;
	
	Matriz* Mtrans;
	
	Parametros();
	void setPos(float*);
	void setLuz(float*);
	void setIa(float*);
	void setIl(float*);
	void setKd(float*);
	void setOd(float*);
	void setKa(float*);
	void setKs(float*);
	void setEta(float*);
	void setN(float*);
	void setV(float*);
	void setD(float*);
	void setHx(float*);
	void setHy(float*);
};

#endif
