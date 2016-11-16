#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "audio.h"
#include "exceptions.h"

/* 
 * algumas coisas aqui tb peguei a ideia do site da libsdl.org 
 * */

/* Inicializa o funcionamento do hardware (placa de som) */
AudioHw::AudioHw(int r, int f, int c, int b)
{
	SDL_Init(SDL_INIT_AUDIO);
	AudioEnabled = Mix_OpenAudio(r,f,c,b);
	if(!AudioEnabled)
		throw audio_exception("impossible to initialize audio hardware");
}

AudioHw::~AudioHw()
{
	if(AudioEnabled) {
		Mix_CloseAudio();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		AudioEnabled = false;
	}
}

/* Um sample so carrega se o som tiver sido habilitado */
Sample::Sample(char *f)
{
	SoundChannel = -1;
	if(AudioEnabled)
	{
		SoundChunk = Mix_LoadWAV(f);
		if(!SoundChunk)
			throw audio_exception(std::string(f));
	}
}

/* toca e para o programa ate terminar */
void Sample::play_wait(void) {
	if( AudioEnabled == 1 && SoundChunk != NULL )
	{
		SoundChannel = Mix_PlayChannel( -1, SoundChunk, 0 );
		while(Mix_Playing(0))
			SDL_Delay(5);
	}
}

/* toca em background ate o fim do arquivo*/
void Sample::play(void)
{
	if( AudioEnabled && SoundChunk != NULL )
		SoundChannel = Mix_PlayChannel( -1, SoundChunk, 0 );
}

/* toca em background porem ate que stop seja chamado */
void Sample::play_loop(void)
{
	if(AudioEnabled && SoundChunk)
		SoundChannel = Mix_PlayChannel( -1, SoundChunk, -1 );
}

void Sample::stop(void)
{
	if(AudioEnabled && SoundChunk && SoundChannel != -1)
	{
		Mix_HaltChannel(SoundChannel);
		SoundChannel = -1;
	}
}

