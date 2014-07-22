#ifndef HELP_H
#define HELP_H

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
}

class GravnixHelp
{
public:
	GravnixHelp(SDL_Surface* pScreen);
	~GravnixHelp();

	bool Loop();
	
protected:
	bool PollEvents();
	void UpdateDisplay();

protected:
	SDL_Surface	*m_pScreen;//Does not own
	nSDL_Font	*m_pFont;
	SDL_Surface	*m_pGlassGraphic;
        SDL_Surface     *m_pRainbowGraphic;
        SDL_Surface     *m_pBlackholeGraphic;
        SDL_Surface     *m_pStickyGraphic;
};

#endif
