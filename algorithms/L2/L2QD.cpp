//2009 Kursancew, Vinicius Alexandre
//AVL C++ template implementation
#include <cstdio>
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <exception>
#include <cstdlib>

int modulo(int i)
{
	if (i < 0) return (-1*i);
	else return i;
}

class eBad {
	std::string e;
public:
	eBad(std::string s): e(s) {}
	std::string what() { return e; }
};

class eExists {};
class eEnded {
	int e;
public:
	eEnded(int i): e(i){}
	int what() {return e;}
};

template <class T> class AVLNode {
public:
	AVLNode() { balance = 0; fdir = fesq = 0; }
	//AVLNode(T& d) {  balance = 0; fdir = fesq = 0; dado = d; }
	T dado;
	AVLNode<T> *fdir, *fesq;
	int balance;
};

template <class T> class BRet {
public:
	int balancing;
	AVLNode<T> *gruda;
	BRet(AVLNode<T>* r,int b) { gruda = r; balancing = modulo(b); }
};

template <class T> class AVLTree {
	BRet<T> _inserir(T&, AVLNode<T>*);
	int (*compare)(T&,T&);
	std::string (*asa)(T&,bool);
	AVLNode<T> *root;
	void print(AVLNode<T>*);
	bool find(T&,AVLNode<T>*,int*);
	std::ofstream &OUT;
	void delall(AVLNode<T>*);
	void count(AVLNode<T>*, int*);
	
	void k_esimo(AVLNode<T>*, int*, int);
	void print_lvl(AVLNode<T>*,int*,int);
	void count_lr(T&,AVLNode<T>*);
public:
	AVLTree(std::ofstream &o);
	~AVLTree();
	void count_lr(T& s) { count_lr(s,root);}
	void k_esimo(int ke) { int l=0; k_esimo(root, &l, ke); if(l < ke) OUT << "Nao existe." << std::endl; }
	void print_lvl(int tl) { int l=0; print_lvl(root,&l,tl); OUT << std::endl; }
	bool inserir(T&);
	bool find(T&);
	void setCompare(int (*c)(T&, T&)) {compare = c;}
	void setAsStr(std::string (*a)(T&,bool)) { asa = a; }
};

//implementacoes
template <class T> void AVLTree<T>::count(AVLNode<T>* raiz, int* qtd)
{
	if(!raiz) return;
	*qtd += 1;
	count (raiz->fesq, qtd);
	count (raiz->fdir, qtd);
}

template <class T> void AVLTree<T>::k_esimo(AVLNode<T>* raiz, int* count, int target)
{
	if(!raiz) return;
	k_esimo(raiz->fesq,count,target);
	*count += 1;
	if(*count == target) {
		OUT << raiz->dado.nome << " " << raiz->dado.codigo << std::endl;
	}
	k_esimo(raiz->fdir,count,target);
}

template <class T> void AVLTree<T>::print_lvl(AVLNode<T>* raiz, int* clvl, int tlvl)
{
	if(!raiz) return;
	*clvl += 1;
	print_lvl(raiz->fesq,clvl,tlvl);
	print_lvl(raiz->fdir,clvl,tlvl);
	*clvl -= 1;
	if(*clvl == tlvl) {
		OUT << asa(raiz->dado, false) << " ";
	}
}

template <class T> AVLTree<T>::~AVLTree()
{
    delall(root);
}

template <class T> void AVLTree<T>::count_lr(T& subject, AVLNode<T>* r)
{
	if(!r) { OUT << "-1" << std::endl; return; }
	int c = compare(subject,r->dado);
	if(!c) {
		int c=0;
		count(r->fesq, &c);
		OUT << c << " ";
		c = 0;
		count(r->fdir, &c);
		OUT << c << std::endl;
	} else if (c < 0) {
		count_lr(subject,r->fesq);
	} else {
		count_lr(subject,r->fdir);
	}
}


template <class T> bool AVLTree<T>::find(T& subject, AVLNode<T>* r, int *depth)
{
	if(!r) throw std::string("Professor nao encontrado.");
	int c = compare(subject,r->dado);
	if(!c) {
		subject = r->dado;
		return true;
	} else if (c < 0) {
		*depth += 1;
		return (bool)find(subject,r->fesq,depth);
	} else {
		*depth += 1;
		return (bool)find(subject,r->fdir,depth);
	}
}

template <class T> bool AVLTree<T>::find(T& subject)
{
	try {
		int d = 0;
		find(subject,root, &d);
		int c = compare(subject, root->dado);
		if(!c) {
			OUT << asa(subject,true) << " esta na raiz." << std::endl;
		} else if( c > 0) {
			OUT << asa(subject,true) << " esta no nivel " << d << " direita." << std::endl;
		} else {
			OUT << asa(subject,true) << " esta no nivel " << d << " esquerda." << std::endl;
		}
	} catch (std::string &e) {
		OUT << e << std::endl;
		return false;
	}
	return true;
}

template <class T> AVLTree<T>::AVLTree(std::ofstream &o):OUT(o)
{
	root = NULL;
	compare = NULL;
}

template <class T> BRet<T> AVLTree<T>::_inserir(T& dado, AVLNode<T>* raiz)
{
	if(!raiz) {
		//std::cout << asa(dado) << "esta na raiz." << std::endl;
		AVLNode<T>* tmp = new AVLNode<T>;
		tmp->fdir = 0;
		tmp->fesq = 0;
		tmp->balance = 0;
		tmp->dado = dado;
		return BRet<T>(tmp,0);
	}
	if (compare(dado, raiz->dado) > 0) {
		//direita
		if(!raiz->fdir) {
			//chegou a um ponto onde nao ha mais pra onde caminhar
			//std::cout << "chegou onde nao da mais para caminhar (dir)" << std::endl;
			AVLNode<T>* tmp = new AVLNode<T>;
			tmp->fdir = 0;
			tmp->fesq = 0;
			tmp->dado = dado;
			tmp->balance = 0;
			raiz->fdir = tmp;
			++raiz->balance;
			return BRet<T>(raiz,raiz->balance);
		} else {
			//std::cout << "caminha (dir)" << std::endl;
			BRet<T> rval = _inserir(dado, raiz->fdir);
			raiz->fdir = rval.gruda;
			
			if(rval.balancing == 0) return BRet<T>(raiz,0); // retornou um 0, acaba recursao
			raiz->balance += rval.balancing;
			
			if(raiz->balance > 1 && raiz->fdir->balance > 0) {
				//balance > 1, rotacao simples para esquerda
				//std::cout << "rot simp esq " << raiz->dado << std::endl;
				AVLNode<T>* tmp = raiz->fdir;
				raiz->fdir = raiz->fdir->fesq;
				raiz->balance = 0;
				tmp->balance = 0;
				tmp->fesq = raiz;
				return BRet<T>(tmp,0);
			} else if(raiz->balance > 1 && raiz->fdir->balance < 0) {
				//balance > 1, rotacao dupla para esquerda
				//std::cout << "rot dup esq " << raiz->dado << std::endl;
				AVLNode<T>* tmp = raiz->fdir;
				AVLNode<T>* tmp2 = raiz->fdir->fesq;
				raiz->fdir = tmp2->fesq;
				tmp->fesq = tmp2->fdir;
				tmp2->fdir = tmp;
				tmp2->fesq = raiz;
				if(tmp2->balance == 0) {
					raiz->balance = 0;
					tmp->balance = 0;
				} else if (tmp2->balance == 1) {
					tmp2->balance = 0;
					raiz->balance = -1;
					tmp->balance = 0;
				} else if (tmp2->balance == -1) {
					tmp2->balance = 0;
					raiz->balance = 0;
					tmp->balance = 1;
				} else {
					throw eBad("rot dup esq: |tmp2| > 1");
				}
				return BRet<T>(tmp2,0);
			} else {
				return BRet<T>(raiz,raiz->balance);
			}
		}
	} else if(compare(dado, raiz->dado) < 0){
		//esquerda
		if(!raiz->fesq) {
			//chegou a um ponto onde nao ha mais pra onde caminhar
			//std::cout << "chegou onde nao da mais para caminhar (esq)" << std::endl;
			AVLNode<T>* tmp = new AVLNode<T>;
			tmp->fdir = 0;
			tmp->fesq = 0;
			tmp->dado = dado;
			tmp->balance = 0;
			raiz->fesq = tmp;
			--raiz->balance;
			return BRet<T>(raiz,raiz->balance);
		} else {
			//continua recursao
			//std::cout << "caminha (esq)" << std::endl;
			BRet<T> rval = _inserir(dado, raiz->fesq);
			raiz->fesq = rval.gruda;
			
			if(rval.balancing == 0) return BRet<T>(raiz,0); // retornou um 0 acaba a recursao
			raiz->balance -= rval.balancing;
			
			if(raiz->balance < -1 && raiz->fesq->balance < 0) {
				//balance < -1,  rotacao simples para direita
				//std::cout << "rot simp dir " << raiz->dado << std::endl;
				AVLNode<T>* tmp = raiz->fesq; // guardar raiz->fesq em tmp
				raiz->fesq = raiz->fesq->fdir;
				raiz->balance = 0; // zerar balances
				tmp->balance = 0;
				tmp->fdir = raiz;
				return BRet<T>(tmp,0); // voltar nova raiz para recursao
			} else if(raiz->balance < -1 && raiz->fesq->balance > 0) {
				//balance < -1, insercao a direida do filho, rotacao dupla para direita
				//std::cout << "rot dup dir " << raiz->dado << std::endl;
				AVLNode<T>* tmp = raiz->fesq;
				AVLNode<T>* tmp2 = raiz->fesq->fdir;
				raiz->fesq = tmp2->fdir;
				tmp->fdir = tmp2->fesq;
				tmp2->fesq = tmp;
				tmp2->fdir = raiz;
				if(tmp2->balance == 0) {
					raiz->balance = 0;
					tmp->balance = 0;
				} else if (tmp2->balance == 1) {
					tmp2->balance = 0;
					raiz->balance = 0;
					tmp->balance = -1;
				} else if (tmp2->balance == -1) {
					tmp2->balance = 0;
					raiz->balance = 1;
					tmp->balance = 0;
				} else {
					throw eBad("rot dup dir: |tmp2| > 1");
				}
				return BRet<T>(tmp2,0);
			} else {
				return BRet<T>(raiz,raiz->balance);
			}
		}
	} else throw eExists();
}

template <class T> void AVLTree<T>::delall(AVLNode<T>* r)
{
	if(!r) return;
	if(r->fesq) delall(r->fesq);
    if(r->fdir) delall(r->fdir);
	delete r;
}

template <class T> bool AVLTree<T>::inserir(T& dado)
{
	try {
		if(!compare) throw eBad("!compare");
		BRet<T> rval = _inserir(dado, root);
		root = rval.gruda;
		find(dado);
	} catch (eBad &e) {
		std::cerr << e.what() << std::endl;
		exit(-1);
	} catch (eExists &e) {
		OUT << "Professor ja existe!" << std::endl;
		return false;
	} catch (eEnded &e){
	
	}
	return true;
}

class professor {
public:
	professor(){ nome = ""; codigo = -1; }
	professor(std::string n, int c):nome(n),codigo(c) {}
	std::string nome;
	int codigo;
};

int intcompare(professor& a, professor& b) {
	if(a.codigo > b.codigo) return 1;
	else if(b.codigo > a.codigo) return -1;
	return 0;
}
int strcompare(professor& a, professor& b) {
	if(a.nome > b.nome) return 1;
	else if(b.nome > a.nome) return -1;
	return 0;
}

std::string printcod(professor &p, bool t)
{
	char x[100];
	sprintf(x,"%d",p.codigo);
	if(t)
		return std::string("Codigo: ") + std::string(x);
	return p.nome;
}
std::string printstr(professor &p, bool t)
{
	if(t)
		return std::string("Nome: ") + p.nome;
	char x[100];
	sprintf(x,"%d",p.codigo);
	return std::string(x);
}

int main()
{
	std::ifstream IN("L2QD.in",std::ios::in);
	std::ofstream OUT("L2QD.out",std::ios::out);
	int conj = 1;
	try { 
		while(!IN.eof()) {
			int ops;
			char t[10];
			bzero(t,10);
			while (!strlen(t)) {
				IN.getline(t,10);
				if(IN.eof()) break;
			}
			if(IN.eof()) break;
			OUT << "Conjunto #" << conj++ << std::endl;
			std::string tipo(t);
			AVLTree<professor> x(OUT);
			IN >> ops;
			if(IN.eof()) break;
			IN.getline(t,5);
			if(tipo == "codigo") {
				x.setCompare(intcompare);
				x.setAsStr(printstr);
			} else if(tipo == "nome") {
				x.setCompare(strcompare);
				x.setAsStr(printcod);
			} else {
				throw eBad("tipo inexistente");
			}
			for (int i = 0; i < ops; ++i) {
				char l[1000];
				char nome[1000];
				char foo[10];
				char operacao;
				int cod;
				IN.getline(l,1000);
				//std::cout << l << " -> ";
				operacao = l[0];
				if (operacao == 'I') {
					sscanf(l,"%s %s %d",foo,nome,&cod);
					professor p = professor(nome,cod);
					//std::cout << "insere " << nome << " " << cod << std::endl;
					x.inserir(p);
				} else if (operacao == 'L') {
					if(tipo == "codigo") {
						sscanf(l,"%s %d",foo,&cod);
						professor p = professor("",cod);
						x.find(p);
					} else {
						sscanf(l,"%c %s",&operacao,nome);
						professor p = professor(nome,0);
						x.find(p);
					}
				} else if (operacao == 'F') {
					if(tipo == "codigo") {
						sscanf(l,"%s %d",foo,&cod);
						professor p = professor("",cod);
						x.count_lr(p);
					} else {
						sscanf(l,"%c %s",&operacao,nome);
						professor p = professor(nome,0);
						x.count_lr(p);
					}
				} else if (operacao == 'M') {
					sscanf(l,"%s %d",foo,&cod);
					x.k_esimo(cod);
				} else if (operacao == 'N') {
					sscanf(l,"%s %d",foo,&cod);
					x.print_lvl(cod);
				}
			}
			OUT << std::endl;
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	} catch (eBad &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
