//2009 Kursancew, Vinicius Alexandre
//C++ heap implementation (note: not copy safe, dont use operator=)
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <exception>
#include <cstdlib>
#include <string>

template <class	T, int N> class	heap {
	T theHeap[N];
	int	mSize;
public:
	heap() { mSize = 0;	}
	void add(T);
	T pop();
	T top()	{ return theHeap[0]; }
	int	size() { return	mSize; }
	void heapfy();
	void heapfy(int i);
	void p(void(*e)(T&,std::ofstream&),	std::ofstream& o) {	for	(int i = 0;	i <	mSize; ++i)	e(theHeap[i],o); }
};

template <class	T, int N> void heap<T,N>::heapfy()
{
	for(int i = mSize/2; i >= 1; --i) {
		heapfy(i);
	}
}

template <class	T, int N> void heap<T,N>::heapfy(int i)
{
	while ( i <= mSize/2) {
		int maior = 2*i;
		if( i*2+1 <= mSize) {
			if(theHeap[2*i-1] < theHeap[2*i])
				maior = 2*i + 1;
		}
		if(theHeap[i-1] < theHeap[maior-1]) {
			T temp = theHeap[i-1];
			theHeap[i-1] = theHeap[maior-1];
			theHeap[maior-1] = temp;
			i = maior;
		} else i = mSize;
	}
}

template <class	T, int N> void heap<T,N>::add(T	d)
{
	theHeap[mSize++] = d;
}

template <class	T, int N> T	heap<T,N>::pop()
{
	T tmp =	theHeap[0];
	--mSize;
	theHeap[0] = theHeap[mSize];
	if(mSize == 1) return tmp;
	heapfy(1);
	return tmp;
/*	int child = 2;
	int parent = 1;
	while (child < mSize) {
		if (child + 1 < mSize)
			if (theHeap[child-1] < theHeap[child]) ++child;
		if ( theHeap[parent-1] < theHeap[child-1] ) {
			T tmp2 = theHeap[parent-1];
			theHeap[parent-1] = theHeap[child-1];
			theHeap[child-1] = tmp2;
			parent = child;
			child *= 2;
		} else child = mSize;
	}*/
	/*int	pos	= 0;
	while((theHeap[pos]	< theHeap[pos*2	+ 1] ||	theHeap[pos] < theHeap[pos*2 + 2]) && (	pos	<= (mSize/2)-1 ) ) {
		if (theHeap[pos*2 +	2] <= theHeap[pos*2	+ 1]) {
			T tmp2 = theHeap[pos];
			theHeap[pos] = theHeap[pos*2 + 1];
			theHeap[pos*2 +	1] = tmp2;
			pos	= pos*2+1;
		} else { //if (	theHeap[pos*2 +	1] < theHeap[pos*2 + 2]	)
			T tmp2 = theHeap[pos];
			theHeap[pos] = theHeap[pos*2 + 2];
			theHeap[pos*2 +	2] = tmp2;
			pos	= pos*2+2;
		}
		if(pos*2+2 >= mSize) { std::cout << "cu\n" ; exit(-1); }
	}*/
}

class trabalho {
public:
	trabalho(std::string n,	int	p, int p2):nome(n),perm(p),paginas(p2) {}
	trabalho() { nome=""; perm=-1; paginas=0; }
	std::string	nome;
	int	perm,paginas;
	bool operator<=(trabalho o2) { return perm <= o2.perm; }
	bool operator>=(trabalho o2) { return perm >= o2.perm; }
	bool operator<(trabalho	o2)	{ return perm <	o2.perm; }
	bool operator>(trabalho	o2)	{ return perm >	o2.perm; }
};

class impressora {
	bool perms[5];
public:
	trabalho *trabalho_atual;
	impressora(std::string n):nome(n) {	for(int	i =	0; i < 5; ++i) perms[i]	= false; trabalho_atual	= NULL;	}
	void add_perm(int p) { perms[p]	= true;	}
	bool is_ociosa() { return trabalho_atual ==	NULL; }
	bool can_print(trabalho	p) { return	(perms[p.perm] && !trabalho_atual);	}
	void set_job(trabalho t) {
	   if(trabalho_atual) delete trabalho_atual;
	   trabalho_atual =	new	trabalho(t);
	}
	void do_page() {
		if (trabalho_atual)
		   if (--trabalho_atual->paginas ==	0) { delete	trabalho_atual;	trabalho_atual = NULL; }
	}
	std::string	nome;
};

void tp(trabalho &t, std::ofstream &OUT) {
	OUT	<< t.nome << " ";
}

class spool	{
	int	nimpressoras;
	impressora *impressoras[50];
	heap<trabalho, 60> trabalhos;
public:
	spool()	{ nimpressoras = 0;	}
	~spool();
	static int classe_to_int(std::string c);
	void add_impressora(std::string	nome) {	impressoras[nimpressoras++]	= new impressora(nome);	}
	bool add_classe(std::string	nome, std::string classe);
	void add_job(std::string perm, std::string job,	int	pages) { trabalhos.add(	trabalho(job, classe_to_int(perm), pages) );}
	void print(std::ofstream &OUT);
};

const static std::string tabela_perm[] = { "adm", "grad", "mest", "dout", "prof" };

int	spool::classe_to_int(std::string c)
{
	if(c ==	"prof")	return 4;
	else if(c == "dout") return	3;
	else if(c == "mest") return	2;
	else if(c == "grad") return	1;
	return 0;
}

spool::~spool()
{
	for	(int i = 0;	i <	nimpressoras; ++i) {
		delete impressoras[i];
		impressoras[i] = 0;
	}
}

bool spool::add_classe(std::string nome, std::string classe)
{
	for	(int i = 0;	i <	nimpressoras; ++i)
		if(impressoras[i]->nome	== nome) {
			impressoras[i]->add_perm(classe_to_int(classe));
			return true;
		}
	return false;
}

void spool::print(std::ofstream	&OUT)
{
	trabalhos.heapfy();
	bool done = false;
	while (!done) {
		for	(int i = 0;	i <	nimpressoras; ++i) {
			if(trabalhos.size())
				if ( impressoras[i]->can_print(trabalhos.top())	) {
					impressoras[i]->set_job(trabalhos.pop());
				}
		}
		OUT	<< "heap: ";
		if(	trabalhos.size())
			trabalhos.p(tp,	OUT);
		else OUT <<	"Vazio!";
		OUT	<< std::endl;
		done = true;
		for	(int i = 0;	i <	nimpressoras; ++i) {
			if (impressoras[i]->is_ociosa()) {
				OUT	<< "ociosa;";
			} else {
				OUT	<< "[" << impressoras[i]->trabalho_atual->nome << "," << impressoras[i]->trabalho_atual->paginas <<	"];";
			}
			done &= impressoras[i]->is_ociosa();
			impressoras[i]->do_page();
		}
		OUT	<< std::endl;
	}
}

int	main()
{
	std::ifstream IN("L3Q1.in",	std::ios::in);
	std::ofstream OUT("L3Q1.out", std::ios::out);
	IN.setf(std::ios::skipws);
	try	{
		int	conj = 1;
		while (!IN.eof()) {
			spool imps;
			int	nprinters;
			IN >> nprinters;
			if (IN.eof()) { OUT<< std::endl; break; }
			OUT	<< "Conjunto#" << conj++ <<	std::endl;
			for	(int i = 0;	i <	nprinters; ++i)	{
				std::string	nome;
				IN >> nome;
				imps.add_impressora(nome);
				std::string	cl;
				do {
					IN >> cl;
					if(cl != "fimpermissoes")
						imps.add_classe(nome, cl);
				} while	(cl	!= "fimpermissoes");
			}
			
			int	njobs;
			IN >> njobs;
			for	(int i = 0;	i <	njobs; ++i)	{
				std::string	perm, job;
				int	pages;
				IN >> perm >> job >> pages;
				imps.add_job(perm, job,	pages);
			}
			imps.print(OUT);
			OUT << std::endl;
		}
	} catch(std::exception &e) {
		//std::cerr << "erro:	" << e.what() << std::endl;
	}
}
