//2009 Kursancew, Vinicius Alexandre
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
const int tamanho_tabuleiro = 8;

class vertice {	
	bool proibido;
public:
	int pi,pj;
	int nivel;
	bool marcado;
	vertice();
	void set_proibido() { proibido = true; }
	bool get_proibido() { return proibido; }
	void print();
};

vertice::vertice()
{
	marcado = proibido = false;
	nivel = -1;
}

class tabuleiro {
public:
	vertice casas[tamanho_tabuleiro][tamanho_tabuleiro];
	tabuleiro() {}
	void set_proibida(int i , int j) { casas[i][j].set_proibido(); }
	bool is_proibida(int i, int j) { return casas[i][j].get_proibido(); }
	bool is_marcada(int i, int j) { return casas[i][j].marcado; }
	void marca(int i, int j) { casas[i][j].marcado = true; }
	void set_nivel(int i, int j, int p) { casas[i][j].nivel = p; }
	int get_nivel(int i, int j) { return casas[i][j].nivel; }
	void print();
	void desmarca();
};

void tabuleiro::desmarca()
{
	for(int i = 0; i < tamanho_tabuleiro; ++i)
		for(int j = 0; j < tamanho_tabuleiro; ++j) {
			casas[i][j].pi = i;
			casas[i][j].pj = j;
			casas[i][j].marcado = false;
			casas[i][j].nivel = -1;
		}
}

void tabuleiro::print()
{
	for(int i = 0; i < tamanho_tabuleiro; ++i) {
		for(int j = 0; j < tamanho_tabuleiro; ++j) {
			std::cout << casas[i][j].nivel << " ";
		}
		std::cout << std::endl;
	}
}

class fila {
	vertice *f[256];
	int ini, fim;
public:
	fila() { ini = fim = 0; }
	void ins(vertice* v) { f[fim++] = v; }
	vertice *primeiro() { return f[ini]; }
	void remove() { ++ini; }
	bool vazia() { return ini == fim; }
};

class peca {
protected:
	int posi, posj;
	int direcao;
public:
	peca(int i, int j):posi(i), posj(j) { }
	virtual ~peca() {}
	virtual void constroi_caminhos(tabuleiro&) {}
	int geti() { return posi; }
	int getj() { return posj; }
	void setij(int i, int j) { posi = i; posj = j; }
};

class torre : public peca {
public:
	torre(int i, int j): peca(i,j) {}
	virtual void constroi_caminhos(tabuleiro&);
};

void torre::constroi_caminhos(tabuleiro& t)
{
	fila ofila;
	ofila.ins(&t.casas[posi][posj]);
	t.casas[posi][posj].nivel = 0;
	while(!ofila.vazia()) {
		vertice* v = ofila.primeiro();
		ofila.remove();
		v->marcado = true;
		int k = 0;
		int i = v->pi;
		int j = v->pj;
		while (k < tamanho_tabuleiro) {
			if (!t.is_proibida(k, j) && !t.is_marcada(k,j)) {
				ofila.ins(&t.casas[k][j]);
				t.marca(k, j);
				t.set_nivel(k, j, t.get_nivel(i, j)+1);
			}	
			++k;
		}
		k = 0;
		while (k < tamanho_tabuleiro) {
			if(!t.is_proibida(i, k) && !t.is_marcada(i,k)) {
				ofila.ins(&t.casas[i][k]);
				t.marca(i,k);
				t.set_nivel(i, k, t.get_nivel(i, j) + 1);
			}
			++k;
		}
	}
}

class bispo : public peca {
public:
	bispo(int x, int y): peca(x,y) {}
	void constroi_caminhos(tabuleiro& t);
};

void bispo::constroi_caminhos(tabuleiro& t)
{
	fila ofila;
	ofila.ins(&t.casas[posi][posj]);
	t.casas[posi][posj].nivel = 0;
	while(!ofila.vazia()) {
		vertice* v = ofila.primeiro();
		ofila.remove();
		v->marcado = true;
		int i = v->pi;
		int j = v->pj;
		int k = 1;
		while (i+k < tamanho_tabuleiro && j+k < tamanho_tabuleiro) {
			if (!t.is_proibida(i+k, j+k) && !t.is_marcada(i+k,j+k)) {
				ofila.ins(&t.casas[i+k][j+k]);
				t.marca(i+k, j+k);
				t.set_nivel(i+k, j+k, t.get_nivel(i, j)+1);
			}
			++k;
		}
		k = 1;
		while (i-k >= 0 && j+k < tamanho_tabuleiro) {
			if (!t.is_proibida(i-k, j+k) && !t.is_marcada(i-k,j+k)) {
				ofila.ins(&t.casas[i-k][j+k]);
				t.marca(i-k, j+k);
				t.set_nivel(i-k, j+k, t.get_nivel(i, j)+1);
			}
			++k;
		}
		k=1;
		while (i-k >= 0 && j-k >= 0) {
			if (!t.is_proibida(i-k, j-k) && !t.is_marcada(i-k,j-k)) {
				ofila.ins(&t.casas[i-k][j-k]);
				t.marca(i-k, j-k);
				t.set_nivel(i-k, j-k, t.get_nivel(i, j)+1);
			}
			++k;
		}
		k=1;
		while (i+k < tamanho_tabuleiro && j-k >= 0) {
			if (!t.is_proibida(i+k, j-k) && !t.is_marcada(i+k,j-k)) {
				ofila.ins(&t.casas[i+k][j-k]);
				t.marca(i+k, j-k);
				t.set_nivel(i+k, j-k, t.get_nivel(i, j)+1);
			}
			++k;
		}
	}
}

class rainha : public peca {
public:
	rainha(int x, int y): peca(x,y) {}
	void constroi_caminhos(tabuleiro& t);
};

void rainha::constroi_caminhos(tabuleiro& t)
{
	fila ofila;
	ofila.ins(&t.casas[posi][posj]);
	t.casas[posi][posj].nivel = 0;
	while(!ofila.vazia()) {
		vertice* v = ofila.primeiro();
		ofila.remove();
		v->marcado = true;
		int i = v->pi;
		int j = v->pj;
		int k = 0;
		while (k < tamanho_tabuleiro) {
			if (!t.is_proibida(k, j) && !t.is_marcada(k,j)) {
				ofila.ins(&t.casas[k][j]);
				t.marca(k, j);
				t.set_nivel(k, j, t.get_nivel(i, j)+1);
			}	
			++k;
		}
		k = 0;
		while (k < tamanho_tabuleiro) {
			if(!t.is_proibida(i, k) && !t.is_marcada(i,k)) {
				ofila.ins(&t.casas[i][k]);
				t.marca(i,k);
				t.set_nivel(i, k, t.get_nivel(i, j) + 1);
			}
			++k;
		}

		k = 1;
		while (i+k < tamanho_tabuleiro && j+k < tamanho_tabuleiro) {
			if (!t.is_proibida(i+k, j+k) && !t.is_marcada(i+k,j+k)) {
				ofila.ins(&t.casas[i+k][j+k]);
				t.marca(i+k, j+k);
				t.set_nivel(i+k, j+k, t.get_nivel(i, j)+1);
			}
			++k;
		}
		k = 1;
		while (i-k >= 0 && j+k < tamanho_tabuleiro) {
			if (!t.is_proibida(i-k, j+k) && !t.is_marcada(i-k,j+k)) {
				ofila.ins(&t.casas[i-k][j+k]);
				t.marca(i-k, j+k);
				t.set_nivel(i-k, j+k, t.get_nivel(i, j)+1);
			}
			++k;
		}
		k=1;
		while (i-k >= 0 && j-k >= 0) {
			if (!t.is_proibida(i-k, j-k) && !t.is_marcada(i-k,j-k)) {
				ofila.ins(&t.casas[i-k][j-k]);
				t.marca(i-k, j-k);
				t.set_nivel(i-k, j-k, t.get_nivel(i, j)+1);
			}
			++k;
		}
		k=1;
		while (i+k < tamanho_tabuleiro && j-k >= 0) {
			if (!t.is_proibida(i+k, j-k) && !t.is_marcada(i+k,j-k)) {
				ofila.ins(&t.casas[i+k][j-k]);
				t.marca(i+k, j-k);
				t.set_nivel(i+k, j-k, t.get_nivel(i, j)+1);
			}
			++k;
		}
	}
}

int main()
{

	std::ofstream OUT("L6Q1.out", std::ios::out);
	std::ifstream IN("L6Q1.in", std::ios::in);
	IN.flags(std::ios::skipws);
	/*peca *p = new rainha(0,0);
	tabuleiro t;
	//t.set_proibida(2,2);
		t.desmarca();
	p->constroi_caminhos(t);

	t.print();*/
	//a.print(a.raiz);
	//t.print();
	
	
	while (!IN.eof()) {
			char type;
			int li, lj, proib;
			IN >> type >> li >> lj >> proib;
			if(IN.eof()) break;
			peca* p = NULL;
			switch(type) {
				case 't': p = new torre(li, lj); break;
				case 'b': p = new bispo(li, lj); break;
				case 'r': p = new rainha(li, lj); break;
			}
			if(!p) break;
			tabuleiro t;
			t.desmarca();
			for(int i = 0 ; i < proib; ++i) {
				int pi, pj;
				IN >> pi >> pj;
				t.set_proibida(pi, pj);
			}
			p->constroi_caminhos(t);
			int nreis;
			IN >> nreis;
			bool foo = false;
			int sum = 0;
			for(int i = 0; i < nreis; ++i) {
				int ri, rj;
				IN >> ri >> rj;
				t.desmarca();
				p->constroi_caminhos(t);
				//t.print();
				//std::cout << std::endl;
				if(!foo && t.casas[ri][rj].nivel > 0) {
					sum += t.casas[ri][rj].nivel;
					p->setij(ri,rj);
					OUT << sum << " ";
				} else {
					foo = true;
					OUT << "-1 ";
				}
			}
			OUT << std::endl;
			delete p;
	}
	OUT << std::endl;
}
