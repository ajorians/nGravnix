#include "Piece.h"
#include "Defines.h"
#include "PieceGraphics.h"

PieceControl::PieceControl(SDL_Surface* pScreen, Metrics* pBoardMetrics, MovesRemainingLabel* pLabel, Selector* pSelector)
: m_pScreen(pScreen), m_pBoardMetrics(pBoardMetrics), m_pMovesLabel(pLabel), m_pSelector(pSelector), m_pRoot(NULL)
{
}

PieceControl::~PieceControl()
{
   ClearPieces();
}

void PieceControl::ClearPieces()
{
   PieceSprite* pPiece = m_pRoot;
   while(pPiece != NULL) {
      PieceSprite* pTemp = pPiece;
      SDL_FreeSurface(pPiece->img);
      SDL_FreeSurface(pPiece->replace);
      pPiece = pPiece->next;
      free(pTemp);
   }
   m_pRoot = NULL;
}

void LoadPieceTypeImage(SDL_Surface* pImg, int nType)
{
#ifndef PIECE_DRAW_COLOR
   SDL_Surface* pSurface;
   switch(nType) {
      //default:
      case BOARD_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_Block);
      break;
      default:
      case RED_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_RedBlock);
      break;
      case BLUE_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_CyanBlock);
      break;
      case PURPLE_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_PurpleBlock);
      break;
      case ORANGE_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_OrangeBlock);
      break;
      case YELLOW_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_YellowBlock);
      break;
      case GREEN_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_GreenBlock);
      break;
      case GLASS_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_GlassBlock);
      break;
      case RAINBOW_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_RainbowBlock);
      break;
      case BLACKHOLE_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_BlackHole);
      break;
      case STICKY_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_StickyBlock);
      break;
      case STUCK_GLASS_BLOCK_VALUE:
         pSurface = nSDL_LoadImage(image_Gravnix_StuckGlass);
      break;
   }

   SDL_SoftStretch(pSurface, NULL, pImg, NULL);
   SDL_FreeSurface(pSurface);

#else
   Uint32 clr;
   switch(nType) {
      default:
      case BOARD_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 139, 137, 137);
      break;
      case RED_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 255, 0, 0);
      break;
      case BLUE_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 0, 255, 255);
      break;
      case PURPLE_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 153, 50, 204);
      break;
      case ORANGE_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 255, 165, 0);
      break;
      case YELLOW_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 255, 255, 0);
      break;
      case GREEN_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 0, 255, 0);
      break;
      case GLASS_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 205, 201, 201);
      break;
      case RAINBOW_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 123, 123, 123);
      break;
      case BLACKHOLE_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 0, 0, 0);
      break;
      case STICKY_BLOCK_VALUE:
         clr = SDL_MapRGB(m_pScreen->format, 250, 128, 114);
      break;
   }

   SDL_FillRect(pImg, NULL, clr);
#endif
}

//#define PIECE_DRAW_COLOR

bool PieceControl::CreatePiece(int nType, int nX, int nY)
{
   struct PieceSprite* pPiece = (PieceSprite*)malloc(sizeof(struct PieceSprite));

   pPiece->img = SDL_CreateRGBSurface(SDL_SWSURFACE, m_pBoardMetrics->GetPieceSize(), m_pBoardMetrics->GetPieceSize(), 16, 0, 0, 0, 0);

   LoadPieceTypeImage(pPiece->img, nType);

   pPiece->replace = SDL_CreateRGBSurface(SDL_SWSURFACE, m_pBoardMetrics->GetPieceSize(), m_pBoardMetrics->GetPieceSize(), 16, 0, 0, 0, 0);
   pPiece->nPieceType = nType;

   pPiece->boardX = nX;
   pPiece->boardY = nY;
#ifdef PIECE_ANIMATE_RANDOM_START
   pPiece->x = rand() % 320;//I do random start position; I suppose it is alright :)
   pPiece->y = rand() % 240;
#else
   pPiece->x = m_pBoardMetrics->GetXPos(nX);
   pPiece->y = m_pBoardMetrics->GetYPos(nY);
#endif
   pPiece->toX = m_pBoardMetrics->GetXPos(nX);
   pPiece->toY = m_pBoardMetrics->GetYPos(nY);
   pPiece->toFade = false;
   pPiece->toShrink = false;
   pPiece->visible = true;
   pPiece->nFadeAmount = 0;

   pPiece->next = m_pRoot;
   m_pRoot = pPiece;

   return true;
}

bool PieceControl::DropPiece(int nX, int nY, Direction eDirection)
{
   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( pCurrent->boardX == nX && pCurrent->boardY == nY )
        break;
   }

   if( pCurrent ) {
      int nOffsetX = 0, nOffsetY = 0;
      if( eDirection == Up )
         nOffsetY = -1;
      else if( eDirection == Left )
         nOffsetX = -1;
      else if( eDirection == Right )
         nOffsetX = 1;
      else if( eDirection == Down )
         nOffsetY = 1;
      pCurrent->boardX = nX+nOffsetX;
      pCurrent->boardY = nY+nOffsetY;
      pCurrent->toX = m_pBoardMetrics->GetXPos(nX+nOffsetX);
      pCurrent->toY = m_pBoardMetrics->GetYPos(nY+nOffsetY);
   }

   return true;
}

bool PieceControl::DisappearPiece(int nX, int nY)
{
   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( pCurrent->boardX == nX && pCurrent->boardY == nY && pCurrent->visible == true )
        break;
   }

   if( pCurrent ) {
      pCurrent->toFade = true;
   }

   return true;
}

bool PieceControl::HidePiece(int nX, int nY, int nType, bool bHide)
{
   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( pCurrent->boardX == nX && pCurrent->boardY == nY && pCurrent->nPieceType == nType )
        break;
   }

   if( pCurrent ) {
      pCurrent->visible = !bHide;
   }

   return true;
}

bool PieceControl::ChangePiece(int nX, int nY, int nType, int nNewType)
{
   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( pCurrent->boardX == nX && pCurrent->boardY == nY && pCurrent->nPieceType == nType )
        break;
   }

   if( pCurrent ) {
      pCurrent->nPieceType = nNewType;
      SDL_FreeSurface(pCurrent->img);
      pCurrent->img = SDL_CreateRGBSurface(SDL_SWSURFACE, m_pBoardMetrics->GetPieceSize(), m_pBoardMetrics->GetPieceSize(), 16, 0, 0, 0, 0);
      LoadPieceTypeImage(pCurrent->img, nNewType);
   }

   return true;
}

bool PieceControl::ShrinkPiece(int nX, int nY, int nType)
{
   PieceSprite* pPrev = NULL;
   for(PieceSprite* pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( pCurrent->boardX == nX && pCurrent->boardY == nY ) {
         if( pCurrent->nPieceType == nType ) {
            SDL_FreeSurface(pCurrent->img);
            SDL_FreeSurface(pCurrent->replace);
            if( pPrev ) {
               pPrev->next = pCurrent->next;
            } else {
               m_pRoot = pCurrent->next;
            }
            free(pCurrent);
            break;
         }
      }
      pPrev = pCurrent;
   }

   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( pCurrent->boardX == nX && pCurrent->boardY == nY ) {
         pCurrent->toShrink = true;
      }
   }

   return true;
}

bool IsPieceMoving(PieceSprite* pSprite)
{
   return (pSprite->y != pSprite->toY) || (pSprite->x != pSprite->toX);
}

bool IsPieceFading(PieceSprite* pSprite, int nFadeLimit)
{
   return (pSprite->toFade == true && pSprite->nFadeAmount < nFadeLimit);
}

bool IsPieceShrinking(PieceSprite* pSprite, int nShrinkLimit)
{
   return (pSprite->toShrink == true && pSprite->nFadeAmount < nShrinkLimit);//Maybe rename nFadeAmount?
}

bool PieceControl::IsAnimating() const
{
   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( IsPieceMoving(pCurrent) )
         return true;
      if( IsPieceFading(pCurrent, 10/*m_pConfig->GetFadeSteps()*/) )
         return true;
      if( IsPieceShrinking(pCurrent, 10/*m_pConfig->GetFadeSteps()*/) )//maybe new config item
         return true;
   }

   return false;
}

void SetBackground(SDL_Surface* pScreen, PieceSprite* pSprite, Metrics* pMetrics)
{
   SDL_Rect rect;
   rect.x = pSprite->x;
   rect.y = pSprite->y;
   rect.w = pMetrics->GetPieceSize();
   rect.h = pMetrics->GetPieceSize();

   SDL_BlitSurface(pScreen, &rect, pSprite->replace, NULL);
}

void MovePiece(SDL_Surface* pScreen, PieceSprite* pSprite, Metrics* pMetrics, int nMoveStepAmount)
{
   if( (pSprite->x != pSprite->toX) || (pSprite->y != pSprite->toY) ) {
      //int nDistX = pSprite->toX - pSprite->x;
      //int nDistY = pSprite->toY - pSprite->y;

      for(int n=0; n<nMoveStepAmount; n++) {
         if( pSprite->x == pSprite->toX )
            break;
         if( pSprite->x < pSprite->toX ) {
            pSprite->x++;
         }
         if( pSprite->x > pSprite->toX ) {
            pSprite->x--;
         }
      }

      for(int n=0; n<nMoveStepAmount; n++) {
         if( pSprite->y == pSprite->toY )
            break;
         if( pSprite->y < pSprite->toY ) {
            pSprite->y++;
         }
         if( pSprite->y > pSprite->toY ) {
            pSprite->y--;
         }
      }
   }

   SetBackground(pScreen, pSprite, pMetrics);
}

void Brighten(SDL_Surface* pSurface, int nBrightenAmount)
{
   //If the surface must be locked 
   if( SDL_MUSTLOCK( pSurface ) )
   {
      //Lock the surface 
      SDL_LockSurface( pSurface );
   }

   //Go through columns 
   for( int x = 0; x < pSurface->w; x++ )
   {
      //Go through rows 
      for( int y = 0; y < pSurface->h; y++ )
      {
         Uint32 pixel = nSDL_GetPixel(pSurface, x, y);
         Uint8 r = 0, g = 0, b = 0;
         SDL_GetRGB(pixel, pSurface->format, &r, &g, &b);
         r = Puz_Min(255, r+nBrightenAmount);
         g = Puz_Min(255, g+nBrightenAmount);
         b = Puz_Min(255, b+nBrightenAmount);

         nSDL_SetPixel(pSurface, x, y, SDL_MapRGB(pSurface->format, r, g, b));
      }
   }

   //Unlock surface 
   if( SDL_MUSTLOCK( pSurface ) )
   {
      SDL_UnlockSurface( pSurface );
   }
}

void FadePiece(SDL_Surface* pScreen, PieceSprite* pSprite, int nFadeLimit, int nBrightenAmount)
{
   if( pSprite->toFade == false )
      return;

   if( pSprite->nFadeAmount < nFadeLimit )
      pSprite->nFadeAmount++;

   Brighten(pSprite->img, nBrightenAmount);
}

void ShrinkPieceSprite(SDL_Surface* pScreen, PieceSprite* pSprite, int nShrinkLimit)
{
   if( pSprite->toShrink == false )
      return;

   if( pSprite->nFadeAmount < nShrinkLimit )
      pSprite->nFadeAmount++;

   int nPieceSize = pSprite->img->w;
   int nPixelIn = pSprite->nFadeAmount;
   if( nPixelIn > (nPieceSize/2) )
      return;

   SDL_Surface* pSurface = pSprite->img;
   Uint32 clr = SDL_MapRGB(pSurface->format, 255, 255, 255);

   //If the surface must be locked 
   if( SDL_MUSTLOCK( pSurface ) )
   {
      //Lock the surface 
      SDL_LockSurface( pSurface );
   }

   for( int x = 0; x < pSurface->w; x++ )
   {
      nSDL_SetPixel(pSurface, x, nPixelIn-1, clr);
      nSDL_SetPixel(pSurface, x, pSurface->w-nPixelIn, clr);
   }
   for( int y = 0; y < pSurface->h; y++ )
   {
      nSDL_SetPixel(pSurface, nPixelIn-1, y, clr);
      nSDL_SetPixel(pSurface, pSurface->h-nPixelIn, y, clr);
   }

   //Unlock surface 
   if( SDL_MUSTLOCK( pSurface ) )
   {
      SDL_UnlockSurface( pSurface );
   }
}

void ShowPiece(SDL_Surface* pScreen, PieceSprite* pSprite, Metrics* pMetrics)
{
   if( pSprite->visible == false )
      return;

   SDL_Rect rect;
   rect.x = pSprite->x;
   rect.y = pSprite->y;
   rect.w = pMetrics->GetPieceSize();
   rect.h = pMetrics->GetPieceSize();

   SDL_BlitSurface(pSprite->img, NULL, pScreen, &rect);
}

void ResetBackground(SDL_Surface* pScreen, PieceSprite* pSprite, Metrics* pMetrics)
{
   SDL_Rect rect;
   rect.x = pSprite->x;
   rect.y = pSprite->y;
   rect.w = pMetrics->GetPieceSize();
   rect.h = pMetrics->GetPieceSize();

   SDL_BlitSurface(pSprite->replace, NULL, pScreen, &rect);
}

bool PieceControl::Animate()
{
   //if( !IsAnimating() ) {
      //return false;
   //}

   PieceSprite* pCurrent = m_pRoot;

   while(pCurrent != NULL) {
      MovePiece(m_pScreen, pCurrent, m_pBoardMetrics, 10/*m_pConfig->GetPieceMovePerStep()*/);
      FadePiece(m_pScreen, pCurrent, 10/*m_pConfig->GetFadeSteps()*/, 10/*m_pConfig->GetBrightenPerStep()*/);
      ShrinkPieceSprite(m_pScreen, pCurrent, 10/*m_pConfig->GetFadeSteps()*/);
      pCurrent = pCurrent->next;
   }

   //Check for any dead
   PieceSprite* pPrevious = NULL;
   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      PieceSprite* pTemp = pCurrent;
      pCurrent = pCurrent->next;
      if( (pTemp->toFade == true || pTemp->toShrink) && pTemp->nFadeAmount >= 10/*m_pConfig->GetFadeSteps()*/ ) {
         SDL_FreeSurface(pTemp->img);
         SDL_FreeSurface(pTemp->replace);
         if( pPrevious != NULL ) {
            pPrevious->next = pTemp->next;
         }
         else {
            m_pRoot = pTemp->next;
         }
         free(pTemp);
      }
      else
      {
         pPrevious = pTemp;
      }
   }

   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      ShowPiece(m_pScreen, pCurrent, m_pBoardMetrics);
      pCurrent = pCurrent->next;
   }

   if( m_pMovesLabel ) {
      m_pMovesLabel->DrawLabel();
   }
   if( m_pSelector ) {
      m_pSelector->DrawSelector();
      SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   }
   SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      ResetBackground(m_pScreen, pCurrent, m_pBoardMetrics);
      pCurrent = pCurrent->next;
   }

   return true;
}



