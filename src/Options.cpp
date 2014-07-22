#include "Options.h"
#include "Defines.h"

Options::Options(SDL_Surface* pScreen, Config* pConfig)
: m_pScreen(pScreen), m_nOptionsIndex(0), m_pConfig(pConfig)
{
	m_pFont = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 0, 0, 0);
	nSDL_SetFontSpacing(m_pFont, 0, 2);
}

Options::~Options()
{
	nSDL_FreeFont(m_pFont);
}

bool Options::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();
	
	return true;
}

bool Options::PollEvents()
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
						return false;
					break;

					case SDLK_SPACE:
                                        case SDLK_RETURN:
                                        case SDLK_LCTRL:
						ToggleCurrentOption();
                                                break;

					case SDLK_UP:
                                        case SDLK_8:
                                                Move(OP_Up);
                                                break;

                                        case SDLK_DOWN:
                                        case SDLK_2:
                                                Move(OP_Down);
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

void Options::ToggleCurrentOption()
{
	if( m_nOptionsIndex == 0 ) {
		m_pConfig->SetBackgroundMoves(!m_pConfig->GetBackgroundMoves());
	} else if( m_nOptionsIndex == 1 ) {
		int nFadeSteps = m_pConfig->GetFadeSteps();
		int nBrightenPerStep = m_pConfig->GetBrightenPerStep();
		int nPieceMovePerStep = m_pConfig->GetPieceMovePerStep();
		if( nFadeSteps >= PIECE_FADE_LIMIT ) {
			//Set to Fast
			nFadeSteps = PIECE_FADE_LIMIT/2;
			nBrightenPerStep = PIECE_BRIGHTEN_STEP_AMOUNT*2;
			nPieceMovePerStep = PIECE_STEP_AMOUNT*3;
		} else if( nFadeSteps >= PIECE_FADE_LIMIT/2 ) {
			//Set to Instant
			nFadeSteps = PIECE_FADE_LIMIT/10;
                        nBrightenPerStep = PIECE_BRIGHTEN_STEP_AMOUNT*3;
                        nPieceMovePerStep = PIECE_STEP_AMOUNT*15;
		} else {
			//Set to Normal
			nFadeSteps = PIECE_FADE_LIMIT;
                        nBrightenPerStep = PIECE_BRIGHTEN_STEP_AMOUNT;
                        nPieceMovePerStep = PIECE_STEP_AMOUNT;
		}
		m_pConfig->SetFadeSteps(nFadeSteps);
		m_pConfig->SetBrightenPerStep(nBrightenPerStep);
		m_pConfig->SetPieceMovePerStep(nPieceMovePerStep);
        }
}

void Options::Move(Option_Direction eDirection)
{
	if( eDirection == OP_Down && m_nOptionsIndex < 1 ) {
		m_nOptionsIndex++;
	} else if( eDirection == OP_Up && m_nOptionsIndex > 0 ) {
		m_nOptionsIndex--;
	}
}

#define BACKGROUND_MOVES_Y      (45)
#define ANIMATION_SPEED_Y	(95)
void Options::UpdateDisplay()
{
	//Draw background
	SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 153, 153, 255));
	nSDL_DrawString(m_pScreen, m_pFont, (SCREEN_WIDTH-nSDL_GetStringWidth(m_pFont, "Options:"))/2, 15, "Options:");

	char buffer[256] = "Moving Background: ";

	if( m_pConfig->GetBackgroundMoves() ) {
		strcat(buffer, "ON");
	} else {
		strcat(buffer, "OFF");
	}

	strcat(buffer, 
"\n\
CX Version only.  Specifies whether\n\
the background slides or not");

	nSDL_DrawString(m_pScreen, m_pFont, 12, BACKGROUND_MOVES_Y, buffer); 

	if( m_nOptionsIndex == 0 ) {
		if( is_classic ) {
			draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 255, 255), 7, BACKGROUND_MOVES_Y-5, 312, 45, 1);
		} else {
			draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 7, BACKGROUND_MOVES_Y-5, 312, 45, 1);
		}
	}

	strcpy(buffer, "Animation Speed: ");
	if( m_pConfig->GetFadeSteps() >= PIECE_FADE_LIMIT ) {
		strcat(buffer, "NORMAL");
	} else if( m_pConfig->GetFadeSteps() >= PIECE_FADE_LIMIT/2 ) {
		strcat(buffer, "FAST");
	} else {
		strcat(buffer, "INSTANT");
	}

	strcat(buffer,
"\n\
Speed in which pieces animate");
	nSDL_DrawString(m_pScreen, m_pFont, 12, ANIMATION_SPEED_Y, buffer);

        if( m_nOptionsIndex == 1 ) {
                if( is_classic ) {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 255, 255), 7, ANIMATION_SPEED_Y-5, 312, 35, 1);
                } else {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 7, ANIMATION_SPEED_Y-5, 312, 35, 1);
                }
        }

	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

}




