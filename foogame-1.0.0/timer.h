#ifndef __TIMER_H__
#define __TIMER_H__
#include <sys/types.h>
#include <sys/time.h>

class Timer {
public:
	Timer();
	void start(long); //inicializa o timer para X milisegundos
	void start(struct timeval); //inicializa baseado em um timeval
	void stop(void); // para o timer
	bool done(void); // retorna verdadeiro se o timer tiver terminado a contagem
	void reload(bool); //seta se o timer deve se autocarregar quando estourar
	void restart(void); //reinicia o timer
	long int lapse(void); //retorna quanto tempo passou (em ms)
	bool is_running(void) {return running;} //retorna o estado do timer
private:
	bool running;
	struct timeval read_tv;
	struct timeval lapse_tv;
	bool mReload;
};

#endif
