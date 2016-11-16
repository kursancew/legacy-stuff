#include "log.h"
#include <time.h>
#include <string>
std::string log::now(void) {
	time_t t;
	struct tm v;
	time(&t);
	gmtime_r(&t,&v);
	char c[32];
	sprintf(c,"%d%02d%02d-%02d:%02d ",v.tm_year,v.tm_mon,
			v.tm_mday,v.tm_hour,v.tm_min);
	return std::string(c);
}
	
