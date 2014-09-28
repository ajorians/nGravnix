#include "Selector.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

#include "Defines.h"

#define SELECTOR_LINE_WIDTH     (4)
#define ANIMATION_STEPS		(4)

Selector::Selector(SDL_Surface* pScreen, Metrics* pMetrics)
: m_pScreen(pScreen), m_pBoardMetrics(pMetrics)
#ifdef ANIMATE_SELECTOR_MOVEMENT
, m_nPreviousX(0), m_nPreviousY(0), m_nAnimateStep(0)
#endif
, m_nCurrentX(0), m_nCurrentY(0)
{
}

int Difference(int a, int b)
{
   return Puz_Max(a-b, b-a);
}

void Selector::DrawSelector()
{
   int r=255, g=0, b=0, a=200;
#ifdef SELECTOR_COLOR_ANIMATES
   m_ColorIntensity.Animate();
   m_ColorIntensity.AdjustColor(r,g,b,a);
   //printf("r:%d, g:%d, b:%d, a:%d\n", r, g, b, a);
#endif

#ifdef ANIMATE_SELECTOR_MOVEMENT
   if( m_nPreviousX != m_nCurrentX || m_nPreviousY != m_nCurrentY ) {
      m_nAnimateStep++;
      if( m_nAnimateStep >= ANIMATION_STEPS ) {
         m_nPreviousX = m_nCurrentX;
         m_nPreviousY = m_nCurrentY;
         m_nAnimateStep = 0;
      }
   }
#endif

   int nX = 
#ifdef ANIMATE_SELECTOR_MOVEMENT
	m_nPreviousX;
#else
	m_nCurrentX;
#endif
   int nY = 
#ifdef ANIMATE_SELECTOR_MOVEMENT
	m_nPreviousY;
#else
	m_nCurrentY;
#endif

   int nExtraX =
#ifdef ANIMATE_SELECTOR_MOVEMENT
	m_nPreviousX != m_nCurrentX ? Difference(m_nPreviousX, m_nCurrentX)*m_pBoardMetrics->GetPieceSize()*m_nAnimateStep/ANIMATION_STEPS : 0;
   if( m_nPreviousX > m_nCurrentX ) nExtraX *= -1;
#else
	0;
#endif
	int nExtraY =
#ifdef ANIMATE_SELECTOR_MOVEMENT
        m_nPreviousY != m_nCurrentY ? Difference(m_nPreviousY, m_nCurrentY)*m_pBoardMetrics->GetPieceSize()*m_nAnimateStep/ANIMATION_STEPS : 0;
   if( m_nPreviousY > m_nCurrentY ) nExtraY *= -1;
#else
        0;
#endif


   //Top
   thickLineRGBA(m_pScreen, m_pBoardMetrics->GetXPos(nX) + nExtraX, m_pBoardMetrics->GetYPos(nY) + nExtraY, m_pBoardMetrics->GetXPos(nX) + m_pBoardMetrics->GetPieceSize() + nExtraX, m_pBoardMetrics->GetYPos(nY) + nExtraY, SELECTOR_LINE_WIDTH, r, g, b, a);
   //Left
   thickLineRGBA(m_pScreen, m_pBoardMetrics->GetXPos(nX) + nExtraX, m_pBoardMetrics->GetYPos(nY) + nExtraY, m_pBoardMetrics->GetXPos(nX) + nExtraX, m_pBoardMetrics->GetYPos(nY) + m_pBoardMetrics->GetPieceSize() + nExtraY, SELECTOR_LINE_WIDTH, r, g, b, a);
   //Bottom
   thickLineRGBA(m_pScreen, m_pBoardMetrics->GetXPos(nX) + nExtraX, m_pBoardMetrics->GetYPos(nY) + m_pBoardMetrics->GetPieceSize() + nExtraY, m_pBoardMetrics->GetXPos(nX) + m_pBoardMetrics->GetPieceSize() + nExtraX, m_pBoardMetrics->GetYPos(nY) + m_pBoardMetrics->GetPieceSize() + nExtraY, SELECTOR_LINE_WIDTH, r, g, b, a);
   //Right
   thickLineRGBA(m_pScreen, m_pBoardMetrics->GetXPos(nX) + m_pBoardMetrics->GetPieceSize() + nExtraX, m_pBoardMetrics->GetYPos(nY) + nExtraY, m_pBoardMetrics->GetXPos(nX) + m_pBoardMetrics->GetPieceSize() + nExtraX, m_pBoardMetrics->GetYPos(nY) + m_pBoardMetrics->GetPieceSize() + nExtraY, SELECTOR_LINE_WIDTH, r, g, b, a);
   //roundedRectangleRGBA(m_pScreen, nLeft + nX*nPieceSize, nTop + nY*nPieceSize, nLeft + nX*nPieceSize + nPieceSize, nTop + nY*nPieceSize + nPieceSize, 8, 0, 0, 255, 127);
}

void Selector::Move(Direction eDirection)
{
#ifdef ANIMATE_SELECTOR_MOVEMENT
   m_nPreviousX = m_nCurrentX;
   m_nPreviousY = m_nCurrentY;
   m_nAnimateStep = 0;
#endif

   if( eDirection == Up ) {
      if( m_nCurrentY > 0 ) m_nCurrentY--;
#ifdef MOVEMENT_WRAPS
      else if( m_nCurrentY == 0 ) m_nCurrentY = m_pBoardMetrics->GetHeight()-2;
#endif
   }
   else if( eDirection == Left ) {
      if( m_nCurrentX > 0 ) m_nCurrentX--;
#ifdef MOVEMENT_WRAPS
      else if( m_nCurrentX == 0 ) m_nCurrentX = m_pBoardMetrics->GetWidth()-1;
#endif
   }
   else if( eDirection == Right ) {
      if( m_nCurrentX < (m_pBoardMetrics->GetWidth()-1) ) m_nCurrentX++;
#ifdef MOVEMENT_WRAPS
      else if( m_nCurrentX == (m_pBoardMetrics->GetWidth()-1) ) m_nCurrentX = 0;
#endif
   }
   else if( eDirection == Down ) {
      if( m_nCurrentY < (m_pBoardMetrics->GetHeight()-1) ) m_nCurrentY++;
#ifdef MOVEMENT_WRAPS
      else if( m_nCurrentY == (m_pBoardMetrics->GetHeight()-1) ) m_nCurrentY = 0;
#endif
   }
}

int Selector::GetCurrentX() const
{
   return m_nCurrentX;
}

int Selector::GetCurrentY() const
{
   return m_nCurrentY;
}
