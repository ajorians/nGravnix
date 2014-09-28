#include "Metrics.h"

Metrics::Metrics()
{
}

bool Metrics::SetDimensions(int nWidth, int nHeight)
{
   m_nWidth = nWidth;
   m_nHeight = nHeight;
   int nPieceWidth = SCREEN_WIDTH/nWidth;
   int nPieceHeight = SCREEN_HEIGHT/nHeight;

   m_nPieceSize = nPieceWidth < nPieceHeight ? nPieceWidth : nPieceHeight;

   int nBoardWidth = m_nPieceSize * nWidth;
   int nBoardHeight = m_nPieceSize * nHeight;

   m_nTop = (SCREEN_HEIGHT - nBoardHeight) / 2;
   m_nLeft = (SCREEN_WIDTH - nBoardWidth) / 2;

   return true;
}

int Metrics::GetWidth() const { return m_nWidth; }
int Metrics::GetHeight() const { return m_nHeight; }

int Metrics::GetXPos(int nBoardX) const
{
   return m_nLeft + nBoardX*m_nPieceSize;
}

int Metrics::GetYPos(int nBoardY) const
{
   return m_nTop + nBoardY*m_nPieceSize;
}

int Metrics::GetPieceSize() const
{
   return m_nPieceSize;
}

int Metrics::GetLeft() const
{
   return m_nLeft;
}

int Metrics::GetRight() const
{
   return m_nLeft + m_nWidth * m_nPieceSize;
}

int Metrics::GetTop() const
{
   return m_nTop;
}

int Metrics::GetBottom() const
{
   return m_nTop + m_nHeight * m_nPieceSize;
}
