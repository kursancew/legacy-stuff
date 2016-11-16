#include <cstring>
#include <cstdlib>
#include <SDL/SDL_image.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "exceptions.h"
#include "graph.h"

/*
 * uma boa contribuicao para este programa foi retirado de um link
 * em libsdl.org, no momento nao me lembro do autor...
 */

/*inicializa uma sprite a partir de uma SDL_Surface 
 * base = superficie da sprite
 * screen = onde vai ser blitada
 * back = o que vai no lugar se ela quando remove() eh chamado
 * */

Sprite::Sprite(SDL_Surface *base, SDL_Surface *screen, SDL_Surface *back) {
	mSpriteBase = new SpriteBase(base);
	mBackGround = back;
	if(mSpriteBase->mBuilt)
	{
		mAnimating = false;
	}
	mScreen = screen;
	mAnimating = mFrame = 0;
	mX = mY = 0;
	mLastpos.y = int(mY);mLastpos.x = int(mX);
	owns_the_base = true;
	mSpeed = 1;
}

/*
 * se a sprite for controladora da base ela ira destruir
 * a base quando seu destrutor for chamado
 *
 * use com cuidado!
 */
void Sprite::setBaseOwner(bool b)
{
	owns_the_base = b;
}

/*
 * utiliza um arquivo descricao de posicoes para modificar
 * a posicao de uma sprite, o formato para requerer uma posicao eh
 * o->set("file:nome_pos");
 *
 * o formato do arquivo eh
 * nome X Y
 *
 * EX:
 *
 * casa 10 400
 * bar 100 40
 * bola 300 440
 * pessoa 40 410
 */
void Sprite::set(std::string s)
{
	std::string desc = s.substr(s.find_first_of(":")+1);
	std::string fname = s.substr(0,s.find_first_of(":"));
	FILE* fd = fopen(fname.c_str(),"rw");
	if(!fd) {
		throw graph_exception(s.c_str());
	} else {
		bool changed=false;
		while(!feof(fd)) {
			char buf[255];
			char descb[32];
			int xp,yp;
			fgets(buf,255,fd);
			if(buf[0] == '#' || buf[0] == '\r' || buf[0] == '\0' || buf[0] == '\n' )
				continue;
			sscanf(buf,"%s %d %d",descb,&xp,&yp);
			if(!strcmp(desc.c_str(),descb)) {
				set(xp,yp);
				changed = true;
				break;
			}
		}
		if(!changed)
			throw("Descricao nao encontrada em arquivo");
	}
}

/*inicializa uma sprite a partir de um PNG
 * base = superficie da sprite
 * screen = onde vai ser blitada
 * back = o que vai no lugar se ela quando remove() eh chamado
 * */
Sprite::Sprite(char *file,SDL_Surface *screen, SDL_Surface *back)
{
	mSpriteBase = new SpriteBase(file);
	mBackGround = back;
	if(mSpriteBase->mBuilt)
	{
		mAnimating = false;
	}
	mScreen = screen;
	mAnimating = mFrame = 0;
	mX = mY = 0;
	mLastpos.y = int(mY);mLastpos.x = int(mX);
	owns_the_base = true;
	mSpeed = 1;
}

/* troca a base da sprite */
void Sprite::ChangeBase(SpriteBase *new_base) {
	if(!new_base)
		throw graph_exception("bad SpriteBase pointer");

	if(owns_the_base && mSpriteBase)
		delete mSpriteBase;

	owns_the_base = false;
	mSpriteBase = new_base;
}

/*
 * inicializa uma sprite a partir de um SpriteBase
 * base = superficie da sprite
 * screen = onde vai ser blitada
 * back = o que vai no lugar se ela quando remove() eh chamado
 * */

Sprite::Sprite(SpriteBase *base, SDL_Surface *screen, SDL_Surface *back)
{
	mSpriteBase = base;
	mBackGround = back;
	if(mSpriteBase->mBuilt)
	{
		if(mSpriteBase->mNumframes>1) mAnimating = true;
		mLastupdate = SDL_GetTicks();
	}
	mScreen = screen;
	mX = mY = mFrame = 0;
	mLastpos.y = int(mY);mLastpos.x = int(mX);
	owns_the_base = false;
	mSpeed = 1;
}

Sprite::~Sprite() {
	if(owns_the_base && mSpriteBase)
		delete mSpriteBase;
}

/* remove a sprite da tela, colocando o backreplacement */
void Sprite::remove(void) {

	if(mAnimating && mDrawn)
	{
		if(mLastupdate+mSpriteBase->mAnim[mFrame].pause*mSpeed < 
				SDL_GetTicks())
		{
			mLastpos.w = getw();
			mLastpos.h = geth();
			if(mBackGround)
				SDL_BlitSurface(mBackGround,&mLastpos,mScreen,&mLastpos);
			mDrawn = false;
		}
	} else {
		if(mDrawn) {
			mLastpos.w = getw();
			mLastpos.h = geth();
			if(mBackGround)
				SDL_BlitSurface(mBackGround,&mLastpos,mScreen,&mLastpos);
			mDrawn = false;
		}
	}
}

/* desenha a sprite 
 * tgt = onde a sprite vai ser desenhada
 * */
void Sprite::draw(SDL_Surface *tgt)
{
	if(!tgt)
		throw graph_exception("bad surface pointer");
	if(mAnimating)
	{
		if(mLastupdate+mSpriteBase->mAnim[mFrame].pause*mSpeed < 
				SDL_GetTicks())
		{
			mFrame++;
			if(mFrame>mSpriteBase->mNumframes-1) mFrame=0;
			mLastupdate = SDL_GetTicks();
			mDrawn = true;
			SDL_Rect r;
			r.x = (int)mX; r.y = (int)mY;
			SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, 
				NULL, tgt, &r);
		}
	} else {
		mDrawn = true;
		SDL_Rect r;
		r.x = (int)mX; r.y = (int)mY;
		SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, NULL, tgt, &r);
	}
}

/*
 * Desenha, se force for true a imagem é desenhada mesmo 
 * que nao esteja na hora
 */
void Sprite::draw(bool force)
{
	if(mAnimating)
	{
		if(mLastupdate+mSpriteBase->mAnim[mFrame].pause*mSpeed < 
				SDL_GetTicks())
		{
			mFrame++;
			if(mFrame>mSpriteBase->mNumframes-1) mFrame=0;
			mLastupdate = SDL_GetTicks();
			mDrawn = true;
			mLastpos.x = (int)mX;
			mLastpos.y = (int)mY;
	
			SDL_Rect r;
			r.x = (int)mX; r.y = (int)mY;
			SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, 
				NULL, mScreen, &r);
		}
		else if(force) {
			SDL_Rect r;
			r.x = (int)mX; r.y = (int)mY;
			SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, 
				NULL, mScreen, &r);
		}
	} else {

		mDrawn = true;
		mLastpos.x = (int)mX;
		mLastpos.y = (int)mY;
	
		SDL_Rect r;
		r.x = (int)mX; r.y = (int)mY;
		SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, NULL, mScreen, &r);
	}
}

SpriteBase::~SpriteBase() {
	for(int i = 0; i < mNumframes; ++i)
		SDL_FreeSurface(mAnim[i].image);
	if(mAnim)
		delete mAnim;
	mAnim = 0;
}

//cria uma base a partir de um Surface e realoca ele dentro de si
SpriteBase::SpriteBase(SDL_Surface *base) {
	mNumframes = 1;
	mAnim = new SpriteFrame[mNumframes];
	//SDL_SetAlpha(base,SDL_SRCALPHA,0);
	mAnim[0].image = SDL_DisplayFormatAlpha(base);
	mAnim[0].pause = 0;
	mW = mAnim[0].image->w; 
	mH = mAnim[0].image->h;
}

/* cria uma base de sprite a partir de um arquivo INFO ou uma imagem
 * dir: diretorio onde se encontra o INFO ou caminho para arquivo de imagem
 *
 * formato arquivo INFO:
 *
 * FILES: numero_de_arqs
 * 1.png ms_entre_frames ckey_r ckey_g ckey_b
 * */
SpriteBase::SpriteBase(char *dir)
{
	char buffer[255];
	char filename[255];
	char name[255];
	int pause=0, r=0, g=0, b=0;
	FILE *fp;
	struct stat s;
	stat(dir,&s);
	if(s.st_mode & S_IFDIR) {
		mNumframes = 1;
		mAnim = new SpriteFrame[mNumframes];
		SDL_Surface *temp;
		if((temp = IMG_Load(dir)) == NULL) 
			throw graph_exception("error while trying" + std::string(filename));
		//SDL_SetAlpha(temp,SDL_SRCALPHA,0);

		mAnim[0].image = SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);

		mAnim[0].pause = pause;
		mW = mAnim[0].image->w; 
		mH = mAnim[0].image->h;

	} else {
		sprintf(filename, "%s/info", dir);
		if((fp=fopen(filename, "r")) == NULL) {
			std::string tbuf;
			tbuf = std::string("ERRO abrindo ") + std::string(filename);
			throw graph_exception(tbuf);
			exit(-1);
		}
		fgets(buffer, 255, fp);
		sscanf(buffer, "FILES: %d", &mNumframes);
		mAnim = new SpriteFrame[mNumframes];
		mBuilt = 1;
		mW = mH = 0;
		int count = 0;
		while(!feof(fp) && count<mNumframes) {
			fgets(buffer, 255, fp);
			if(	buffer[0] != '#' && buffer[0] != '\r' &&
				buffer[0] != '\0' && buffer[0] != '\n' && strlen(buffer) > 9)
			{

				sscanf(buffer, "%s %d %d %d %d", 
						name, &pause, &r, &g, &b);

				if (pause < 0 || r < 0 || g < 0 || b < 0)
					throw graph_exception("parse error on info file");
				
				sprintf(filename, "%s/%s", dir, name);
				SDL_Surface *temp;
				if((temp = IMG_Load(filename)) == NULL) {
					throw graph_exception(std::string(filename));
				}
				/* tenho q ver como fazer como utilizar colorkeys
				 * apenas para gifs... tenho q ver se tb eh necessario
				 * por enquanto as 3 cores sao ignoradas
				 * if(r >= 0) SDL_SetColorKey
					(temp, SDL_SRCCOLORKEY, 
				 	SDL_MapRGB(temp->format, r, g, b));*/
		
				mAnim[count].image = SDL_DisplayFormatAlpha(temp);
				SDL_FreeSurface(temp);

				mAnim[count].pause = pause;
				if(!mW) mW = mAnim[count].image->w; 
				if(!mH) mH = mAnim[count].image->h;
				++count;
			}
		}
		fclose(fp);
	}
}

