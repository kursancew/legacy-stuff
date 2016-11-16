//2009 Kursancew, Vinicius Alexandre
//dijkstra c++ template implementation
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>

template <class	T, int N> class	heap {
	T theHeap[N];
	int	mSize;
	bool(*compare)(T, T);
public:
	heap() { mSize = 0;	}
	void add(T);
	T pop();
	T top()	{ return theHeap[0]; }
	int	size() { return	mSize; }
	void heapfy();
	void heapfy(int i);
	void set_compare(bool(*c)(T,T)) { compare = c; }
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
			if(compare(theHeap[2*i-1], theHeap[2*i]))
				maior = 2*i + 1;
		}
		if( compare(theHeap[i-1], theHeap[maior-1])) {
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
}
////////////////// -------------------------

const int inf = 1 << 28;

int id = 0;
class vertice {
	int contagem_adjacentes;
	int* menor_custo;

	bool visitado;
	int dist_atual;
	int myid;
public:
	vertice** adjacentes;
	int* pesos_adj;
	vertice(int max);
	~vertice();
	void add_adjacencia(vertice *, int, bool=true);
	void dijkstra( vertice**, int );
	void print_adj(int);
	void marca() { visitado = true; }
	void desmarca() { visitado = false; dist_atual = inf; }
	bool marcado() { return visitado; }
	int get_id() { return myid; }
	int Nadjacentes() { return contagem_adjacentes; }
	void set_peso_atual(int p) { dist_atual = p; }
	int get_peso_atual () { return dist_atual; }
	int get_cost(int v) { return menor_custo[v]; }
};

bool less(vertice* a, vertice* b) {
	if(a->get_peso_atual() > b->get_peso_atual()) return 1;
	return 0;
}

void vertice::dijkstra(vertice **G, int nvert)
{
	for (int i = 0; i < nvert; ++i) {
		menor_custo[i] = inf;
		G[i]->desmarca();
	}
	heap<vertice*, 1024> H;
	H.set_compare(less);
	H.add(this);
	this->marca();
	this->set_peso_atual(0);
	menor_custo[get_id()] = 0;
	while (H.size()) {
		vertice* v = H.pop();
		for (int i = 0; i < v->Nadjacentes(); ++i) { // para cada adjacente em A de v
			vertice* A = v->adjacentes[i];
			if(!A->marcado()) {
				H.add(A);
				A->marca();
				//A->set_peso_atual(v->get_peso_atual() + v->pesos_adj[i]);
				A->set_peso_atual(inf);
			}
			//if(menor_custo[v->get_id()] + v->pesos_adj[i] < menor_custo[A->get_id()]) {
			if(v->get_peso_atual() + v->pesos_adj[i] < A->get_peso_atual()) {
				menor_custo[A->get_id()] = menor_custo[v->get_id()] + v->pesos_adj[i];
				A->set_peso_atual(menor_custo[A->get_id()]);
			}
		}
		H.heapfy();
	}
}

void vertice::print_adj(int m)
{
	for(int i = 0; i < contagem_adjacentes; ++i) {
	//	std::cout << get_id() << "->" << adjacentes[i]->get_id() << " ";
	}
	for(int i = 0; i < m; ++i) {
		std::cout << get_id() << "->" << i << "=" << (menor_custo[i] < 10000?menor_custo[i]:-1) << " ";
	}
	std::cout << std::endl;
}

void vertice::add_adjacencia(vertice* v, int p, bool link)
{
	adjacentes[contagem_adjacentes] = v;
	pesos_adj[contagem_adjacentes] = p;
	++contagem_adjacentes;
	if(link)
		v->add_adjacencia(this, p, false);
}

vertice::vertice(int max)
{
	contagem_adjacentes = 0;
	adjacentes = new vertice*[max];
	pesos_adj = new int[max];
	menor_custo = new int[max];
	myid = id++;
}
vertice::~vertice()
{
	delete adjacentes;
	delete pesos_adj;
	delete menor_custo;
}

int main()
{
	/*vertice** graph;
	graph = new vertice*[10];
	for (int i = 0; i < 10; ++i)
		graph[i] = new vertice(10);
	graph[0]->add_adjacencia(graph[1],5);
	graph[0]->add_adjacencia(graph[2],5);
	graph[0]->add_adjacencia(graph[3],10);
	graph[2]->add_adjacencia(graph[3],1);
	graph[0]->dijkstra(graph, 10);
	for (int i = 0; i < 10; ++i) {
		graph[i]->print_adj();
	}
	*/
	std::ofstream OUT("L6Q2.out", std::ios::out);
	std::ifstream IN("L6Q2.in", std::ios::in);
	IN.flags(std::ios::skipws);
	int conj = 0;
	while (!IN.eof()) {
		int vert, arest;
		IN >> vert >> arest;
		if(IN.eof()) break;
		vertice** graph = new vertice*[vert];
		for(int i = 0; i < vert; ++i)
			graph[i] = new vertice(vert);

		for(int i = 0; i < arest; ++i) {
			std::string ar;
			char l;
			int peso, v1, v2;
			IN >> l >> v1 >> ar >> l >> v2 >> peso;
			//std::cout << v1 << "<->" << v2 << " " << peso << std::endl;
			graph[v1]->add_adjacencia(graph[v2], peso);
		}
		for(int i = 0; i < vert; ++i) {
			graph[i]->dijkstra(graph, vert);
			//graph[i]->print_adj(vert);
		}
		int ops;
		IN >> ops;
		int sum = 0;
		char l;
		int prev, next;
		IN >> l >> next;
		OUT << "Conjunto #" << ++conj << std::endl;
		for(int i = 0; i < ops; ++i) {
			prev = next;
			IN >> l >> next;
			OUT << "Gasto: " << graph[prev]->get_cost(next) << std::endl;
			sum += graph[prev]->get_cost(next);
		}
		OUT << "total: " << sum << std::endl;
		OUT << std::endl;
		for(int i = 0; i < vert; ++i)
			delete graph[i];
		delete graph;
		id = 0;
	}
}
