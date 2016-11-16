#include "timer.h"
#include <sys/time.h>
#include <sys/types.h>

/*
 * as rotinas sao "self explanatory"
 * */

Timer::Timer() {
	running = false;
	mReload = false;
}

void Timer::start(struct timeval tv) {
	gettimeofday(&read_tv,0);
	lapse_tv = tv;
	running = true;
}

void Timer::start(long msec) {
	gettimeofday(&read_tv,0);
	lapse_tv.tv_sec = msec / 1000;
	lapse_tv.tv_usec = msec%1000 * 1000;
	running = true;
}

long int Timer::lapse(void) {
	struct timeval tv;
	gettimeofday(&tv,0);
	long int tott = ((tv.tv_usec/1000)+(tv.tv_sec*1000))-
		((read_tv.tv_sec*1000)+(read_tv.tv_usec/1000) );
	return tott;
}

void Timer::stop(void) {
	running = false;
}

void Timer::reload(bool r) {
	mReload = r;
}

bool Timer::done(void) {
	
	struct timeval tv,ctv;
	gettimeofday(&tv,0);
	ctv.tv_usec = lapse_tv.tv_usec + read_tv.tv_usec;
	ctv.tv_sec = lapse_tv.tv_sec + read_tv.tv_sec;

	if(timercmp(&tv,&ctv,>=)) {
		stop();
		if(mReload)
			restart();
		return true;
	} else return false;
}

void Timer::restart(void) {
	start(lapse_tv);
}
