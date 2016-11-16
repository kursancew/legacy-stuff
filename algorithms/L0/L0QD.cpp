//2009 Kursancew, Vinicius Alexandre
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <cstdlib>
#include <fstream>
#include <exception>

long double fat(long double n) {
	if(n == 0) return 1;
	return n*fat(n-1);
}
long double partfat(long double n, long double lim) {
	if (n == lim) return lim;
	return n*fat(n-1);
}

//ja q nao pode usar libs
long long rnd(long double n) {
	if ( n-(long long)n < 0.5 ) return (long long)n;
	else return (long long)n + 1;
}

int main() {
	try {
		std::ifstream IN("L0QD.in",std::ios::in);
		std::ofstream OUT("L0QD.out",std::ios::out);
		while (!IN.eof()) {
			long double m,n;
			IN >> n;
			if(IN.eof()) exit(0);
			IN >> m;
			long double fatm = fat(m);
			long double fatnm = partfat(n-m,fatm);
			long double fatn = partfat(n,fatnm);
			// n!/(n-m)!*m!
			OUT << rnd(fatn/(fatnm*fatm))  << std::endl;
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	return 0;
}
