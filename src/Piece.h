#ifndef PIECE_H
#define PIECE_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
}

#include "Metrics.h"
#include "Config.h"
#include "MovesRemainingLabel.h"
#include "Direction.h"

struct PieceSprite
{
   SDL_Surface* img;
   SDL_Surface* replace;
   int nPieceType;
   int boardX, boardY;
   int x,y;
   int toX, toY;
   bool toFade;
   bool toShrink;
   bool visible;
   int nFadeAmount;
   struct PieceSprite* next;
};

class PieceControl
{
public:
   PieceControl(SDL_Surface* pScreen, Metrics* pBoardMetrics, Config* pConfig, MovesRemainingLabel* pLabel);
   ~PieceControl();

   void ClearPieces();
   bool CreatePiece(int nType, int nX, int nY);
   bool DropPiece(int nX, int nY, Direction eDirection);
   bool DisappearPiece(int nX, int nY);
   bool HidePiece(int nX, int nY, int nType, bool bHide);
   bool ChangePiece(int nX, int nY, int nType, int nNewType);
   bool ShrinkPiece(int nX, int nY, int nType);
   bool IsAnimating() const;
   bool Animate();

protected:
   SDL_Surface* m_pScreen;//Does NOT own
   Metrics* m_pBoardMetrics;//Does NOT own
   Config* m_pConfig;//Does NOT own
   MovesRemainingLabel* m_pMovesLabel;//Does NOT own
   PieceSprite* m_pRoot;
};

#endif


