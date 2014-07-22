#ifndef BACKGROUND_H
#define BACKGROUND_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
}

class Config;

class Background
{
public:
   Background(SDL_Surface* pScreen, Config* pConfig);
   ~Background();
   void DrawBackground();

protected:
        SDL_Surface     *m_pScreen;//Does not own
	Config		*m_pConfig;//Does not own
	SDL_Surface	*m_pBackground;
	int m_nX;//427 width
};

#endif

