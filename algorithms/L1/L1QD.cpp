#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <cstdlib>
#include <string>
#include <exception>
#include <cstdio>
#include <assert.h>

#define MAX 10000


class eLista {
	std::string e;
public:
	eLista(std::string s):e(s){}
	std::string what() { return e; }
};

template <class T> class lista
{
	class sLista {
		public:
		sLista *next;
		T dado;
	};
public:
	class iterador {
		sLista *elm;
	public:
		iterador(sLista *e):elm(e){}
		bool next() { // retorna falso se chegou no fim da linha
			elm = elm?elm->next:0;
			if(elm) return true;
			return false;
		}
		bool isEnd() {
			return !elm;
		}
		sLista *elemento() { return elm; }
		T dado() { if(!elm) throw eLista("dereferenciamento de iterador nulo"); return elm->dado; }
		T operator*() {
			return dado();
		}
		iterador& operator++() {
			next();
			return *this;
		}
		/*iterador& operator++(int foo) {
			iterador c = iterador(*this);
			next();
			return c;
		}*/
	};

	lista();
	~lista();

	unsigned int tamanho();
	void inserir(T, unsigned int);
	void next();
	void remover(unsigned int);
	T obter(unsigned int);
	int achar(T);
	T operator[](unsigned int pos) {
		return obter(pos);
	}
	iterador inicio() {
		return iterador(mHead);
	}
private:
	sLista *mHead;
	unsigned int mCardinalidade;
};

template <class T> lista<T>::lista()
{
	mHead = 0;
	mCardinalidade = 0;
}

template <class T> lista<T>::~lista()
{
	for (unsigned int i = 0; i < tamanho(); ++i) {
		remover(0);
	}
	mHead = 0;
}

template <class T> T lista<T>::obter(unsigned int pos)
{
	iterador itr = inicio();
	if(pos >= mCardinalidade) pos = mCardinalidade-1;
	for(unsigned int i = 0; i < pos; ++i) ++itr;
	return *itr;
}

template <class T> int lista<T>::achar(T item)
{
	iterador itr = inicio();
	int i = 0;
	while (!itr.isEnd()) {
		if(*itr == item) return i;
		++itr;
		++i;
	}
	return -1;
}

template <class T> unsigned int lista<T>::tamanho()
{
	return mCardinalidade;
}

template <class T> void lista<T>::inserir(T item, unsigned int pos)
{
	if (pos > mCardinalidade) pos = mCardinalidade-1;
	if(!mHead || !pos) {
		sLista *tmp = new sLista;
		tmp->dado = item;
		tmp->next = mHead;
		mHead = tmp;
	} else {
		iterador itr = inicio();
		unsigned int i = 0;
		sLista *ant = 0;
		while ( i++ < pos && itr.elemento()) {
			ant = itr.elemento();
			++itr;
		}
		if(!ant) throw eLista("lista<T>::inserir: dereferencia de ponteiro nulo");
		sLista* tmp = new sLista;
		tmp->dado = item;
		tmp->next = ant->next;
		ant->next = tmp;
	}
	++mCardinalidade;
}
template <class T> void lista<T>::remover(unsigned int pos)
{
	if (pos >= mCardinalidade) pos = mCardinalidade-1;
	if(mCardinalidade--) {
		if(pos == 0) {
			sLista *tmp = mHead->next;
			delete mHead;
			mHead = tmp;
		} else {
			iterador itr = inicio();
			for(unsigned int i = 0; i < pos-1; ++i) ++itr;
			sLista *ant = itr.elemento();
			if(!ant) throw eLista("lista<T>::remover: dereferencia de ponteiro nulo");
			sLista *tmp = ant->next;
			ant->next = tmp->next;
			delete tmp;
		}
	}
}

int main() 
{
	bool array[MAX];
	int primes[220];
	for(int i = 0; i < MAX; ++i)
		array[i] = 1;
	array[0] = 0;
	array[1] = 0;
	int fat = 2;
	int hi = 0, low = 0;
	int nprimes = 0;
	while (fat*fat <= MAX) {
		int mm = 2*fat;
		while (mm <= MAX) {
			array[mm] = 0;
			mm += fat;
		}
		hi = fat*fat;
		for(;low < hi;++low) {
			if(array[low]) {
				primes[nprimes] = low;
				++nprimes;
			}
		}
		if(nprimes >= 220) break;
		++fat;
		while(!array[fat])++fat;
		assert(hi <= MAX);
	}
	for(;low <= MAX; ++low) {
		if(array[low]) {
			primes[nprimes] = low;
			if(++nprimes >= 220) break;
		}
	}
	std::ifstream IN("L1QD.in",std::ios::in);
	std::ofstream OUT("L1QD.out",std::ios::out);
	while (!IN.eof()) {
		int pessoas;
		IN >> pessoas;
		//pessoas = 6;
		//std::cout << pessoas << std::endl;
		if(!pessoas) break;
		lista<int> li;
		for(int i = pessoas; i > 0; --i)
			li.inserir(i,0);
		int primo_atual = 0;
		int ult_rem = 0;
		while(li.tamanho() > 1) {
			int pos_rem = ult_rem + array[primo_atual]%li.tamanho();
			li.remover(pos_rem);
			++primo_atual;
		}
		OUT << li.obter(0) << std::endl;
	}
	return 0;
}
