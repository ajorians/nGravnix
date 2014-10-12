#include <nspireio.h>
#include "GravnixLib/GravnixLib.h"
#include "GravnixLevels.h"

#define PRINT_FUNC nio_printf("%s", __func__); /*printf(__func__); printf("\n");*/

#define TEST_SUCCEEDED	(0)
#define TEST_FAILED	(1)

#define TEST_PERSISTING

int g_nRelocatedGlobals = 0;

int TestConstruction()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestNumberOfMovesLV1()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( 6 != GetGravnixMoveLimit(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestWidthHeightLV1()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( 6 != GetGravnixBoardWidth(api) )
      return TEST_FAILED;

   if( 6 != GetGravnixBoardWidth(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestInitiallySolvedLV1()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestInitialMovesMadeLV1()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

typedef enum
{
   Move_Left = GRAVNIXLIB_DIRECTION_LEFT,
   Move_Up = GRAVNIXLIB_DIRECTION_UP,
   Move_Right = GRAVNIXLIB_DIRECTION_RIGHT,
   Move_Down = GRAVNIXLIB_DIRECTION_DOWN
} MoveDirection_t;

int MakeMoves(GravnixLib* api, MoveDirection_t arr[], int nCount)
{
   int i;
   for(i=0; i<nCount; i++) {
      int nDirection = (int) arr[i];
      if( GRAVNIXLIB_OK != GravnixMakeMove(*api, nDirection) ) {
         printf("Failed moving\n");
         return TEST_FAILED;
      }

      if( (i < (nCount-1)) && GRAVNIXLIB_SOLVED == IsGravnixSolved(*api) ) {
         printf("Failed solve check\n");
         return TEST_FAILED;
      }
   }
   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(*api) )
      return TEST_SUCCEEDED;
   printf("failed other solve check\n");
   return TEST_FAILED;
}

int TestBeatLV1()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 2);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV2()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_2) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Up, Move_Left };
   int nRet = MakeMoves(&api, arrMoves, 2);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV3()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_3) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Left, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV4()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_4) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 3);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV5()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_5) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Left };
   int nRet = MakeMoves(&api, arrMoves, 2);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV6()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_6) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Up, Move_Left };
   int nRet = MakeMoves(&api, arrMoves, 2);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV7()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_7) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Up, Move_Left, Move_Down, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV8()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_8) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Left, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV9()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_9) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Right, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV10()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_10) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Down, Move_Left, Move_Up, Move_Left };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV11()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_11) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Right, Move_Up, Move_Left, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV12()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_12) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Down, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 3);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV13()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_13) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Down, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 3);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV14()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_14) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Right, Move_Left, Move_Up, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV15()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_15) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Left, Move_Up, Move_Right, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV16()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_16) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Right, Move_Left };
   int nRet = MakeMoves(&api, arrMoves, 3);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV17()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_17) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Right, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV18()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_18) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = {
      Move_Down, 
      Move_Right,
      Move_Left,
      Move_Up,
      Move_Left,
      Move_Up,
      Move_Left
   };
   int nRet = MakeMoves(&api, arrMoves, 7);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV19()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_19) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up,
      Move_Down,
      Move_Right,
      Move_Up     
   };
   int nRet = MakeMoves(&api, arrMoves, 9);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV20()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_20) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Down, Move_Right, Move_Up, Move_Left };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV21()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_21) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Left, Move_Down, Move_Left, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV22()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_22) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Right, Move_Up, Move_Left, Move_Down, Move_Right, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 6);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV23()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_23) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Right, Move_Down, Move_Left, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV24()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_24) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Left,
      Move_Up,
      Move_Left,
      Move_Right,
      Move_Down,
      Move_Right,
      Move_Down,
      Move_Left
   };
   int nRet = MakeMoves(&api, arrMoves, 8);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV25()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_25) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Down, Move_Up, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV26()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_26) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Down, 
      Move_Left, 
      Move_Down, 
      Move_Right, 
      Move_Down, 
      Move_Left, 
      Move_Down, 
      Move_Right 
   };
   int nRet = MakeMoves(&api, arrMoves, 8);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV27()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_27) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Right, 
      Move_Down,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up
   };
   int nRet = MakeMoves(&api, arrMoves, 7);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV28()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_28) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Right, Move_Down, Move_Left, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV29()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_29) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Down,
      Move_Left
   };
   int nRet = MakeMoves(&api, arrMoves, 10);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV30()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_30) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Down, 
      Move_Left, 
      Move_Up, 
      Move_Right, 
      Move_Up, 
      Move_Left, 
      Move_Down, 
      Move_Left, 
      Move_Up 
   };
   int nRet = MakeMoves(&api, arrMoves, 9);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV31()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_31) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Up, 
      Move_Left, 
      Move_Right, 
      Move_Down, 
      Move_Right, 
      Move_Down, 
      Move_Left 
   };
   int nRet = MakeMoves(&api, arrMoves, 7);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV32()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_32) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Left, Move_Down, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV33()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_33) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Right, Move_Up, Move_Left, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV34()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_34) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Left,
      Move_Up,
      Move_Left,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Down,
      Move_Up,
      Move_Left,
      Move_Down
   };
   int nRet = MakeMoves(&api, arrMoves, 11);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV35()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_35) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Right, 
      Move_Up, 
      Move_Right, 
      Move_Down, 
      Move_Right, 
      Move_Up, 
      Move_Left 
   };
   int nRet = MakeMoves(&api, arrMoves, 7);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV36()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_36) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Right, 
      Move_Down, 
      Move_Left, 
      Move_Right, 
      Move_Up, 
      Move_Left, 
      Move_Down 
   };
   int nRet = MakeMoves(&api, arrMoves, 7);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV37()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_37) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Left,
      Move_Up,
      Move_Left,
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up
   };
   int nRet = MakeMoves(&api, arrMoves, 9);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV38()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_38) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Up, Move_Down, Move_Left, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 4);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV39()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_39) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up,
      Move_Down,
      Move_Right,
      Move_Down
   };
   int nRet = MakeMoves(&api, arrMoves, 11);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV40()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_40) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Left, Move_Up, Move_Right, Move_Left, Move_Up };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV41()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_41) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Up, Move_Right, Move_Left, Move_Right, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV42()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_42) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Down,
      Move_Left,
      Move_Up,
      Move_Right
   };
   int nRet = MakeMoves(&api, arrMoves, 10);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV43()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_43) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Left,
      Move_Down
   };
   int nRet = MakeMoves(&api, arrMoves, 12);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV44()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_44) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Up, Move_Down, Move_Left, Move_Up, Move_Right, Move_Down };
   int nRet = MakeMoves(&api, arrMoves, 6);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV45()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_45) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Left,
      Move_Down,
      Move_Left,
      Move_Down,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Up,
      Move_Down,
      Move_Right,
      Move_Up,
      Move_Right,
      Move_Up,
      Move_Right
   };
   int nRet = MakeMoves(&api, arrMoves, 14);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV46()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_46) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Right,
      Move_Down,
      Move_Left,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Down,
      Move_Right,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Down,
      Move_Left
   };
   int nRet = MakeMoves(&api, arrMoves, 14);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV47()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_47) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Down,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Up,
      Move_Left
   };
   int nRet = MakeMoves(&api, arrMoves, 12);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV48()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_48) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Up,
      Move_Right,
      Move_Up,
      Move_Left,
      Move_Down,
      Move_Right,
      Move_Down,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Down,
      Move_Left,
      Move_Up,
      Move_Right,
      Move_Down,
      Move_Left,
      Move_Up
   };
   int nRet = MakeMoves(&api, arrMoves, 17);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV49()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_49) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { Move_Down, Move_Right, Move_Left, Move_Up, Move_Right };
   int nRet = MakeMoves(&api, arrMoves, 5);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

int TestBeatLV50()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_50) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   MoveDirection_t arrMoves[] = { 
      Move_Up,//1
      Move_Right,//2
      Move_Up,//3
      Move_Left,//4
      Move_Down,//5
      Move_Right,//6
      Move_Down,//7
      Move_Left,//8
      Move_Down,//9
      Move_Left,//10
      Move_Right,//11
      Move_Down,//12
      Move_Left,//13
      Move_Up,//14
      Move_Down,//15
      Move_Right,//16
      Move_Up,//17
      Move_Right,//18
      Move_Down,//19
      Move_Left,//20
      Move_Down//21
    };
   int nRet = MakeMoves(&api, arrMoves, 21);

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return nRet;
}

typedef int (*testfunc)();
   testfunc g_Tests[] =
   {
      TestConstruction,
      TestNumberOfMovesLV1,
      TestWidthHeightLV1,
      TestInitiallySolvedLV1,
      TestInitialMovesMadeLV1,
      TestBeatLV1,
      TestBeatLV2,
      TestBeatLV3,
      TestBeatLV4,
      TestBeatLV5,
      TestBeatLV6,
      TestBeatLV7,
      TestBeatLV8,
      TestBeatLV9,
      TestBeatLV10,
      TestBeatLV11,
      TestBeatLV12,
      TestBeatLV13,
      TestBeatLV14,
      TestBeatLV15,
      TestBeatLV16,
      TestBeatLV17,
      TestBeatLV18,
      TestBeatLV19,
      TestBeatLV20,
      TestBeatLV21,
      TestBeatLV22,
      TestBeatLV23,
      TestBeatLV24,
      TestBeatLV25,
      TestBeatLV26,
      TestBeatLV27,
      TestBeatLV28,
      TestBeatLV29,
      TestBeatLV30,
      TestBeatLV31,
      TestBeatLV32,
      TestBeatLV33,
      TestBeatLV34,
      TestBeatLV35,
      TestBeatLV36,
      TestBeatLV37,
      TestBeatLV38,
      TestBeatLV39,
      TestBeatLV40,
      TestBeatLV41,
      TestBeatLV42,
      TestBeatLV43,
      TestBeatLV44,
      TestBeatLV45,
      TestBeatLV46,
      TestBeatLV47,
      TestBeatLV48,
      TestBeatLV49,
      TestBeatLV50
   };

void RunTests()
{
   if( !g_nRelocatedGlobals ) {
      nl_relocdata((unsigned*)g_Tests, sizeof(g_Tests)/sizeof(g_Tests[0]));
      g_nRelocatedGlobals = 1;
   }

   unsigned i;
   for(i=0; i<sizeof(g_Tests)/sizeof(testfunc); i++) {

      int nRet = g_Tests[i]();
      if( nRet == TEST_SUCCEEDED ) {
         nio_printf("........OK\n");
      } else {
         nio_printf("........FAIL\n");
      }
   }
}


