#include "MovesRemainingLabel.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

#include "Defines.h"

MovesRemainingLabel::MovesRemainingLabel(SDL_Surface* pScreen, Metrics* pMetrics)
: m_pScreen(pScreen), m_pBoardMetrics(pMetrics)
{
   m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0, 0, 0);
}

MovesRemainingLabel::~MovesRemainingLabel()
{
   nSDL_FreeFont(m_pFont);
}

void MovesRemainingLabel::SetGravnixLib(GravnixLib* pGravnix)
{
   m_pGravnix = pGravnix;
}

void MovesRemainingLabel::DrawLabel()
{
   char buffer[32];
   char buf[8];
   strcpy(buffer, "Moves made: ");
   int nMovesMade = GetGravnixMovesMadeSoFar(*m_pGravnix);
   int nMovesTotal = GetGravnixMoveLimit(*m_pGravnix);
   Puz_itoa(nMovesMade, buf, 8);
   strcat(buffer, buf);
   strcat(buffer, " of ");
   Puz_itoa(nMovesTotal, buf, 8);
   strcat(buffer, buf);
   
   nSDL_DrawString(m_pScreen, m_pFont, m_pBoardMetrics->GetRight() - nSDL_GetStringWidth(m_pFont, buffer), m_pBoardMetrics->GetBottom() - nSDL_GetStringHeight(m_pFont, buffer), buffer);
}

