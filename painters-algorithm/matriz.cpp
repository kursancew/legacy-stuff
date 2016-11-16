#include "matriz.h"

Matriz::Matriz(int i, int j) : lin(i),col(j) {
//	mMatriz = new float[ i*sizeof(float)*j*sizeof(float) ];
//	if (!mMatriz) {
//		throw eMatriz("erro: Matriz::Matriz(): memoria insuficiente");
//	}
	mMatriz = std::vector<float>(i*j, 0.0);
}

float Matriz::at(int i, int j)
{
	if (i < lin && j < col)
		return mMatriz[i*col+j];
	throw eMatriz("erro: Matriz::at(): indice invalido");
}

void Matriz::setAt(int i, int j, float v)
{
	if (i < lin && j < col)
		mMatriz[i*col+j] = v;
	else 
		throw eMatriz("erro: Matriz::setAt(): indice invalido");
}

std::string Matriz::asString()
{
	char *l = new char[col * 60];
	if(!l) {
		throw eMatriz("erro: Matriz::asString(): memoria insuficiente");
	}
	std::string r;
	//sprintf(l,"%d %d\n",lin,col);
	//r = l;
	for ( int i = 0; i < lin; ++i) {
		for(int j = 0; j < col; ++j) {
			if(j)
				sprintf(l," %f",at(i,j));
			else
				sprintf(l,"%f",at(i,j));
			r += l;
		}
		r += "\n";
	}
	delete l;
	return r;
}

Matriz::~Matriz()
{
	//if(mMatriz) delete mMatriz;
	//mMatriz = 0;
}

Ponto Matriz::asPonto()
{
	if(lin != 3 || col != 1) {
		throw eMatriz("erro: Matriz::asPonto(): matriz nao representa ponto");
	}
	return Ponto(at(0, 0), at(1, 0), at(2, 0));
}

