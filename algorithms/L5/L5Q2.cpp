//2009 Kursancew, Vinicius Alexandre
//levenshtein distance c++ implementation
#include <string>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <math.h>
int min2(int a, int b) {
	if (a < b) return a;
	return b;
}
int min3(int a, int b, int c) {
	return min2(min2(a,b),c);
}
int min(int a, int b, int c)
{
	if(a <= b && a <= c)
		return a;
	if(b <= a && b <= c)
		return b;
	return c;
}

int
distance(const char *s1, const char *s2, std::ofstream &OUT)
{
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	int mat[len1+1][len2+1];
	for (int i = 0; i < len1+1; ++i) 
		mat[i][0] = i*2;
	for (int i = 0; i < len2+1; ++i)
		mat[0][i] = i*2;
	for (int i = 0; i < len1; ++i) {
		for (int j = 0; j < len2; ++j) {
			int cost = s1[i] == s2[j]?4:3;
				mat[i+1][j+1] = min(	mat[i][j+1] + 2, //del
									mat[i+1][j] + 2, //ins
									mat[i][j] + cost // match
									);

		}
	}
	OUT << mat[len1][len2] << std::endl;
	for(int i = 0; i < len1+1; ++i) {
		for(int j = 0; j < len2+1; ++j){
			OUT << mat[i][j];
			if(j != len2)
				OUT << " ";
		}
		OUT << std::endl;
	}
			
	return mat[len1][len2];
}

int
main()
{
	std::ifstream IN("L5Q2.in", std::ios::in);
	std::ofstream OUT("L5Q2.out", std::ios::out);
	IN.flags(std::ios::skipws);
	int c = 0;
	while(!IN.eof()) {
		std::string s1, s2;
		OUT << "Conjunto #" << ++c << std::endl;
		IN >> s1 >> s2;
		if(IN.eof()) break;
		distance(s1.c_str(), s2.c_str(), OUT);
		OUT << std::endl;
	}
	return 0;
}
