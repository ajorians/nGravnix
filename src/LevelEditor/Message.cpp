#include "Message.h"
#include "Defines.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

#define MESSAGE_SPRITE_WIDTH	(20)
#define MESSAGE_SPRITE_HEIGHT	(20)
#define MESSAGE_LINE_SPACING	(10)
#define MESSAGE_SPRITE_SPACING	(10)
#define MESSAGE_STEP_AMOUNT	(7)
#define MESSAGE_STAY_DURATION	(20)

#define MESSAGE_FONT		(NSDL_FONT_THIN)//(NSDL_FONT_VGA)

Message::Message(SDL_Surface* pScreen)
: m_pScreen(pScreen), m_pRoot(NULL), m_nStayDuration(MESSAGE_STAY_DURATION), m_bFinishFast(false)
{
   m_pFont = nSDL_LoadFont(MESSAGE_FONT, MESSAGE_TEXT_R, MESSAGE_TEXT_G, MESSAGE_TEXT_B);
}

Message::~Message()
{
   ClearMessage();
   nSDL_FreeFont(m_pFont);
}

void Message::ClearMessage()
{
   LetterSprite* pLetter = m_pRoot;
   while(pLetter != NULL) {
      LetterSprite* pTemp = pLetter;
      SDL_FreeSurface(pLetter->img);
      SDL_FreeSurface(pLetter->replace);
      pLetter = pLetter->next;
      free(pTemp);
   }
   m_pRoot = NULL;
}

int GetLettersInLine(const char* pstrMessage)
{
   int nCount = 0;
   const char* pstr = pstrMessage;
   while(*pstr != '\0') {
      if( *pstr == '\n' )
         return nCount;
      pstr = pstr+1;
      nCount++;
   }
   return nCount;
}

int GetStartXForBonusMessage(int nLettersInLine)
{
   int nLengthForLine = (MESSAGE_SPRITE_WIDTH + MESSAGE_SPRITE_SPACING)*nLettersInLine;   

   nLengthForLine -= MESSAGE_SPRITE_SPACING;//So flush with last letter!

   int nX = (SCREEN_WIDTH - nLengthForLine)/2;
   return nX;
}

void Message::CreateMessage(const char* pstrMessage)
{
   ClearMessage();

   int nNumLines = 1;
   for(int n=0; n<strlen(pstrMessage); n++){
      if( *(pstrMessage+n) == '\n' )
         nNumLines++;
   }

   int nLettersInLine = GetLettersInLine(pstrMessage);
   int nX = GetStartXForBonusMessage(nLettersInLine);
   int nY = (SCREEN_HEIGHT - (nNumLines*(MESSAGE_SPRITE_HEIGHT+MESSAGE_LINE_SPACING)))/2;
   int nLettersIntoLine = 0;

   LetterSprite* pPrevious = NULL;
   for(int n=0; n<strlen(pstrMessage); n++){
      LetterSprite* pSprite = (LetterSprite*)malloc(sizeof(LetterSprite));

      pSprite->img = SDL_CreateRGBSurface(SDL_SWSURFACE, MESSAGE_SPRITE_WIDTH, MESSAGE_SPRITE_HEIGHT, 16, 0, 0, 0, 0);

      SDL_FillRect(pSprite->img, NULL, SDL_MapRGB(m_pScreen->format, MESSAGE_BACKGROUND_R, MESSAGE_BACKGROUND_G, MESSAGE_BACKGROUND_B));

#ifdef MESSAGE_DRAW_BORDER
      vlineRGBA(pSprite->img, 0, 			0, MESSAGE_SPRITE_HEIGHT-1, 	0, 			0, 0, 255);
      vlineRGBA(pSprite->img, MESSAGE_SPRITE_WIDTH-1, 	0, MESSAGE_SPRITE_HEIGHT-1, 	0, 			0, 0, 255);
      hlineRGBA(pSprite->img, 0, 			MESSAGE_SPRITE_WIDTH-1, 	0, 			0, 0, 0, 255);
      hlineRGBA(pSprite->img, 0, 			MESSAGE_SPRITE_WIDTH-1, 	MESSAGE_SPRITE_HEIGHT-1,0, 0, 0, 255);
#endif

      char ch = *(pstrMessage+n);
      if( ch == '\n' ) {
         nLettersInLine = GetLettersInLine(pstrMessage+n+1/*Get past newline*/);
         nX = GetStartXForBonusMessage(nLettersInLine);
         nLettersIntoLine = 0;
         nY += (MESSAGE_SPRITE_HEIGHT+MESSAGE_LINE_SPACING);
         continue;
      }
      if( ch == ' ' ) {
         nLettersIntoLine++;
         continue;
      }

      char buffer[8];
      buffer[0] = ch;
      buffer[1] = '\0';

      nSDL_DrawString(pSprite->img, m_pFont, (MESSAGE_SPRITE_WIDTH-nSDL_GetStringWidth(m_pFont, buffer))/2, (MESSAGE_SPRITE_HEIGHT-nSDL_GetStringHeight(m_pFont,buffer))/2, buffer);
      pSprite->replace = SDL_CreateRGBSurface(SDL_SWSURFACE, MESSAGE_SPRITE_WIDTH, MESSAGE_SPRITE_HEIGHT, 16, 0, 0, 0, 0);

      pSprite->ch = ch;

      pSprite->x = rand() % 320;//I do random start position; I suppose it is alright :)
      pSprite->y = rand() % 240;

      int nDesiredX = nX + nLettersIntoLine*(MESSAGE_SPRITE_WIDTH + MESSAGE_SPRITE_SPACING);
      pSprite->toX = nDesiredX;
      pSprite->toY = nY;

      nLettersIntoLine++;

      pSprite->next = pPrevious;
      pPrevious = pSprite;

      m_pRoot = pSprite;
   }

   m_nStayDuration = MESSAGE_STAY_DURATION;
}

bool IsLetterMoving(LetterSprite* pSprite)
{
   return (pSprite->y != pSprite->toY) || (pSprite->x != pSprite->toX);
}

bool Message::HasMessage() const
{
   return m_pRoot != NULL;
}

bool Message::IsAnimating() const
{
   LetterSprite* pCurrent = NULL;
   for(pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
      if( IsLetterMoving(pCurrent) )
         return true;
   }

   return false;
}

bool Message::IsStayDuration() const
{
   return m_nStayDuration > 0;
}

bool Message::CheckStayDuration()
{
   //This is how I keep the letters still to be readable
   if( m_nStayDuration > 0 ) {
      m_nStayDuration--;
      //If been there long enough have them drop
      if( m_nStayDuration <= 0 ) {
         for(LetterSprite* pCurrent = m_pRoot; pCurrent != NULL; pCurrent = pCurrent->next) {
            pCurrent->toY = SCREEN_HEIGHT;
         }
      }
      return true;
   }

   return false;
}

void SetBackground(SDL_Surface* pScreen, LetterSprite* pSprite)
{
   SDL_Rect rect;
   rect.x = pSprite->x;
   rect.y = pSprite->y;
   rect.w = MESSAGE_SPRITE_WIDTH;
   rect.h = MESSAGE_SPRITE_HEIGHT;

   SDL_BlitSurface(pScreen, &rect, pSprite->replace, NULL);
}

void MoveLetter(SDL_Surface* pScreen, LetterSprite* pSprite)
{
   if( (pSprite->x != pSprite->toX) || (pSprite->y != pSprite->toY) ) {
      //int nDistX = pSprite->toX - pSprite->x;
      //int nDistY = pSprite->toY - pSprite->y;

      for(int n=0; n<MESSAGE_STEP_AMOUNT; n++) {
         if( pSprite->x < pSprite->toX ) {
            pSprite->x++;
         }
         if( pSprite->x > pSprite->toX ) {
            pSprite->x--;
         }
      }

      for(int n=0; n<MESSAGE_STEP_AMOUNT; n++) {
         if( pSprite->y < pSprite->toY ) {
            pSprite->y++;
         }
         if( pSprite->y > pSprite->toY ) {
            pSprite->y--;
         }
      }
   }

   SetBackground(pScreen, pSprite);
}

void ShowLetter(SDL_Surface* pScreen, LetterSprite* pSprite)
{
   SDL_Rect rect;
   rect.x = pSprite->x;
   rect.y = pSprite->y;
   rect.w = MESSAGE_SPRITE_WIDTH;
   rect.h = MESSAGE_SPRITE_HEIGHT;

   SDL_BlitSurface(pSprite->img, NULL, pScreen, &rect);
}

void ResetBackground(SDL_Surface* pScreen, LetterSprite* pSprite)
{
   SDL_Rect rect;
   rect.x = pSprite->x;
   rect.y = pSprite->y;
   rect.w = MESSAGE_SPRITE_WIDTH;
   rect.h = MESSAGE_SPRITE_HEIGHT;

   SDL_BlitSurface(pSprite->replace, NULL, pScreen, &rect);
}

bool Message::Animate()
{
   if( !IsAnimating() ) {
      if( !CheckStayDuration() )
         return false;
   }

   LetterSprite* pCurrent = m_pRoot;

   while(pCurrent != NULL) {
      MoveLetter(m_pScreen, pCurrent);
      pCurrent = pCurrent->next;
   }

   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      ShowLetter(m_pScreen, pCurrent);
      pCurrent = pCurrent->next;
   }
   SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   pCurrent = m_pRoot;
   while(pCurrent != NULL) {
      ResetBackground(m_pScreen, pCurrent);
      pCurrent = pCurrent->next;
   }

   return true;
}

void Message::SetFinishFast()
{
   m_bFinishFast = true;
}

bool Message::FinishFast() const
{
   return m_bFinishFast;
}

