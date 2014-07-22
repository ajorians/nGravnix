#include "Menu.h"
//#include "MainMenuGraphic.h"
#include "GravnixGraphic.h"
#include "PlayGraphic.h"
#include "OptionsHelpGraphic.h"
#include "GravnixStarGraphic.h"
#include "Defines.h"
#ifdef SHOW_LEVEL_DIMENSIONS
extern "C"
{
        #include "GravnixLib/GravnixLib.h"
}
#endif
#include "GravnixLevels.h"

int MainMenu::s_nLevel = 0;

MainMenu::MainMenu(SDL_Surface* pScreen, Config* pConfig)
: m_pScreen(pScreen), m_Background(pScreen, pConfig), m_eChoice(Play), m_pConfig(pConfig), m_bBeatLevel(false), m_nCompletionTime(-1)
#ifdef SHOW_LEVEL_DIMENSIONS
, m_nWidth(-1), m_nHeight(-1)
#endif
{
	m_pTitleGraphic 	= nSDL_LoadImage(image_nGravnix);
	m_pPlayGraphic		= nSDL_LoadImage(image_Play);
	m_pOptionsGraphic	= nSDL_LoadImage(image_OptionsAndHelp);
	SDL_SetColorKey(m_pTitleGraphic, SDL_SRCCOLORKEY, SDL_MapRGB(m_pTitleGraphic->format, 255, 255, 255));
	SDL_SetColorKey(m_pPlayGraphic, SDL_SRCCOLORKEY, SDL_MapRGB(m_pPlayGraphic->format, 255, 255, 255));
	SDL_SetColorKey(m_pOptionsGraphic, SDL_SRCCOLORKEY, SDL_MapRGB(m_pOptionsGraphic->format, 255, 255, 255));
	m_pStar = nSDL_LoadImage(image_GravnixStar);
	SDL_SetColorKey(m_pStar, SDL_SRCCOLORKEY, SDL_MapRGB(m_pStar->format, 0, 0, 0));

	m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);

	if( !g_nRelocatedGlobals ) {
        	nl_relocdata((unsigned*)g_Levels, sizeof(g_Levels)/sizeof(g_Levels[0]));
        	g_nRelocatedGlobals = 1;
   	}

	UpdateLevelDimensions();
}

MainMenu::~MainMenu()
{
	SDL_FreeSurface(m_pTitleGraphic);
	SDL_FreeSurface(m_pPlayGraphic);
	SDL_FreeSurface(m_pOptionsGraphic);
	nSDL_FreeFont(m_pFont);
}

bool MainMenu::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();

	SDL_Delay(30);
	
	return true;
}

bool MainMenu::ShouldQuit() const
{
	return m_eChoice == Quit;
}

bool MainMenu::ShowShowOptions() const
{
	return m_eChoice == Options;
}

bool MainMenu::ShouldShowHelp() const
{
	return m_eChoice == Help;
}

const char* MainMenu::GetLevel()
{
   return g_Levels[s_nLevel];
}

int MainMenu::GetLevelNumber() const
{
   return s_nLevel;
}

bool MainMenu::IncrementLevel()
{
   if( s_nLevel < (int)((sizeof(g_Levels)/sizeof(g_Levels[0]))-1) ) {
      s_nLevel++;
      return true;
   }
   return false;
}

bool MainMenu::PollEvents()
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
				printf( "Key press detected\n" );
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						fprintf(stderr, "Hit Escape!n");
						m_eChoice = Quit;
					return false;
					break;
					
					case SDLK_UP:
					case SDLK_8:
						if( m_eChoice == Help )
							m_eChoice = Options;
						else if( m_eChoice == Options )
							m_eChoice = Play;
						break;
					
					case SDLK_DOWN:
					case SDLK_2:
						if( m_eChoice == Play )
							m_eChoice = Options;
						else if( m_eChoice == Options )
							m_eChoice = Help;
						break;

					case SDLK_LEFT:
                                        case SDLK_4:
						if( s_nLevel == 0 )
							s_nLevel = (sizeof(g_Levels)/sizeof(g_Levels[0]));
						s_nLevel--;
						UpdateLevelDimensions();
						break;

					case SDLK_RIGHT:
                                        case SDLK_6:
						s_nLevel++;
						if( s_nLevel >= (int)((sizeof(g_Levels)/sizeof(g_Levels[0]))) )
							s_nLevel = 0;
						UpdateLevelDimensions();
						break;
					
					case SDLK_SPACE:
					case SDLK_RETURN:
					case SDLK_LCTRL:
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
				printf( "Key release detected\n" );
				break;
			
			default:
				break;
		}
	}
	return true;
}

void MainMenu::UpdateDisplay()
{
	//Draw background
	if( is_classic ) {
		SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 255, 255, 255));
		nSDL_DrawString(m_pScreen, m_pFont, SCREEN_WIDTH/2-30, 15, "nGravnix");
		nSDL_DrawString(m_pScreen, m_pFont, SCREEN_WIDTH/2-30, 75, "Play");
		nSDL_DrawString(m_pScreen, m_pFont, SCREEN_WIDTH/2-30, 182, "Options");
		nSDL_DrawString(m_pScreen, m_pFont, SCREEN_WIDTH/2-30, 205, "Help");
	} else {
		m_Background.DrawBackground();
		SDL_Rect rectTitle;
		rectTitle.x = (SCREEN_WIDTH - m_pTitleGraphic->w)/2;
		rectTitle.y = 15;
		rectTitle.w = m_pTitleGraphic->w;
		rectTitle.h = m_pTitleGraphic->h;
		SDL_BlitSurface(m_pTitleGraphic, NULL, m_pScreen, &rectTitle);

		SDL_Rect rectPlay;
                rectPlay.x = (SCREEN_WIDTH - m_pPlayGraphic->w)/2;
                rectPlay.y = 76;
                rectPlay.w = m_pPlayGraphic->w;
                rectPlay.h = m_pPlayGraphic->h;
                SDL_BlitSurface(m_pPlayGraphic, NULL, m_pScreen, &rectPlay);

		SDL_Rect rectOptions;
                rectOptions.x = (SCREEN_WIDTH - m_pOptionsGraphic->w)/2;
                rectOptions.y = 179;
                rectOptions.w = m_pOptionsGraphic->w;
                rectOptions.h = m_pOptionsGraphic->h;
                SDL_BlitSurface(m_pOptionsGraphic, NULL, m_pScreen, &rectOptions);
	}

        char buffer[16] = "Level ";
        Puz_itoa(s_nLevel+1, buffer + 6, 16-6);
	int nLeftLevelName = (SCREEN_WIDTH-nSDL_GetStringWidth(m_pFont, buffer))/2;
	nSDL_DrawString(m_pScreen, m_pFont, nLeftLevelName, 116, buffer);

#ifdef SHOW_LEVEL_DIMENSIONS
	if( m_nWidth > 0 && m_nHeight > 0 ) {
		char bufMessage[16];
		char buf[8];
		strcpy(bufMessage, "(");
		Puz_itoa(m_nWidth, buf, 8);
		strcat(bufMessage, buf);
		strcat(bufMessage, ", ");
		Puz_itoa(m_nHeight, buf, 8);
		strcat(bufMessage, buf);
		strcat(bufMessage, ")");
		nSDL_DrawString(m_pScreen, m_pFont, (SCREEN_WIDTH-nSDL_GetStringWidth(m_pFont, bufMessage))/2, 130, bufMessage);
	}
#endif

	if( m_bBeatLevel ) {
		SDL_Rect rectStar;
		rectStar.x = nLeftLevelName - 20;
		rectStar.y = 109;
		rectStar.w = 16;
		rectStar.h = 16;
		SDL_BlitSurface(m_pStar, NULL, m_pScreen, &rectStar);
		if( m_nCompletionTime >= 0 ) {
			char bufMessage[32];
			char buf[8];
			strcpy(bufMessage, "Completed in ");
			Puz_itoa(m_nCompletionTime, buf, 8);
			strcat(bufMessage, buf);
			strcat(bufMessage, " seconds");
			nSDL_DrawString(m_pScreen, m_pFont, (SCREEN_WIDTH-nSDL_GetStringWidth(m_pFont, bufMessage))/2, 140, bufMessage);
		}
	}
	
	if( m_eChoice == Play )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), SCREEN_WIDTH/2-60, 70, 120, 65
#ifdef SHOW_LEVEL_DIMENSIONS
+10
#endif
	, 1);
	else if( m_eChoice == Options )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 114, 172, 96, 24, 1);
	if( m_eChoice == Help )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 130, 195, 60, 25, 1);
	
	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void MainMenu::UpdateLevelDimensions()
{
	m_bBeatLevel = m_pConfig->BeatLevel(s_nLevel, m_nCompletionTime);
}


