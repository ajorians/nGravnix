#include "Defines.h"

int Puz_Min(int a, int b) { return a<b ? a : b; }
int Puz_Max(int a, int b) { return a>b ? a : b; }

int Puz_NumberOfDigits(int n)//Very quickly thrown together
{
        if( n == 0 )
                return 1;
        int nDigits = 0;
        while(n/10 > 0 || n%10 > 0)
        {
                nDigits++;
                n/=10;
        }
        return nDigits;
}

void Puz_itoa(int n, char buffer[], int nBufferSize)//Very quickly thrown together
{
        int i=2;
        nBufferSize = Puz_Min(nBufferSize, Puz_NumberOfDigits(n)+1);
        for(; i<=nBufferSize; i++)
        {
                int nRemainder = n%10;
                buffer[nBufferSize-i] = '0' + nRemainder;
                n/=10;
        }
        buffer[nBufferSize-1] = '\0';
}

void draw_rectangle(SDL_Surface* Surface, Uint32 color, Uint16 x, Uint16 y, Uint16 width, Uint16 height, Uint8 lnpx )
{
        SDL_Rect DestRect;

        // Draw the top line
        DestRect.x = x;
        DestRect.y = y;
        DestRect.w = width;
        DestRect.h = 1;
        SDL_FillRect (Surface, &DestRect, color);

        // Draw the bottum line
        DestRect.y = y+height-1;
        SDL_FillRect (Surface, &DestRect, color);

        // Draw the left line
        DestRect.y = y;
        DestRect.w = 1;
        DestRect.h = height;
        SDL_FillRect (Surface, &DestRect, color);

        // Draw the left line
        DestRect.x = x+width-1;
        SDL_FillRect (Surface, &DestRect, color);
}
