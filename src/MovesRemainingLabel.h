#ifndef MOVESREMAINING_H
#define MOVESREMAINING_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
	#include "GravnixLib/GravnixLib.h"
}

#include "Metrics.h"

class MovesRemainingLabel
{
public:
   MovesRemainingLabel(SDL_Surface* pScreen, Metrics* pMetrics);
   ~MovesRemainingLabel();

   void SetGravnixLib(GravnixLib* pGravnix);
  
   void DrawLabel();

protected:
   SDL_Surface	*m_pScreen;//Does NOT own
   Metrics      *m_pBoardMetrics;//Does NOT own
   nSDL_Font	*m_pFont;
   GravnixLib *m_pGravnix;//Does NOT own
};

#endif

