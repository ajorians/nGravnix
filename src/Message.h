#ifndef MESSAGE_H
#define MESSAGE_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
}

struct LetterSprite
{
   SDL_Surface* img;
   SDL_Surface* replace;
   char ch;
   int x,y;
   int toX, toY;
   struct LetterSprite* next;
};

class Message
{
public:
   Message(SDL_Surface* pScreen);
   ~Message();

   void CreateMessage(const char* pstrMessage);
   bool HasMessage() const;
   bool IsAnimating() const;
   bool IsStayDuration() const;
   bool CheckStayDuration();
   bool Animate();
   void ClearMessage();

   void SetFinishFast();
   bool FinishFast() const;

protected:
   SDL_Surface* m_pScreen;//Does NOT own
   LetterSprite* m_pRoot;
   nSDL_Font *m_pFont;
   int m_nStayDuration;
   bool m_bFinishFast;
};

#endif


