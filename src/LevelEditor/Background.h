#ifndef BACKGROUND_H
#define BACKGROUND_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
}

class Background
{
public:
   Background(SDL_Surface* pScreen);
   ~Background();
   void DrawBackground();

protected:
        SDL_Surface     *m_pScreen;//Does not own
	SDL_Surface	*m_pBackground;
	int m_nX;//427 width
};

#endif

