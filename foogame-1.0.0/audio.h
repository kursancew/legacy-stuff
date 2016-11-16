#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <SDL/SDL_mixer.h>

namespace audio_consts {
	const static int DEFAULT_RATE = 44100;
	const static int DEFAULT_FORMAT = AUDIO_S16;
	const static int DEFAULT_CHANNELS = 1;
	const static int DEFAULT_BUFFERS = 4096;
}

static bool AudioEnabled = false;

class AudioHw {
public:
	AudioHw(int=audio_consts::DEFAULT_RATE,int=audio_consts::DEFAULT_FORMAT,
			int=audio_consts::DEFAULT_CHANNELS,int=audio_consts::DEFAULT_BUFFERS);
	~AudioHw(); 
};

class Sample
{
public:
	Sample(char*);
	void play(void);
	void play_wait(void);
	void play_loop(void);
	void stop(void);

private:
	Mix_Chunk *SoundChunk;
	int SoundChannel;
};
#endif
