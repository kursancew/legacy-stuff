#include "parametros.h"
#include <fstream>
#include "config.h"

Parametros::Parametros()
{
	std::ifstream IN(CENA_CONF, std::ios::in);
	float tx, ty, tz;
	IN >> tx >> ty >> tz;
	luz = Ponto(tx, ty, tz);
	IN >> tx >> ty >> tz;
	I_a = Vetor(tx, ty, tz);
	IN >> tx >> ty >> tz;
	I_l = Vetor(tx, ty, tz);
	IN >> tx >> ty >> tz;
	P_l = Vetor(tx, ty, tz);
	IN.close();

	std::ifstream CONF(OBJ_CONF, std::ios::in);
	CONF >> tx >> ty >> tz;
	K_d = Vetor(tx, ty, tz);
	CONF >> tx >> ty >> tz;
	O_d = Vetor(tx, ty, tz);
	CONF >> K_a;
	CONF >> K_s;
	CONF >> eta;
	CONF.close();
	
	IN.open(CAM_CONF, std::ios::in);
	IN >> tx >> ty >> tz;
	posicao = Ponto(tx, ty, tz);
	
	IN >> tx >> ty >> tz;
	pN = Vetor(tx, ty, tz);
	
	IN >> tx >> ty >> tz;
	pV = Vetor(tx, ty, tz);
	V = pV;
	N = pN;
	N.orto(V);
	U = N.pv(V);
	N.normalizar();
	V.normalizar();
	U.normalizar();
	
	Mtrans = new Matriz(3,3);
	Mtrans->setAt(2, 0, N.x());
	Mtrans->setAt(2, 1, N.y());
	Mtrans->setAt(2, 2, N.z());
	Mtrans->setAt(1, 0, V.x());
	Mtrans->setAt(1, 1, V.y());
	Mtrans->setAt(1, 2, V.z());
	Mtrans->setAt(0, 0, U.x());
	Mtrans->setAt(0, 1, U.y());
	Mtrans->setAt(0, 2, U.z());
	
	IN >> H_x >> H_y >> D_plano;
	IN.close();
}

void Parametros::setPos(float* f)
{
	posicao.x() += f[0];
	posicao.y() += f[1];
	posicao.z() += f[2];
	atual[0] = posicao.x();
	atual[1] = posicao.y();
	atual[2] = posicao.z();
	sprintf(nomepar, "Camera");
}

void Parametros::setLuz(float* f)
{
	luz.x() += f[0];
	luz.y() += f[1];
	luz.z() += f[2];
	atual[0] = luz.x();
	atual[1] = luz.y();
	atual[2] = luz.z();
	sprintf(nomepar, "Luz");
}

void Parametros::setIl(float *f)
{
	I_l.x() += f[0];
	I_l.y() += f[1];
	I_l.z() += f[2];
	if(I_l.x() > 255) I_l.x() = 255;
	if(I_l.y() > 255) I_l.y() = 255;
	if(I_l.z() > 255) I_l.z() = 255;
	if(I_l.x() < 0) I_l.x() = 0;
	if(I_l.y() < 0) I_l.y() = 0;
	if(I_l.z() < 0) I_l.z() = 0;
	atual[0] = I_l.x();
	atual[1] = I_l.y();
	atual[2] = I_l.z();
	sprintf(nomepar, "I_l");
}

void Parametros::setIa(float *f)
{
	I_a.x() += f[0];
	I_a.y() += f[1];
	I_a.z() += f[2];
	if(I_a.x() > 255) I_a.x() = 255;
	if(I_a.y() > 255) I_a.y() = 255;
	if(I_a.z() > 255) I_a.z() = 255;
	if(I_a.x() < 0) I_a.x() = 0;
	if(I_a.y() < 0) I_a.y() = 0;
	if(I_a.z() < 0) I_a.z() = 0;
	atual[0] = I_a.x();
	atual[1] = I_a.y();
	atual[2] = I_a.z();
	sprintf(nomepar, "I_a");
}

void Parametros::setKd(float *f)
{
	K_d.x() += f[0];
	K_d.y() += f[1];
	K_d.z() += f[2];
	if(K_d.x() < 0) K_d.x() = 0;
	if(K_d.y() < 0) K_d.y() = 0;
	if(K_d.z() < 0) K_d.z() = 0;
	if(K_d.x() > 1) K_d.x() = 1;
	if(K_d.y() > 1) K_d.y() = 1;
	if(K_d.z() > 1) K_d.z() = 1;
	atual[0] = K_d.x();
	atual[1] = K_d.y();
	atual[2] = K_d.z();
	sprintf(nomepar, "K_d");
}

void Parametros::setOd(float *f)
{
	O_d.x() += f[0];
	O_d.y() += f[1];
	O_d.z() += f[2];
	if(O_d.x() < 0) O_d.x() = 0;
	if(O_d.y() < 0) O_d.y() = 0;
	if(O_d.z() < 0) O_d.z() = 0;
	if(O_d.x() > 1) O_d.x() = 1;
	if(O_d.y() > 1) O_d.y() = 1;
	if(O_d.z() > 1) O_d.z() = 1;
	atual[0] = O_d.x();
	atual[1] = O_d.y();
	atual[2] = O_d.z();
	sprintf(nomepar, "O_d");
}

void Parametros::setKa(float *f)
{
	K_a += f[0];
	if(K_a < 0) K_a = 0;
	if(K_a > 1) K_a = 1;
	atual[0] = K_a;
	atual[1] = 0;
	atual[2] = 0;
	sprintf(nomepar, "K_a");
}

void Parametros::setKs(float *f)
{
	K_s += f[0];
	if(K_s < 0) K_s = 0;
	if(K_s > 1) K_s = 1;
	atual[0] = K_s;
	atual[1] = 0;
	atual[2] = 0;
	sprintf(nomepar, "K_s");
}

void Parametros::setEta(float *f)
{
	eta += f[0];
	if(eta < 0) eta = 0;
	atual[0] = eta;
	atual[1] = 0;
	atual[2] = 0;
	sprintf(nomepar, "eta");
}

void Parametros::setN(float *f)
{
	pN.x() += f[0];
	pN.y() += f[1];
	pN.z() += f[2];
	V = pV;
	N = pN;
	N.orto(V);
	U = N.pv(V);
	N.normalizar();
	V.normalizar();
	U.normalizar();
	atual[0] = pN.x();
	atual[1] = pN.y();
	atual[2] = pN.z();
	sprintf(nomepar, "N");
}

void Parametros::setV(float *f)
{
	pV.x() += f[0];
	pV.y() += f[1];
	pV.z() += f[2];
	V = pV;
	N = pN;
	N.orto(V);
	U = N.pv(V);
	N.normalizar();
	V.normalizar();
	U.normalizar();
	atual[0] = pV.x();
	atual[1] = pV.y();
	atual[2] = pV.z();
	sprintf(nomepar, "V");
}

void Parametros::setD(float *f)
{
	D_plano += f[0];
	if(D_plano < 0) D_plano = 0;
	atual[0] = D_plano;
	atual[1] = 0;
	atual[2] = 0;
	sprintf(nomepar, "D_plano");
}

void Parametros::setHx(float *f)
{
	H_x += f[0];
	if(H_x < 0) H_x = 0;
	atual[0] = H_x;
	atual[1] = 0;
	atual[2] = 0;
	sprintf(nomepar, "H_x");
}

void Parametros::setHy(float *f)
{
	H_y += f[0];
	if(H_y < 0) H_y = 0;
	atual[0] = H_y;
	atual[1] = 0;
	atual[2] = 0;
	sprintf(nomepar, "H_y");
}
