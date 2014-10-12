#include "Background.h"
#include "GameBackground.h"

#define GRAPHIC_WIDTH	(427)

Background::Background(SDL_Surface* pScreen)
: m_pScreen(pScreen), m_nX(0)
{
   m_pBackground = nSDL_LoadImage(image_Squares_Background);
}

Background::~Background()
{
   SDL_FreeSurface(m_pBackground);
}

void Background::DrawBackground()
{
   if( !is_classic ) {
       SDL_Rect rectSrc, rectDst, rectDst2;
       rectSrc.w = GRAPHIC_WIDTH;
       rectSrc.h = 240;
       rectSrc.x = 0;
       rectSrc.y = 0;

       rectDst.w = GRAPHIC_WIDTH;
       rectDst.h = 240;
       rectDst.x = m_nX;
       rectDst.y = 0;

       rectDst2.w = GRAPHIC_WIDTH;
       rectDst2.h = 240;
       rectDst2.x = GRAPHIC_WIDTH + m_nX;
       rectDst2.y = 0;

       SDL_BlitSurface(m_pBackground, &rectSrc, m_pScreen, &rectDst);
       SDL_BlitSurface(m_pBackground, &rectSrc, m_pScreen, &rectDst2);

       if( true/*m_pConfig->GetBackgroundMoves()*/ ) {
          m_nX--;
          if( (m_nX + GRAPHIC_WIDTH) <= 0 )
             m_nX = 0;
       }
   } else {
       SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 255, 255, 255));
   }
}

 
