#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <string.h>

class Time {
public:
	int pontos;
	int saldo;
	std::string nome;
	Time(){ }
	//~Time() { if(nome) delete nome; }
	Time(std::string _nome, int _pontos, int _saldo) { nome = std::string(_nome); pontos = _pontos; saldo = _saldo; }
	void procPartida(int _saldo); // se saldo=0 empate, >0 vitoria, <0 derr
	bool operator<=(Time &t2) {
		if(this->pontos < t2.pontos) return true;
		if(this->pontos == t2.pontos) {
			if(this->saldo == t2.saldo) return (this->nome >= t2.nome);
			return (this->saldo < t2.saldo);
		}
		return false;
	}
};

void Time::procPartida(int _saldo) {
	if(_saldo > 0) pontos += 3;
	else if(!_saldo) pontos += 1;
	saldo += _saldo;
}

void print_times(Time* tim, int tam, std::ofstream &OUT)
{
	for(int i = 0; i < tam; ++i) {
		OUT << i+1 << "o->" << tim[i].nome << "(p:" << tim[i].pontos << 
			" s:" << tim[i].saldo << ")";
		if((i+1)%4 == 0 && i != tam-1) OUT << std::endl;
		else if(i != tam-1) OUT << " ";
	}
	OUT << std::endl;
}
static int TROCAS;
void merge(Time* array1, Time* array2, int tam1, int tam2)
{
	int i,j,k;
	Time tmp[tam1+tam2];
	i = j = k = 0;
	while (i < tam1 || j < tam2) {
		if (i >= tam1) {
			tmp[k++] = array2[j++];
		} else if(j >= tam2) {
			tmp[k++] = array1[i++];
		} else if(array2[j] <= array1[i]) {
			tmp[k++] = array1[i++];
		} else {
			++TROCAS;
			tmp[k++] = array2[j++];
		}
	}
	for(int i = 0; i < tam1+tam2;++i) {
		array1[i] = tmp[i];
	}
}

void sort(Time* array, int beg, int end)
{
	int mid = beg+(end-beg)/2;
	if(beg+1 == end) return;
	sort(array,beg,mid);
	sort(array,mid,end);
	merge(array+beg,array+mid,mid-beg,end-mid);
}

int main()
{
	std::ofstream OUT("L4Q1.out",std::ios::out);
	std::ifstream IN("L4Q1.in",std::ios::in);
	IN.setf(std::ios::skipws);
	int conj = 0;
	while(!IN.eof()) {
		int n,n2;
		IN >> n;
		if(IN.eof()) break;
		n2 = 1 << n;
		//std::cout << n << " " << n2 << std::endl;
		Time times[n2];
		OUT << "Conjunto #" << ++conj << std::endl;
		for(int i = 0; i < n2; ++i) {
			std::string nome;
			int pontos, saldo;
			IN >> nome;
			IN >> pontos;
			IN >> saldo;
			times[i] = Time(nome, pontos, saldo);
		}
		bool done = false;
		while (!done) {
			std::string cmd;
			IN >> cmd;
			if (cmd == "r") {
				std::string t1,t2;
				int g1,g2;
				IN >> t1;
				IN >> g1;
				IN >> t2;
				IN >> g2;
				for (int i = 0; i < n2; ++i) {
					if(times[i].nome == t1) {
						times[i].procPartida(g1-g2);
					} else if(times[i].nome == t2) {
						times[i].procPartida(g2-g1);
					}
				}
			} else if (cmd == "c") {
				TROCAS = 0;
				sort(times,0,n2);
				OUT << "trocas: " << TROCAS << std::endl;
				print_times(times, n2, OUT);
			} else {
				TROCAS = 0;
				sort(times,0,n2);
				OUT << "trocas: " << TROCAS << std::endl;
				print_times(times, n2, OUT);
				done = true;
			}
		}
		OUT << std::endl;
	}
}
