#ifndef __LOG_H__
#define __LOG_H__
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
	
class fim_de_linha {};

static fim_de_linha fdl;

class log {
	std::string now(void);
	std::ofstream log_o;
	std::string logFile;
public:
	log(std::string f):logFile(f) {}
	~log(){};
	void start(void) { log_o.open(logFile.c_str(),std::ios::app); }
	std::string s;
	template<class T> log &operator<<(const T &f) {
		this->ins(f);
		return *this;
	}
	void ins(const double &i) {
		char c[30];
		sprintf(c,"%.3f",i);
		s += c;
	}
	void ins(const int &i) {
		char c[30];
		sprintf(c,"%d",i);
		s += c;
	}

	inline void ins(const char& c) { s += c; }
	inline void ins(const std::string& c) { s += c; }
	inline void ins(const fim_de_linha& c) {
		log_o << now() << s << std::endl;
		s.clear();
	}
};

#endif
