//2009 Kursancew, Vinicius Alexandre
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <malloc.h>
#include <fstream>
#include <exception>

#define self (*this)

class eMatrix
{
	std::string err;
	public:
		eMatrix(std::string e):err(e) {}
		std::string what() {
			return err;
		}
};

class Matrix {
public:
	Matrix(int, int);
	Matrix(std::ifstream&);
	~Matrix();
	
	Matrix operator*(Matrix& m) {
		if ( self.col != m.getLines() ) {
			return Matrix(0,0);
		}
		Matrix matResultado(self.lin,m.getCols());
		for (int i = 0; i < matResultado.getLines(); ++i) {
			for (int j = 0; j < matResultado.getCols(); ++j) {
				matResultado.setAt(i,j,0);
				for (int w = 0; w < self.col; ++w)
					matResultado.setAt(i,j,matResultado.at(i,j)+ self.at(i,w) * m.at(w,j) );
				}
		}
		return matResultado;
	}
	int at(int, int);
	void setAt(int,int,int);
	int getLines() { return lin; }
	int getCols() { return col; }
	std::string asString();
	bool bad() { return (!col || !lin); }
	void free() { delete mMatrix; mMatrix = 0; lin = col = 0; }
private:
/* cuidado com este ponteiro, pois quando eh feita uma copia do objeto o conteudo 
do ponteiro nao eh copiado, apenas referenciado, por isso nao de pode liberar o 
ponteiro no ~Matrix. Teria que se fazer um overload do operator= pra permitir isto */
	int* mMatrix;
	int lin;
	int col;
};


Matrix::Matrix(int i, int j) : lin(i),col(j) {
	mMatrix = new int[ i*sizeof(int)*j*sizeof(int) ];
	if (!mMatrix) {
		throw eMatrix("erro: Matrix::Matrix(): memoria insuficiente");
	}
}

int Matrix::at(int i, int j)
{
	if (i < self.lin && j < self.col)
		return *(mMatrix+i*self.col+j  );
	throw eMatrix("erro: Matrix::at(): indice invalido");
}

void Matrix::setAt(int i, int j, int v)
{
	if (i < self.lin && j < self.col)
		*(mMatrix+i*self.col+j) = v;
	else 
		throw eMatrix("erro: Matrix::setAt(): indice invalido");
}

std::string Matrix::asString()
{
	char *l = new char[col * 60];
	if(!l) {
		throw eMatrix("erro: Matrix::asString(): memoria insuficiente");
	}
	std::string r;
	sprintf(l,"%d %d\n",lin,col);
	r = l;
	for ( int i = 0; i < lin; ++i) {
		for(int j = 0; j < col; ++j) {
			if(j)
				sprintf(l," %d",at(i,j));
			else
				sprintf(l,"%d",at(i,j));
			r += l;
		}
		r += "\n";
	}
	delete l;
	return r;
}

Matrix::~Matrix()
{
	//if(mMatrix) delete mMatrix;
	mMatrix = 0;
}

Matrix::Matrix(std::ifstream &in) {
	int l,c,tmp;
	in >> l;
	mMatrix = 0;
    lin = col = 0;
	if(in.eof()) exit(0);
    in >> c;
    if(in.eof()) throw eMatrix("erro: Matrix::Matrix(): fim de arquivo prematuro");
	lin = l;
	col = c;
	mMatrix = new int[ l*sizeof(int)*c*sizeof(int) ];
	if (!mMatrix) {
		throw eMatrix("erro: Matrix::Matrix(): memoria insuficiente");
	}	
	for(int i = 0; i < l; ++i) {
		for(int j = 0; j < c; ++j) {
			if(in.eof()) {
				throw eMatrix("erro: Matrix::Matrix(): fim de arquivo prematuro");
			}
			in >> tmp;
			setAt(i,j,tmp);
		}
	}
}

int main() {
	try {
		std::ifstream IN("L0Q1.in",std::ios::in);
		std::ofstream OUT("L0Q1.out",std::ios::out);
		int conj = 1;
		while (!IN.eof()) {
			Matrix x(IN);
			Matrix y(IN);
			Matrix m = x*y;
			OUT << "Conjunto #" << conj++ << std::endl;
			if(m.bad())
				OUT << "-1" << std::endl;
			else 
				OUT << m.asString();
			OUT << std::endl;
			x.free();
			y.free();
			m.free();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	} catch (eMatrix &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
