#include "Help.h"
#include "PieceGraphics.h"

GravnixHelp::GravnixHelp(SDL_Surface* pScreen)
: m_pScreen(pScreen)
{
	m_pFont = nSDL_LoadFont(NSDL_FONT_VGA, 0, 0, 0);

	m_pGlassGraphic = nSDL_LoadImage(image_Gravnix_GlassBlock);
	m_pRainbowGraphic = nSDL_LoadImage(image_Gravnix_RainbowBlock);
	m_pBlackholeGraphic = nSDL_LoadImage(image_Gravnix_BlackHole);
	m_pStickyGraphic = nSDL_LoadImage(image_Gravnix_StickyBlock);
}

GravnixHelp::~GravnixHelp()
{
	nSDL_FreeFont(m_pFont);
	SDL_FreeSurface(m_pGlassGraphic);
	SDL_FreeSurface(m_pRainbowGraphic);
	SDL_FreeSurface(m_pBlackholeGraphic);
	SDL_FreeSurface(m_pStickyGraphic);
}

bool GravnixHelp::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();
	
	return true;
}

bool GravnixHelp::PollEvents()
{
	SDL_Event event;
	
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                  */
	while( SDL_PollEvent( &event ) )
	{
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch( event.type )
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
					case SDLK_RETURN:
					case SDLK_SPACE:
						return false;
					break;
					
					default:
						break;
				}

				break;
			
			//Called when the mouse moves
			case SDL_MOUSEMOTION:
				break;
			
			case SDL_KEYUP:
				break;
			
			default:
				break;
		}
	}
	return true;
}

void GravnixHelp::UpdateDisplay()
{
	SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 153, 153, 255));

	nSDL_DrawString(m_pScreen, m_pFont, 15, 20, 
"Gravnix is a puzzle game.\n\
The object is to clear the board.\n\
You do this by sliding all pieces\n\
a particular direction and then\n\
if 2 or more are next to each other.\n\
then they will disappear!");

	int nLeft = 55;

	{
	SDL_Rect rect;
	rect.x = 15;
	rect.y = 80;
	rect.w = m_pGlassGraphic->w;
	rect.h = m_pGlassGraphic->h;
	SDL_BlitSurface(m_pGlassGraphic, NULL, m_pScreen, &rect);

	nSDL_DrawString(m_pScreen, m_pFont, nLeft, 80,
"Glass blocks don't match\n\
with any color block.");
	}

	{
        SDL_Rect rect;
        rect.x = 15;
        rect.y = 120;
        rect.w = m_pRainbowGraphic->w;
        rect.h = m_pRainbowGraphic->h;
        SDL_BlitSurface(m_pRainbowGraphic, NULL, m_pScreen, &rect);
	nSDL_DrawString(m_pScreen, m_pFont, nLeft, 120,
"Rainbow blocks will match\n\
any color block.");
        }

	{
        SDL_Rect rect;
        rect.x = 15;
        rect.y = 160;
        rect.w = m_pBlackholeGraphic->w;
        rect.h = m_pBlackholeGraphic->h;
        SDL_BlitSurface(m_pBlackholeGraphic, NULL, m_pScreen, &rect);
	nSDL_DrawString(m_pScreen, m_pFont, nLeft, 160,
"Blackholes dissapear once\n\
they swallow up a block.");
        }

	{
        SDL_Rect rect;
        rect.x = 15;
        rect.y = 200;
        rect.w = m_pStickyGraphic->w;
        rect.h = m_pStickyGraphic->h;
        SDL_BlitSurface(m_pStickyGraphic, NULL, m_pScreen, &rect);
	nSDL_DrawString(m_pScreen, m_pFont, nLeft, 200,
"Sticky spots prevent\n\
a block from moving");
        }
	
	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}




