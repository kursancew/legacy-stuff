#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <SDL/SDL.h>
#include <string>
#include <vector>
struct SpriteFrame
{
	SDL_Surface *image;
	int pause;
};

class SpriteBase
{
public:	
	SpriteBase(char*);
	SpriteBase(SDL_Surface*);
	~SpriteBase();

	SpriteFrame *mAnim;
	int mBuilt, mNumframes, mW, mH;
};


class Sprite
{
public:
	~Sprite();
	Sprite(SpriteBase *base, SDL_Surface*, SDL_Surface *back);
	Sprite(SDL_Surface *base, SDL_Surface*, SDL_Surface *back);
	Sprite(char *file,SDL_Surface *screen, SDL_Surface *back);
	void ChangeBase(SpriteBase*);
	void draw(bool=false);
	void draw(SDL_Surface*);
	void remove(void);
	void nd(void) { mDrawn = false; }
	
	void setFrame(int nr) { mFrame = nr; }
	int getFrame(void) { return mFrame; }
	
	void setSpeed(float nr) { mSpeed = nr; }
	float getSpeed(void) { return mSpeed; }
	
	void toggleAnim(void) { mAnimating = !mAnimating; }
	void startAnim(void) { mAnimating = true; }
	void stopAnim(void) { mAnimating = false; }
	void rewind(void) { mFrame = 0; }
	void set(int xx, int yy) { mX=xx; mY=yy; }
	void set(std::string);

	void setBaseOwner(bool=true);
	
	float getx(void) {return mX;}
	float gety(void) {return mY;}
	
	int getw() {return mSpriteBase->mAnim[0].image->w;}
	int geth() {return mSpriteBase->mAnim[0].image->h;}
	SDL_Surface *getSurface(void) { return mSpriteBase->mAnim[mFrame].image; }
	SDL_Surface *getSurface(int f) { return mSpriteBase->mAnim[f].image; }
private:
	int mFrame;
	int mX, mY;
	bool mAnimating;
	bool mDrawn;
	float mSpeed;
	long mLastupdate;
	SpriteBase *mSpriteBase;
	SDL_Surface *mScreen;
	SDL_Surface *mBackGround;
	SDL_Rect mLastpos;
	bool owns_the_base;
};

#endif
