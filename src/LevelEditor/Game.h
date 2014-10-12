#ifndef GAME_H
#define GAME_H

//#define USE_GRAPHIC_YOU_WIN//Use a static "You Win!!!" image instead of the animated one

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
	#include "GravnixLib/GravnixLib.h"
}

#ifndef USE_GRAPHIC_YOU_WIN
#include "Message.h"
#endif

#include "Background.h"
#include "MovesRemainingLabel.h"
#include "Metrics.h"
#include "Piece.h"
#include "Timer.h"
#include "Direction.h"
#include "Defines.h"

class Game
{
public:
	Game(SDL_Surface* pScreen, GravnixLib* pGravnix);
	~Game();

	bool Loop();
	bool ShouldQuit();
	
protected:
	int GetSpotValue(int nX, int nY);
	Direction GetSlideDirection();
	bool CheckStateConditions();
	bool PollEvents();
	void UpdateDisplay();
	void Move(Direction eDirection);
	void Undo();
	void Redo();

protected:
	SDL_Surface	*m_pScreen;//Does not own
	Background 	m_Background;
	Message		m_StartMessage;
#ifdef USE_GRAPHIC_YOU_WIN
	SDL_Surface 	*m_pWinGraphic;
#else
	Message    m_YouWinMessage;
#endif
	nSDL_Font 	*m_pFont;
	GravnixLib	m_Gravnix;
	MovesRemainingLabel	m_MovesRemainingLabel;
	bool		m_bGameOver;
	Metrics		m_BoardMetrics;
	PieceControl	m_Pieces;
	Timer		m_Timer;
	bool		m_bShouldQuit;
};

#endif
