#include "cena.h"
#include <fstream>
#include "objeto3d.h"
#include "camera.h"
#include "parametros.h"
Cena::Cena(Objeto3d* obj, Camera* cam, Parametros *pp)
{
	mCamera = cam;
	mObjeto = obj;
	p = pp;
	mLuz = p->luz;
}

void Cena::draw()
{
	mObjeto->mudaCoord(*mCamera);
	mCamera->mudaCoord(mLuz);
	mObjeto->blit(*this);
}
