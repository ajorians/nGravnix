#ifndef MENU_H
#define MENU_H

//#define SHOW_LEVEL_DIMENSIONS

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
}

#include "Background.h"
#include "Config.h"

class MainMenu
{
public:
	MainMenu(SDL_Surface* pScreen, Config* pConfig);
	~MainMenu();

	bool Loop();

	bool ShouldQuit() const;
	bool ShowShowOptions() const;
	bool ShouldShowHelp() const;
	const char* GetLevel();
	int GetLevelNumber() const;
	bool IncrementLevel();
	
protected:
	bool PollEvents();
	void UpdateDisplay();
	void UpdateLevelDimensions();

protected:
	SDL_Surface	*m_pScreen;//Does not own
	Background	m_Background;
	SDL_Surface	*m_pTitleGraphic;
	SDL_Surface	*m_pPlayGraphic;
	SDL_Surface	*m_pOptionsGraphic;
	//SDL_Surface 	*m_pBackground;
	nSDL_Font 	*m_pFont;
	enum MenuChoice{Play, Options, Help, Quit};
	MenuChoice 	m_eChoice;
	Config		*m_pConfig;
	static int 	s_nLevel;
	bool		m_bBeatLevel;
	int		m_nCompletionTime;
	SDL_Surface	*m_pStar;
#ifdef SHOW_LEVEL_DIMENSIONS
	int 		m_nWidth;
	int 		m_nHeight;
#endif
};

#endif
