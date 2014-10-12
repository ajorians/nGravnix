#include <nspireio.h>
#include "GravnixLib/GravnixLib.h"
#include "GravnixLevels.h"

#define PRINT_FUNC nio_printf("%s", __func__);

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

int TestBeatLV1()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_1) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBeatLV2()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_2) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBeatLV3()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_3) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;
   
   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;
 
   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;
 
   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBeatLV4()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_4) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;
 
   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_RIGHT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBeatLV5()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_5) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBeatLV6()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_6) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestBeatLV50()
{
   PRINT_FUNC;
   GravnixLib api;
   if( GRAVNIXLIB_OK != GravnixLibCreate(&api, LEVEL_50) )
      return TEST_FAILED;

   if( 0 != GetGravnixMovesMadeSoFar(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )//1
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_RIGHT) )//2
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )//3
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )//4
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//5
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_RIGHT) )//6
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//7
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )//8
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//9
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )//10
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_RIGHT) )//11
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//12
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )//13
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )//14
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//15
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_RIGHT) )//16
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_UP) )//17
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_RIGHT) )//18
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//19
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_LEFT) )//20
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED == IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixMakeMove(api, GRAVNIXLIB_DIRECTION_DOWN) )//21
      return TEST_FAILED;

   if( GRAVNIXLIB_SOLVED != IsGravnixSolved(api) )
      return TEST_FAILED;

   if( GRAVNIXLIB_OK != GravnixLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
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


