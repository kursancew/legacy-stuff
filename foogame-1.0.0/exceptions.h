#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <string>

class graph_exception {
public:
	graph_exception(std::string erro) {e = erro;}
	std::string what() {return e;}
private:
	std::string e;
};

class audio_exception {
public:
	audio_exception(std::string erro) {e = erro;}
	std::string what(){return e;}
private:
	std::string e;
};

#endif
