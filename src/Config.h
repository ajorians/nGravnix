#ifndef CONFIG_H
#define CONFIG_H

extern "C"
{
	#include <os.h>
	#include "ArchiveLib/ArchiveLib.h"
}

#define MAKE_CONFIG_ITEM(type, name) \
public: \
   type Get##name() const { return m_##name; }\
   void Set##name(type val) { m_##name = val; }\
protected: \
   type m_##name;

#define PIECE_FADE_LIMIT                (10)
#define PIECE_BRIGHTEN_STEP_AMOUNT      (25)
#define PIECE_STEP_AMOUNT               (10)

class Config
{
public:
   Config();
   ~Config();

   MAKE_CONFIG_ITEM(bool, BackgroundMoves);

public:
   bool BeatLevel(int nLevelNumber, int& nCompletionTime) const;
   void SetBeatLevel(int nLevelNumber, int nTimeElapsed);

   MAKE_CONFIG_ITEM(int, FadeSteps);
   MAKE_CONFIG_ITEM(int, BrightenPerStep);
   MAKE_CONFIG_ITEM(int, PieceMovePerStep);

   MAKE_CONFIG_ITEM(bool, ShowOpeningMoveLimit);

protected:
   ArchiveLib	m_Archive;
};

#endif

