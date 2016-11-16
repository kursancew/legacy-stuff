//2009 Kursancew, Vinicius Alexandre
//hash and rehash c++
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <exception>

class hashcontainter {
	std::string str;
	int ocorrencias;
	bool removido;
	bool ocupado;
public:
	hashcontainter() { removido = 0; ocorrencias = 0; ocupado = 0;}
	hashcontainter(std::string s, int o): str(s),ocorrencias(o) 
	{ removido = 0; ocupado = 1; }
	std::string get_val() { return str; }
	int get_ocorrencias() { return ocorrencias; }
	void flag_removido() { removido = true; }
	bool is_removido() { return removido; }
	bool is_ocupado() { return ocupado; }
};

long hash(std::string str)
{
	long sum = 11;
	for (size_t i = 0; i < str.length(); ++i) 
		sum = sum*3 + str.at(i);	
	return sum % 127;
}

long rehash(std::string str)
{
	long sum = 0;
	for(size_t i = 0; i < str.length(); ++i) 
		sum += str.at(i);
	return sum;
}

int main()
{
	std::ifstream IN("L3Q2.in", std::ios::in);
	std::ofstream OUT("L3Q2.out", std::ios::out);
	IN.setf(std::ios::skipws);
	try {
		int conj = 1;
		while (!IN.eof()) {
			int ninstrucoes;
			IN >> ninstrucoes;
			if (IN.eof()) throw std::exception();
			hashcontainter table[127];
			OUT << "Conjunto#" << conj++ << std::endl;
			for(int i = 0; i < ninstrucoes; ++i) {
				std::string instrucao;
				IN >> instrucao;
				if ( instrucao == "i" ) {
					std::string dado;
					int ocorrencias;
					IN >> dado >> ocorrencias;
					int h = hash(dado);
					int max = 0;
					while (table[h].is_ocupado() && max < 127) {
						h = (h + rehash(dado)) % 127;
						++max;
					}
					if (max >= 127) std::cout << "sacanearam, inseriram mais de 127" << std::endl;
					OUT << dado << " inserida em " << h << std::endl;
					table[h] = hashcontainter(dado, ocorrencias);
				} else if (instrucao == "cs") {
					std::string dado;
					IN >> dado;
					int h = hash(dado);
					int max = 0;
					while(table[h].is_ocupado() && table[h].get_val() != dado
							&& max < 127) {
						h = (h + rehash(dado)) % 127;
						++max;
					}
					if (table[h].get_val() == dado)
						OUT << "[" << h << "] : " << table[h].get_ocorrencias()
							<< std::endl;
					else
						OUT << "nenhum resultado." << std::endl;
				} else {
					std::string dado1,dado2,conectivo;
					IN >> dado1 >> conectivo >> dado2;
					int h1 = hash(dado1);
					int max = 0;
					bool f1 = false, f2 = false;
					while(table[h1].is_ocupado() && table[h1].get_val() != dado1 && max < 127) {
						h1 = (h1 + rehash(dado1)) % 127;
						++max;
					}
					if (table[h1].get_val() == dado1) f1 = true;
					else h1 = -1;
					
					int h2 = hash(dado2);
					max = 0;
					while(table[h2].is_ocupado() && table[h2].get_val() != dado2 && max < 127) {
						h2 = (h2 + rehash(dado2)) % 127;
						++max;
					}
					if (table[h2].get_val() == dado2) f2 = true;
					else h2 = -1;
					
					if(conectivo == "OR") {
						OUT << "[" << h1 << "," << h2 << "] : " <<
							(h1>=0?table[h1].get_ocorrencias():0) + (h2>=0?table[h2].get_ocorrencias():0)
							<< std::endl;
					} else {
						if(f1 && f2) OUT << "[" << h1 << "," << h2 << "] : " << 
						(table[h1].get_ocorrencias() < table[h2].get_ocorrencias()?
							table[h1].get_ocorrencias():table[h2].get_ocorrencias()) << std::endl;
						else OUT << "nenhum resultado." << std::endl;
					}
				}
			}
			OUT << std::endl;
		}
	} catch (std::exception &e) {
		//std::cerr << e.what() << std::endl;
	}
}
