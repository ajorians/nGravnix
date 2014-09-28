#ifndef COLOR_INTENSITY_H
#define COLOR_INTENSITY_H

class ColorIntensity
{
public:
   ColorIntensity();
   void Animate();
   void AdjustColor(int& r, int& g, int& b, int& a);

protected:
   int Amount(int clr);
   void Lighten(int& clr);
   void Darken(int &clr);
   void Adjust(int& clr);

protected:
   int m_nLevel;
   bool m_bDirection;//true=up
};

#endif

