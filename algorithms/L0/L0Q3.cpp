//2009 Kursancew, Vinicius Alexandre
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <cstring>
#include <cstdio>
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

	int at(int, int);
	void setAt(int,int,int);
	int getLines() { return lin; }
	int getCols() { return col; }
	std::string asString();
	bool bad() { return (!col || !lin || !mMatrix); }
	void free() { delete mMatrix; mMatrix = 0; lin = col = 0; }
	int parity(); // 1=eh par 2=nao par, pode ser 3=nao par, nao pode ser modificada
private:
/* cuidado com este ponteiro, pois quando eh feita uma copia do objeto o conteudo 
do ponteiro nao eh copiado, apenas referenciado, por isso nao de pode liberar o 
ponteiro no ~Matrix. Teria que se fazer um overload do operator= pra permitir isto */

	int* mMatrix; 
	int lin;
	int col;
};


int Matrix::parity()
{
 	int sum = 0;
 	int lins_impar = 0,cols_impar = 0;
	std::cout << lin << " " << col << " ";
 	for (int i = 0; i < lin; ++i) {
 		for (int j = 0; j < col; ++j)
			sum += at(i,j);
		if (sum%2) ++lins_impar;
		sum = 0;
	}
	sum = 0;
	std::cout << lins_impar << " ";
 	for (int i = 0; i < lin; ++i) {
 		for (int j = 0; j < col; ++j)
				sum += at(j,i);
		if (sum %2) ++cols_impar;
		sum = 0;
	}
	std::cout << lins_impar << std::endl;
	if (!lins_impar && !cols_impar) return 1;
	else if(cols_impar == 1 && lins_impar == 1) return 2;
	else return 3;
}


Matrix::Matrix(int i, int j) : lin(i),col(j) {
	mMatrix = 0;
	if(i > 0 && j > 0) {
		mMatrix = new int[ i*sizeof(int)*j*sizeof(int) ];
		if (!mMatrix) 
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
	if(bad()) return std::string("-1");
	char *l = new char[col * 60];
	if(!l)
		throw eMatrix("erro: Matrix::asString(): memoria insuficiente");
	
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
}

Matrix::Matrix(std::ifstream &in) {
	int l,tmp;
	mMatrix = 0;
	in >> l;
	if(in.eof()) {
	    lin = col = 0;
		exit(0);
	}
	lin = col = l;
	if(lin > 0 && col > 0) {
		mMatrix = new int[ l*sizeof(int)*l*sizeof(int) ];
		if (!mMatrix) {
			throw eMatrix("erro: Matrix::Matrix(): memoria insuficiente");
		}
		for(int i = 0; i < l; ++i) {
			for(int j = 0; j < l; ++j) {
				if(in.eof()) {
					throw eMatrix("erro: Matrix::Matrix(): fim de arquivo prematuro");
				}
				in >> tmp;
				setAt(i,j,tmp);
			}
		}
	}
}

int main() {
	try {
		std::ifstream IN("L0Q3.in",std::ios::in);
		std::ofstream OUT("L0Q3.out",std::ios::out);
		while (!IN.eof()) {
			Matrix x(IN);
			if(!x.bad()) {
				if(x.parity() == 1)
					OUT << "true" << std::endl;
				else if(x.parity() == 2)
					OUT << "false possivel" << std::endl;
				else
					OUT << "false impossivel" << std::endl;
				//OUT << x.asString();
			}
			x.free();
		}
 	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
