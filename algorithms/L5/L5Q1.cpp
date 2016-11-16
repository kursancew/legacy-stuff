//2009 Kursancew, Vinicius Alexandre
//KNAPSACK dynamic programming implementation C++
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>

#define INF (1 << 30)
//#define INF 1000000

int min(int a, int b) {return a<=b?a:b;}

int resolve(int m, int n, int* v, int* w)
{
	if(!v || !w)
		throw std::invalid_argument("bad pointers");
	int custo_total = 0;
	for(int i = 0; i < n ; ++i)
		custo_total += v[i];
	++n;
	int W[n][custo_total+2];
	memset(W,0,(custo_total+2)*n*sizeof(int));
	for(int i = 0; i < n; ++i)
		W[i][0] = 0;
	int j = 0;
	do {
		++j;
		W[0][j] = INF;
		for(int i = 1; i < n; ++i) {
			if(v[i-1] >= j) {
				W[i][j] = min(W[i-1][j],w[i-1]);
			} else {
				W[i][j] = min(W[i-1][j],w[i-1]+W[i-1][j-v[i-1]]);
			}
		}
	} while (W[n-1][j] <= m && j != custo_total+1);
	return j-1;
}

int main()
{
	std::ifstream IN("L5Q1.in",std::ios::in);
	std::ofstream OUT("L5Q1.out",std::ios::out);
	IN.flags(std::ios::skipws);
	bool done = false;
	int conj = 0;
	while(!done) {
		int count;
		IN >> count;
		if(IN.eof()) {
			done = true;
			count = -1;
		}
		int *stash_peso = NULL;
		int *stash_custo = NULL;
		if (count > 0) {
			stash_peso = new int[count];
			stash_custo = new int[count];
		}
		for (int i = 0; i < count; ++i) {
			int w,c;
			IN >> w >> c;
			stash_peso[i] = w;
			stash_custo[i] = c;
		}
		bool fim = count <= 0;
		if(!fim)
			OUT << "Conjunto #" << ++conj << std::endl;
		while(!fim) {
			char com;
			IN >> com;
			if(com == 'c') {
				int container;
				IN >> container;
				int resp = resolve(container, count, stash_custo, stash_peso);
				OUT << "R$ " << resp*10 << ",00" << std::endl;
			} else
				fim = true;
		}
		if(stash_peso) {
			delete stash_peso;
			delete stash_custo;
			stash_peso = stash_custo = NULL;
		}
		OUT << std::endl;
	}
	return 0;
}
