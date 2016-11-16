//2009 Kursancew, Vinicius Alexandre
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <cstring>
#include <string>
#include <malloc.h>
#include <fstream>
#include <cstdio>
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
	
	Matrix subMatrix(int,int,int,int);
	int average();
	int at(int, int);
	void setAt(int,int,int);
	int getLines() { return lin; }
	int getCols() { return col; }
	std::string asString();
	bool bad() { return (!col || !lin || !mMatrix); }
	void free() { delete mMatrix; mMatrix = 0; lin = col = 0; }
private:
	int* mMatrix;
	int lin;
	int col;
};

Matrix Matrix::subMatrix(int i1, int j1, int i2, int j2)
{
	int l = i2-i1 + 1; //2
	int c = j2-j1 + 1; //3
	if( l < 1 || c < 1) 
		throw eMatrix("erro: Matrix::subMatrix(): i1 > i2 ou j1 > 2");
	
	Matrix r(l,c);
	for (int i = 0; i < l; ++i) 
		for (int j = 0; j < c; ++j)
			r.setAt(i,j,self.at(i+i1,j+j1));
	return r;
}

int Matrix::average()
{
	int total = 0;
	for (int i = 0; i < lin; ++i)
		for (int j = 0; j < col; ++j)
			total += at(i,j);
	return (int)(total/(col*lin));
}


Matrix::Matrix(int i, int j) : lin(i),col(j) {
	if(i < 1 || j < 1) {
	    mMatrix = 0;
    	return;
	}
	mMatrix = new int[ i*sizeof(int)*j*sizeof(int) ];
	if (!mMatrix) 
		throw eMatrix("erro: Matrix::Matrix(): memoria insuficiente");
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
}

Matrix::Matrix(std::ifstream &in) {
	int l,c,tmp;
	in >> l;
	mMatrix = 0;
    lin = col = 0;
	if(in.eof()) exit(0);
	in >> c;
	if(in.eof()) eMatrix("erro: Matrix::Matrix(): fim de arquivo prematuro");
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
	std::ifstream IN("L0Q2.in",std::ios::in);
	std::ofstream OUT("L0Q2.out",std::ios::out);
	int conj = 1;
	while (!IN.eof()) {
		Matrix x(IN);
		Matrix m(x.getLines(),x.getCols());
		for(int i = 0; i < x.getLines(); ++i)
			for (int j = 0; j < x.getCols(); ++j) {
				Matrix tmp = x.subMatrix(0,0,i,j);
				m.setAt(i,j,tmp.average());
				tmp.free();
			}
		OUT << "Conjunto #" << conj++ << std::endl;
		if(m.bad())
			OUT << "-1" << std::endl;
		else 
			OUT << m.asString();
		OUT << std::endl;
		x.free();
		m.free();
	}
	
	return 0;
}
