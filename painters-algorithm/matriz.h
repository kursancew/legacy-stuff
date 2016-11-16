#ifndef __MATRIZ_H__
#define __MATRIZ_H__

#include <iostream>
#include <cstdlib>
#include "ponto.h"

class eMatriz
{
	std::string err;
	public:
		eMatriz(std::string e):err(e) {}
		std::string what() {
			return err;
		}
};

class Matriz {
public:
	Matriz(int, int);
	Matriz(std::ifstream&);
	~Matriz();
	
	Matriz operator*(Matriz& m) {
		if ( (*this).col != m.getLines() ) {
			return Matriz(0,0);
		}
		Matriz matResultado((*this).lin,m.getCols());
		for (int i = 0; i < matResultado.getLines(); ++i) {
			for (int j = 0; j < matResultado.getCols(); ++j) {
				matResultado.setAt(i,j,0);
				for (int w = 0; w < (*this).col; ++w)
					matResultado.setAt(i,j,matResultado.at(i,j)+ (*this).at(i,w) * m.at(w,j) );
			}
		}
		return matResultado;
	}
	float at(int, int);
	void setAt(int,int,float);
	int getLines() { return lin; }
	int getCols() { return col; }
	std::string asString();
	bool bad() { return (!col || !lin); }
	void free() { lin = col = 0; }
	Ponto asPonto();
private:
	/* cuidado com este ponteiro, pois quando eh feita uma copia do objeto o conteudo 
	do ponteiro nao eh copiado, apenas referenciado, por isso nao de pode liberar o 
	ponteiro no ~Matriz. Teria que se fazer um overload do operator= pra permitir isto */
	std::vector<float> mMatriz;
	int lin;
	int col;
};

#endif

