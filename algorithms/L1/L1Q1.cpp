//2009 Kursancew, Vinicius Alexandre
//dynamic list c++
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <cstdlib>
#include <string>
#include <exception>
#include <cstdio>

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
	if (pos > mCardinalidade) pos = mCardinalidade;
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
	std::ifstream IN("L1Q1.in",std::ios::in);
	std::ofstream OUT("L1Q1.out",std::ios::out);
	try {
		int conj = 1;
		while (!IN.eof()) {
			int carros;
			IN >> carros;
			if(IN.eof()) break;

			lista<int> posicoes;
			for (int i = 0; i < carros; ++i) {
				int nc;
				IN >> nc;
				posicoes.inserir(nc, posicoes.tamanho());
			}
			OUT << "Conjunto #" << conj++ << std::endl;
			while(!IN.eof()) {
				char l[800];
				IN.getline(l,800);
				std::string linha;
				linha = l;
				if (linha.find("fim") != std::string::npos) break;
				if(strlen(l) < 3) continue;
				char comando = linha[0];
				linha = linha.substr(2);
				int a1,a2;
				if(comando == 'i') {
					sscanf(linha.c_str(),"%d",&a1);
					posicoes.inserir(a1,posicoes.tamanho());
					lista<int>::iterador itr = posicoes.inicio();
					while(!itr.isEnd()) {
						OUT << *itr;
						++itr;
						if(!itr.isEnd()) OUT << " ";
					}
				} else if(comando == 'q') {
					sscanf(linha.c_str(),"%d",&a1);
					int curpos = posicoes.achar(a1);
					if(curpos < 0) {
						OUT << "carro inexistente" << std::endl;
						continue;
					}
					posicoes.remover(curpos);
					lista<int>::iterador itr = posicoes.inicio();
					while(!itr.isEnd()) {
						OUT << *itr;
						++itr;
						if(!itr.isEnd()) OUT << " ";
					}
				} else if(comando == 'p') {
					sscanf(linha.c_str(),"%d %d",&a1,&a2);
					int curpos = posicoes.achar(a1);
					if(curpos < 0) {
						OUT << "carro inexistente" << std::endl;
						continue;
					}
					int endpos = curpos+a2 > (int)posicoes.tamanho()?posicoes.tamanho():curpos+a2;
					posicoes.remover(curpos);
					posicoes.inserir(a1,endpos);
					lista<int>::iterador itr = posicoes.inicio();
					while(!itr.isEnd()) {
						OUT << *itr;
						++itr;
						if(!itr.isEnd()) OUT << " ";
					}
				} else if(comando == 'g') {
					sscanf(linha.c_str(),"%d %d",&a1,&a2);
					int curpos = posicoes.achar(a1);
					if(curpos < 0) {
						OUT << "carro inexistente" << std::endl;
						continue;
					}
					int endpos = curpos-a2 < 0?0:curpos-a2;
					posicoes.remover(curpos);
					posicoes.inserir(a1,endpos);
					lista<int>::iterador itr = posicoes.inicio();
					while(!itr.isEnd()) {
						OUT << *itr;
						++itr;
						if(!itr.isEnd()) OUT << " ";
					}
				}
				OUT <<std::endl;
			}
			OUT <<std::endl;
		}
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	} catch (eLista &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
