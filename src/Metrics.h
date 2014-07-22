#ifndef METRICS_H
#define METRICS_H

extern "C"
{
        #include <os.h>
}

class Metrics
{
public:
   Metrics();
   bool SetDimensions(int nWidth, int nHeight);//TODO: Rename; call it DetermineDemensions?
   int GetWidth() const;
   int GetHeight() const;
   int GetXPos(int nBoardX) const;
   int GetYPos(int nBoardY) const;
   int GetPieceSize() const;
   int GetLeft() const;
   int GetRight() const;
   int GetTop() const;
   int GetBottom() const;

protected:
   int m_nWidth;
   int m_nHeight;
   int m_nPieceSize;
   int m_nLeft;
   int m_nTop;
};

#endif

