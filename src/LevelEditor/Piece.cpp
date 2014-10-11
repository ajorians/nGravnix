#include "Piece.h"
#include "Defines.h"
#include "PieceGraphics.h"
extern "C"
{
        #include "GravnixLib/GravnixLib.h"
}

PieceControl::PieceControl(SDL_Surface* pScreen, Metrics* pBoardMetrics, Selector* pSelector)
: m_pScreen(pScreen), m_pBoardMetrics(pBoardMetrics), m_pSelector(pSelector), m_pRoot(NULL)
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

bool PieceControl::CreatePiece(int nType, int nX, int nY)
{
   struct PieceSprite* pPiece = (PieceSprite*)malloc(sizeof(struct PieceSprite));
   pPiece->img = SDL_CreateRGBSurface(SDL_SWSURFACE, m_pBoardMetrics->GetPieceSize(), m_pBoardMetrics->GetPieceSize(), 16, 0, 0, 0, 0);

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

   SDL_SoftStretch(pSurface, NULL, pPiece->img, NULL);
   SDL_FreeSurface(pSurface);

   pPiece->replace = SDL_CreateRGBSurface(SDL_SWSURFACE, m_pBoardMetrics->GetPieceSize(), m_pBoardMetrics->GetPieceSize(), 16, 0, 0, 0, 0);

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
      if( pCurrent->boardX == nX && pCurrent->boardY == nY )
        break;
   }

   if( pCurrent ) {
      pCurrent->toFade = true;
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

bool PieceControl::IsAnimating() const
{
   PieceSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( IsPieceMoving(pCurrent) )
         return true;
      if( IsPieceFading(pCurrent, 10) )
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

void ShowPiece(SDL_Surface* pScreen, PieceSprite* pSprite, Metrics* pMetrics)
{
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
      MovePiece(m_pScreen, pCurrent, m_pBoardMetrics, 10);
      FadePiece(m_pScreen, pCurrent, 10, 10);
      pCurrent = pCurrent->next;
   }

   //Check for any dead
   PieceSprite* pPrevious = NULL;
   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      PieceSprite* pTemp = pCurrent;
      pCurrent = pCurrent->next;
      if( pTemp->toFade == true && pTemp->nFadeAmount >= 10 ) {
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

   m_pSelector->DrawSelector();
   SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      ResetBackground(m_pScreen, pCurrent, m_pBoardMetrics);
      pCurrent = pCurrent->next;
   }

   return true;
}



