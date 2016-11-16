#ifndef __COR_H__
#define __COR_H__

class Cor {
	int mr, mg, mb;
public:
	Cor();
	Cor(int, int, int);
	Cor(unsigned int);
	int& r() {return mr;}
	int& g() {return mg;}
	int& b() {return mb;}
	unsigned int asInt();
};

#endif

