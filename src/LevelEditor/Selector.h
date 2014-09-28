#ifndef SELECTOR_H
#define SELECTOR_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
}

#include "ColorIntensity.h"
#include "Metrics.h"
#include "Direction.h"

//#define MOVEMENT_WRAPS//When you go left from the left most you are on the right; same with all directions
//#define SELECTOR_COLOR_ANIMATES
#define ANIMATE_SELECTOR_MOVEMENT

class Selector
{
public:
   Selector(SDL_Surface* pScreen, Metrics* pMetrics);
   void DrawSelector();
   void Move(Direction eDirection);
   int GetCurrentX() const;
   int GetCurrentY() const;

protected:
   SDL_Surface	*m_pScreen;//Does not own
   Metrics      *m_pBoardMetrics;//Does NOT own
#ifdef ANIMATE_SELECTOR_MOVEMENT
   int		m_nPreviousX;
   int		m_nPreviousY;
   int		m_nAnimateStep;
#endif
   int		m_nCurrentX;
   int     	m_nCurrentY;
#ifdef SELECTOR_COLOR_ANIMATES
   ColorIntensity  m_ColorIntensity;
#endif
};

#endif

