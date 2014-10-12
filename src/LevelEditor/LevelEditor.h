#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
	#include "GravnixLib/GravnixLib.h"
}

#include "Metrics.h"
#include "Piece.h"
#include "Direction.h"
#include "Background.h"
#include "Selector.h"
#include "Defines.h"

class LevelEditor
{
public:
	LevelEditor(SDL_Surface* pScreen, int nWidth, int nHeight, int nMoves);
	~LevelEditor();

	bool Loop();
	
protected:
	bool CheckStateConditions();
	bool PollEvents();
	void UpdateDisplay();
	void Move(Direction eDirection);
	void SetCurrentPiece();
	void PrintLevel();
	bool CheckClose();

	void TestLevel();

protected:
	SDL_Surface	*m_pScreen;//Does not own
	Background m_Background;
	nSDL_Font 	*m_pFont;
	GravnixLib	m_Gravnix;
	Metrics		m_BoardMetrics;
	PieceControl	m_Pieces;
	Selector	m_Selector;
	bool		m_bModified;
};

#endif
