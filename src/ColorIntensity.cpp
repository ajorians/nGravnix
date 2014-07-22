#include "ColorIntensity.h"
#include "Defines.h"

#define COLOR_PERCENTAGE_CHANGE	(30)
#define COLOR_INTENSITY_LEVELS	(20)

ColorIntensity::ColorIntensity()
: m_nLevel(0), m_bDirection(true)
{
}

void ColorIntensity::Animate()
{
   if( m_bDirection ) {
      m_nLevel++;
   } else {
      m_nLevel--;
   }

   if( m_nLevel <= 0 ) {
      m_bDirection = true;
   } else if( m_nLevel >= COLOR_INTENSITY_LEVELS ) {
      m_bDirection = false;
   }
}

int ColorIntensity::Amount(int clr)
{
   int nAmountChangeAllowed = clr*COLOR_PERCENTAGE_CHANGE/100;
   int nAdjust = nAmountChangeAllowed*m_nLevel/COLOR_INTENSITY_LEVELS;
   return nAdjust;
}

void ColorIntensity::Lighten(int& clr)
{
   clr = Puz_Min(255, clr + Amount(clr));
}

void ColorIntensity::Darken(int &clr)
{
   clr = Puz_Max(0, clr - Amount(clr));
}

void ColorIntensity::Adjust(int& clr)
{
   if( clr/2 >= 128 ) {
      Lighten(clr);
   } else {
      Darken(clr);
   }
}

void ColorIntensity::AdjustColor(int& r, int& g, int& b, int& a)
{
   Adjust(r);
   Adjust(g);
   Adjust(b);
   Adjust(a);
}

