#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <string>

#define swap(arr,a,b,t) { (t)=(arr[a]); (arr[a])=(arr[b]); (arr[b])=(t); }

//#define Carro int
#ifndef Carro
class Carro {
public:
	std::string piloto;
	int tempo;
	Carro() { tempo = 0; }
	Carro(std::string _piloto):piloto(_piloto){ tempo = 0;}
	void addTempo(int t) { tempo += t; }
	bool operator<=(Carro& c) {
		if (this->tempo < c.tempo)
			return true;
		else if (this->tempo == c.tempo && this->piloto <= c.piloto)
			return true;
		return false;
	}
	bool operator>(Carro& c) {
		if (this->tempo > c.tempo)
			return true;
		else if (this->tempo == c.tempo && this->piloto > c.piloto)
			return true;
		return false;
	}
};
#endif
//void swap(Carro *v, int a, int b) { Carro t = v[a]; v[a] = v[b]; v[b] = t; }
Carro t;
void qsort(Carro* array, int l, int r, int s, std::ofstream &OUT)
{
	if ( r > l+1 ) {
		int i = l + 1;
		int j = r;
		int piv = l+(r-l-1)/2;
		Carro K = array[piv];
		array[piv] = array[l];
		array[l] = K;
		while(i < j) {
			if(array[i] <= K)
				++i;
			else if(array[j-1] > K) --j;
			else {
				//t = array[i];
				//array[i] = array[--j];
				//array[j] = t;
				--j;
				swap(array, i, j, t);
			}
		}
		--i;
		swap(array,i,l,t);
		OUT << K.piloto << ": ";
	    for (int w = 0; w < s; ++w) {
			OUT << array[w].tempo;
			if(w != s-1) OUT << " ";
		}
		OUT << std::endl;
		qsort(array, l, i, s, OUT);
		qsort(array, j, r, s, OUT);
	}
}

int main()
{
	//int a[] = { 100, 50, 25, 88, 2, 33, 44, 20, 99, 22, 90, 66, 47, 34, 86, 36, 98 };
	//int a[] = { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
	//int a[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	std::ifstream IN("L4Q2.in", std::ios::in);
	std::ofstream OUT("L4Q2.out", std::ios::out);
	IN.setf(std::ios::skipws);
	int conj = 0;
	while(!IN.eof()) {
		int ncarros;
		int netap;
		IN >> ncarros;
		if(IN.eof()) break;
		Carro carros[ncarros];
		for(int i = 0; i < ncarros; ++i) {
			std::string foo;
			IN >> foo;
			carros[i] = Carro(foo);
		}
		IN >> netap;
		OUT << "Conjunto #" << ++conj << std::endl;
		for(int i = 0; i < netap; ++i) {
			for(int j = 0; j < ncarros;++j) {
				int tempo;
				IN >> tempo;
				carros[j].addTempo(tempo);
   			}
			qsort(carros, 0, ncarros, ncarros, OUT);
			OUT << "colocacao = ";
			for(int j = 0; j < ncarros; ++j) {
				OUT << j+1 << "o->" << carros[j].piloto;
				if(j != ncarros-1) OUT << " ";
			}
			OUT << std::endl;
		}
		OUT << std::endl;
	}
	/*
	qsort(a, 0, 16, 16, OUT);
	for(int i = 0; i < 16; ++i)
		std::cout << *(a+i) << " ";
	std::cout << std::endl;
	*/
	return 0;
}
