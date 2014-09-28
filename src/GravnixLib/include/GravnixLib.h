#ifndef GRAVNIXLIB_H_INCLUDED
#define GRAVNIXLIB_H_INCLUDED

typedef void* GravnixLib;

#define GRAVNIXLIB_OK				(0)
#define GRAVNIXLIB_BADARGUMENT			(-1)
#define GRAVNIXLIB_OUT_OF_MEMORY		(-2)
#define GRAVNIXLIB_NOT_PUZZLE_LEVEL		(-3)
#define GRAVNIXLIB_UNKNOWN_VERSION		(-4)

#define GRAVNIXLIB_SOLVED			(0)
#define GRAVNIXLIB_NOTSOLVED			(-1)

#define GRAVNIXLIB_CANNOT_UNDO			(1)
#define GRAVNIXLIB_CANNOT_REDO			(1)

#define GRAVNIXLIB_CAN_SLIDE			(0)
#define GRAVNIXLIB_CANNOT_SLIDE_MOVE_LIMIT	(1)
#define GRAVNIXLIB_CANNOT_SLIDE_NO_DIRECTION	(2)
#define GRAVNIXLIB_CANNOT_SLIDE_SAME_DIRECTION	(3)

#define GRAVNIXLIB_SHOULD_DROP			(0)
#define GRAVNIXLIB_NO_DROP			(1)
#define GRAVNIXLIB_CANNOT_DROP			(2)
#define GRAVNIXLIB_NO_PIECE_NO_DROP		(3)

#define GRAVNIXLIB_DROPPED_PIECE		(0)
#define GRAVNIXLIB_DIDNOT_DROP_PIECE		(1)

#define GRAVNIXLIB_SHOULD_DISAPPEAR		(0)
#define GRAVNIXLIB_SHOULD_DISAPPEAR_LEFT	(GRAVNIXLIB_SHOULD_DISAPPEAR)
#define GRAVNIXLIB_SHOULD_DISAPPEAR_TOP		(GRAVNIXLIB_SHOULD_DISAPPEAR)
#define GRAVNIXLIB_SHOULD_DISAPPEAR_DOWN	(GRAVNIXLIB_SHOULD_DISAPPEAR)
#define GRAVNIXLIB_SHOULD_DISAPPEAR_RIGHT	(GRAVNIXLIB_SHOULD_DISAPPEAR)
#define GRAVNIXLIB_NO_DISAPPEAR			(1)

#define GRAVNIXLIB_SHOULD_SHRINK		(0)
#define GRAVNIXLIB_NO_SHRINK			(1)

#define GRAVNIXLIB_STUCK			(0)
#define GRAVNIXLIB_NOT_STUCK			(1)

#define GRAVNIXLIB_DIRECTION_NONE		(0)
#define GRAVNIXLIB_DIRECTION_UP			(1)
#define GRAVNIXLIB_DIRECTION_LEFT		(2)
#define GRAVNIXLIB_DIRECTION_RIGHT		(3)
#define GRAVNIXLIB_DIRECTION_DOWN		(4)

#define NO_PIECE		(0)
#define BOARD_VALUE             (1)
#define RED_BLOCK_VALUE         (2)
#define BLUE_BLOCK_VALUE        (3)
#define PURPLE_BLOCK_VALUE      (4)
#define ORANGE_BLOCK_VALUE      (5)
#define YELLOW_BLOCK_VALUE      (6)
#define GREEN_BLOCK_VALUE       (7)
#define GLASS_BLOCK_VALUE       (8)
#define RAINBOW_BLOCK_VALUE     (9)
#define BLACKHOLE_BLOCK_VALUE   (10)
#define STICKY_BLOCK_VALUE      (11)
#define STUCK_GLASS_BLOCK_VALUE	(12)

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int GravnixLibCreate(GravnixLib* api, const char* pstrFile );
int GravnixLibLevelCreate(GravnixLib* api, int nWidth, int nHeight );
int GravnixLibFree(GravnixLib* api);

int GetGravnixLibError(GravnixLib api);
void ClearGravnixLibError(GravnixLib api);

int GravnixSave(GravnixLib api, char* pstr);
//////////////////////////////////////////////
//GravnixLib related functions
//////////////////////////////////////////////
int GetGravnixBoardWidth(GravnixLib api);
int GetGravnixBoardHeight(GravnixLib api);
int GetGravnixSpotValue(GravnixLib api, int nX, int nY);
int SetGravnixSpotValue(GravnixLib api, int nX, int nY, int nValue);
int GetGravnixBoardValue(GravnixLib api, int nX, int nY);
int SetGravnixBoardValue(GravnixLib api, int nX, int nY, int nValue);
int IsGravnixSolved(GravnixLib api);
int GetGravnixMoveLimit(GravnixLib api);
int GetGravnixMovesMadeSoFar(GravnixLib api);
int ShouldGravnixPieceDrop(GravnixLib api, int nX, int nY);
int ShouldGravnixPieceShrink(GravnixLib api, int nX, int nY);
int IsGravnixPieceStuck(GravnixLib api, int nX, int nY);
int DropGravnixPiece(GravnixLib api, int nX, int nY);
int DoGravnixDropping(GravnixLib api);
int ShouldGravnixPieceDisappear(GravnixLib api, int nX, int nY);
int GravnixRemoveMatches(GravnixLib api);
int SlideGravnixSpots(GravnixLib api, int nDirection, int nIsUndo);
int GetGravnixDirection(GravnixLib api);
int GravnixUndo(GravnixLib api);
int GravnixRedo(GravnixLib api);

#endif //GRAVNIXLIB_H_INCLUDED
