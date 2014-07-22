//Public domain :)

#include <os.h>
#include "include/GravnixLib.h"
#include "Defines.h"

struct GravnixItem
{
   int m_nValue;
};

struct GravnixBoard
{
   int m_nWidth;
   int m_nHeight;
   struct GravnixItem* m_pItems;//These are items that can slide
   struct GravnixItem* m_pBoardItems;//These are items that don't slide.  The black hole is the only one that disappears
};

struct GravnixItem* GetAt(struct GravnixBoard* pBoard, int nX, int nY)
{
   if( nX < 0 || nY < 0 || (nX >= pBoard->m_nWidth) || (nY >= pBoard->m_nHeight) ) {
      printf("Accessing non-existant element %d,%d\n", nX, nY);
      return NULL;
   }

   return pBoard->m_pItems + (pBoard->m_nWidth * nY + nX);
}

struct GravnixItem* GetBoardItem(struct GravnixBoard* pBoard, int nX, int nY) {
   if( nX < 0 || nY < 0 || (nX >= pBoard->m_nWidth) || (nY >= pBoard->m_nHeight) ) {
      printf("Accessing non-existant Board element %d,%d\n", nX, nY);
      return NULL;
   }

   return pBoard->m_pBoardItems + (pBoard->m_nWidth * nY + nX);
}

void* AllocateBoard(int nWidth, int nHeight)
{
   return malloc(nWidth*nHeight*sizeof(struct GravnixItem));
}

void* AllocateBoardItems(int nWidth, int nHeight)
{
   return malloc(nWidth*nHeight*sizeof(struct GravnixItem));
}

struct GravnixAction
{
   int m_nDirection;
   struct GravnixAction* m_pNext;
};

struct Gravnix
{
   struct GravnixBoard* m_pBoard;
   int m_nDirection;
   char* m_pstrFile;
   int m_nNumberOfMoves;
   struct GravnixAction* m_pUndoActions;//Also use for move count :)
   struct GravnixAction* m_pRedoActions;
   int m_nLastError;
};

int GravnixLoadBoard(GravnixLib api, char* pstrFile)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   if( pH->m_pBoard != NULL ) {
      free(pH->m_pBoard);
      pH->m_pBoard = NULL;
   }

   pH->m_pBoard = malloc(sizeof(struct GravnixBoard));
   if( pH->m_pBoard == NULL ){//Out of memory
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }
   pH->m_pBoard->m_pItems = NULL;
   pH->m_nDirection = GRAVNIXLIB_DIRECTION_NONE;

   if( strstr(pstrFile, "Gravnix ") != pstrFile ) {//Gravnix file version check
      free(pH->m_pBoard);
      pH->m_pBoard = NULL;
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_NOT_PUZZLE_LEVEL;
   }

   if( strstr(pstrFile, "1 ") != (pstrFile + strlen("Gravnix ")) ) {//Version check
      free(pH->m_pBoard);
      pH->m_pBoard = NULL;
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_UNKNOWN_VERSION;
   }

   char* pstr = pstrFile + strlen("Gravnix 1 ");
   char buffer[16];
   int nSpotInBuffer = 0;

   int nNumberOfMoves = -1, nWidth = -1, nHeight = -1;
   int nX = -1, nY = -1;
   while(pstr != '\0') {
      char ch = *pstr; pstr++;
      if( isdigit(ch) ) {
         buffer[nSpotInBuffer++] = ch;
      }
      else {
         if( !isspace(ch) )
            break;
         buffer[nSpotInBuffer] = '\0';
         nSpotInBuffer = 0;
         int nValue = atoi(buffer);

         if( nNumberOfMoves < 0 ) {
            nNumberOfMoves = nValue;
            pH->m_nNumberOfMoves = nNumberOfMoves;
         }
         else if( nWidth < 0 ) {
            nWidth = nValue;
            pH->m_pBoard->m_nWidth = nWidth;
         }
         else if( nHeight < 0 ) {
            nHeight = nValue;
            pH->m_pBoard->m_nHeight = nHeight;
            pH->m_pBoard->m_pItems = malloc(nWidth*nHeight*sizeof(struct GravnixItem));

            if( pH->m_pBoard->m_pItems == NULL ) {//Out of memory
               free(pH->m_pBoard);
               pH->m_pBoard = NULL;
               free(pH);
               pH = NULL;
               return GRAVNIXLIB_OUT_OF_MEMORY;
            }

            pH->m_pBoard->m_pBoardItems = malloc(nWidth*nHeight*sizeof(struct GravnixItem));

            if( pH->m_pBoard->m_pBoardItems == NULL ) {//Out of memory
               free(pH->m_pBoard->m_pItems);
               pH->m_pBoard->m_pItems = NULL;
               free(pH->m_pBoard);
               pH->m_pBoard = NULL;
               free(pH);
               pH = NULL;
               return GRAVNIXLIB_OUT_OF_MEMORY;
            }

            int x,y;
            for(x=0; x<nWidth; x++) {
               for(y=0; y<nHeight; y++) {
                  GetAt(pH->m_pBoard, x, y)->m_nValue = 0;
                  GetBoardItem(pH->m_pBoard, x, y)->m_nValue = 0;
               }
            }

         }
         else {
            if( nX <= -1 ) {
               nX = nValue;
            }
            else if( nY <= -1 ) {
               nY = nValue;
            }
            else {
               if( nValue == BOARD_VALUE ||  nValue == BLACKHOLE_BLOCK_VALUE || nValue == STICKY_BLOCK_VALUE ) {
                  struct GravnixItem* pItem = GetBoardItem(pH->m_pBoard, nX, nY);
                  pItem->m_nValue = nValue;
               } else {
                  struct GravnixItem* pItem = GetAt(pH->m_pBoard, nX, nY);
                  pItem->m_nValue = nValue;
               }
               nX = nY = -1;
            }

            //Maybe check if has more numbers than it should?
         }
      }
   }
   if( nX >= 0 && nY >= 0 ) {
      buffer[nSpotInBuffer] = '\0';
      int nValue = atoi(buffer);
      if( nValue == BOARD_VALUE || nValue == BLACKHOLE_BLOCK_VALUE || nValue == STICKY_BLOCK_VALUE ) {
         struct GravnixItem* pItem = GetBoardItem(pH->m_pBoard, nX, nY);
         pItem->m_nValue = nValue;
      } else {
         struct GravnixItem* pItem = GetAt(pH->m_pBoard, nX, nY);
         pItem->m_nValue = nValue;
      }
   }

   return GRAVNIXLIB_OK;
}

int GravnixLibCreate(GravnixLib* api, const char* pstrFile)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = malloc(sizeof(struct Gravnix));
   if( pH == NULL ){//Out of memory
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   pH->m_pstrFile = pstrFile;
   pH->m_pBoard = NULL;
   pH->m_nDirection = GRAVNIXLIB_DIRECTION_NONE;

   int nRet = GravnixLoadBoard((GravnixLib)pH, pstrFile);
   if( nRet != GRAVNIXLIB_OK )
      return nRet;

   pH->m_pUndoActions = NULL;
   pH->m_pRedoActions = NULL;
   pH->m_nLastError = GRAVNIXLIB_OK;

   *api = pH;

   return GRAVNIXLIB_OK;
}

int GravnixLibLevelCreate(GravnixLib* api, int nWidth, int nHeight)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = malloc(sizeof(struct Gravnix));
   if( pH == NULL ){//Out of memory
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   pH->m_nDirection = GRAVNIXLIB_DIRECTION_NONE; 
   pH->m_nNumberOfMoves = 5;
   pH->m_pstrFile = NULL;
   pH->m_pUndoActions = NULL;
   pH->m_pRedoActions = NULL;
   pH->m_pBoard = malloc(sizeof(struct GravnixBoard));
   if( pH->m_pBoard == NULL ){//Out of memory
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }
   pH->m_pBoard->m_pItems = NULL;

   pH->m_pBoard->m_nWidth = nWidth;
   pH->m_pBoard->m_nHeight = nHeight;
   pH->m_pBoard->m_pItems = malloc(nWidth*nHeight*sizeof(struct GravnixItem));

   if( pH->m_pBoard->m_pItems == NULL ) {//Out of memory
      free(pH->m_pBoard);
      pH->m_pBoard = NULL;
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   int x,y;
   for(x=0; x<nWidth; x++)
      for(y=0; y<nHeight; y++)
         GetAt(pH->m_pBoard, x, y)->m_nValue = 0;

   *api = pH;

   return GRAVNIXLIB_OK;
}

void ClearUndos(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixAction* pCurrent = pH->m_pUndoActions;
   while(pCurrent != NULL) {
      struct GravnixAction* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pH->m_pUndoActions = pCurrent;
   }
}

void ClearRedos(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixAction* pCurrent = pH->m_pRedoActions;
   while(pCurrent != NULL) {
      struct GravnixAction* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pH->m_pRedoActions = pCurrent;
   }
}

void AddUndo(GravnixLib api, int nDirection)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixAction* pAction = malloc(sizeof(struct GravnixAction));
   if( pAction == NULL ) {
      DEBUG_MSG("Out of memory: AddUndo\n");
   //Assume allocated
   }

   pAction->m_nDirection = nDirection;

   struct GravnixAction* pRoot = pH->m_pUndoActions;
   pAction->m_pNext = pRoot;
   pH->m_pUndoActions = pAction;
}

void AddRedo(GravnixLib api, int nDirection)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixAction* pAction = malloc(sizeof(struct GravnixAction));
   if( pAction == NULL ) {
      DEBUG_MSG("Out of memory: AddRedo\n");
      //Assume allocated
   }

   pAction->m_nDirection = nDirection;

   struct GravnixAction* pRoot = pH->m_pRedoActions;
   pAction->m_pNext = pRoot;
   pH->m_pRedoActions = pAction;
}

int GravnixLibFree(GravnixLib* api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = *api;

   ClearUndos(*api);
   ClearRedos(*api);

   free(pH->m_pBoard->m_pItems);
   pH->m_pBoard->m_pItems = NULL;

   free(pH->m_pBoard);
   pH->m_pBoard = NULL;
   free(pH);
   pH = NULL;

   *api = NULL;
   return GRAVNIXLIB_OK;
}

int GetGravnixError(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;
   return pH->m_nLastError;
}

void ClearGravnixError(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;
   pH->m_nLastError = GRAVNIXLIB_OK;
}

int GravnixSave(GravnixLib api, char* pstr)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   strcpy(pstr, "Gravnix 1 ");

   char buffer[15];
   sprintf(buffer, "%d ", pH->m_nNumberOfMoves);
   strcat(pstr, buffer);

   sprintf(buffer, "%d ", pH->m_pBoard->m_nWidth);
   strcat(pstr, buffer);

   sprintf(buffer, "%d ", pH->m_pBoard->m_nHeight);
   strcat(pstr, buffer);

   int x=0, y=0;
   for(y=0; y<GetGravnixBoardHeight(api); y++) {
      for(x=0; x<GetGravnixBoardWidth(api); x++) {
         int nValue = GetGravnixSpotValue(api, x, y);
         if( nValue == 0 )
            continue;

         sprintf(buffer, "%d ", x);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", y);
         strcat(pstr, buffer);

         sprintf(buffer, "%d ", nValue);
         strcat(pstr, buffer);
      }
   }
   
   return GRAVNIXLIB_OK;
}

//GravnixLib related functions
int GetGravnixBoardWidth(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   return pH->m_pBoard->m_nWidth;
}

int GetGravnixBoardHeight(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   return pH->m_pBoard->m_nHeight;
}

int GetGravnixSpotValue(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   return GetAt(pH->m_pBoard, nX, nY)->m_nValue;
}

int SetGravnixSpotValue(GravnixLib api, int nX, int nY, int nValue)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   GetAt(pH->m_pBoard, nX, nY)->m_nValue = nValue;
   return GRAVNIXLIB_OK;
}

int GetGravnixBoardValue(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   return GetBoardItem(pH->m_pBoard, nX, nY)->m_nValue;
}

int IsGravnixSolved(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nX = 0, nY = 0;
   for(nX = 0; nX<GetGravnixBoardWidth(api); nX++)
      for(nY =0; nY<GetGravnixBoardHeight(api); nY++) {
         int nValue = GetGravnixSpotValue(api, nX, nY);
         if( nValue != GLASS_BLOCK_VALUE && nValue > 1 )
            return GRAVNIXLIB_NOTSOLVED;
      }

   return GRAVNIXLIB_SOLVED;
}

int GetGravnixMoveLimit(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   return pH->m_nNumberOfMoves;
}

int GetGravnixMovesMadeSoFar(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nCount = 0;
   struct GravnixAction* pCurrent = pH->m_pUndoActions;
   while(pCurrent != NULL) {
      nCount++;
      pCurrent = pCurrent->m_pNext;
   }

   return nCount;
}

int ShouldGravnixPieceDrop(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_NONE )
      return GRAVNIXLIB_CANNOT_DROP;//TODO: Different return nicer?

   if( GetGravnixSpotValue(api, nX, nY) == 0 ) {
      return GRAVNIXLIB_CANNOT_DROP;//TODO: Different return value
   }

   if( GetGravnixSpotValue(api, nX, nY) == BOARD_VALUE ) {
      return GRAVNIXLIB_CANNOT_DROP;//TODO: Different return value
   }

   if( GetGravnixSpotValue(api, nX, nY) == BLACKHOLE_BLOCK_VALUE ) {
      return GRAVNIXLIB_CANNOT_DROP;
   }

   if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_UP && nY < 1 )
      return GRAVNIXLIB_CANNOT_DROP;
   else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_LEFT && nX < 1 )
      return GRAVNIXLIB_CANNOT_DROP;
   else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_RIGHT && nX >= (GetGravnixBoardWidth(api)-1) )
      return GRAVNIXLIB_CANNOT_DROP;
   else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_DOWN && nY >= (GetGravnixBoardHeight(api)-1) )
      return GRAVNIXLIB_CANNOT_DROP;

   struct GravnixItem* pItem = GetBoardItem(pH->m_pBoard, nX, nY);
   if( pItem && pItem->m_nValue == STICKY_BLOCK_VALUE )
      return GRAVNIXLIB_CANNOT_DROP;//TODO: Different return value;

   int nOffsetX = 0, nOffsetY = 0;
   if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_UP )
      nOffsetY = -1;
   else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_LEFT )
      nOffsetX = -1;
   else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_RIGHT )
      nOffsetX = 1;
   else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_DOWN )
      nOffsetY = 1;

   if( GetGravnixSpotValue(api, nX+nOffsetX, nY+nOffsetY) == 0 ) {
      int nBoardValue = GetGravnixBoardValue(api, nX+nOffsetX, nY+nOffsetY);
      if( nBoardValue == 0 || nBoardValue == BLACKHOLE_BLOCK_VALUE || nBoardValue == STICKY_BLOCK_VALUE )
         return GRAVNIXLIB_SHOULD_DROP;
   }

   return GRAVNIXLIB_NO_DROP;
}

int DropGravnixPiece(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_NONE )
      return GRAVNIXLIB_DIDNOT_DROP_PIECE;

   if( GRAVNIXLIB_SHOULD_DROP == ShouldGravnixPieceDrop(api, nX, nY) ) {
     int nOffsetX = 0, nOffsetY = 0;
      if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_UP )
         nOffsetY = -1;
      else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_LEFT )
         nOffsetX = -1;
      else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_RIGHT )
         nOffsetX = 1;
      else if( pH->m_nDirection == GRAVNIXLIB_DIRECTION_DOWN )
         nOffsetY = 1;

     GetAt(pH->m_pBoard, nX+nOffsetX, nY+nOffsetY)->m_nValue = GetAt(pH->m_pBoard, nX, nY)->m_nValue;
     GetAt(pH->m_pBoard, nX, nY)->m_nValue = 0;
     return GRAVNIXLIB_DROPPED_PIECE;
    }

   return GRAVNIXLIB_DIDNOT_DROP_PIECE;
}

int DoGravnixDropping(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nAnyChanges = 0;
   int nX = 0, nY = 0;
   for(nY =0; nY<GetGravnixBoardHeight(api); nY++) {
      for(nX = 1; nX<GetGravnixBoardWidth(api); nX++) {
         if( GRAVNIXLIB_DROPPED_PIECE == DropGravnixPiece(api, nX, nY) ) {
            nAnyChanges = 1;
         }
      }
   }
   return nAnyChanges;
}

int IsNormalPieceType(int nType)
{
   if( nType == 0 || nType == BOARD_VALUE || nType == GLASS_BLOCK_VALUE || nType == RAINBOW_BLOCK_VALUE || nType == BLACKHOLE_BLOCK_VALUE || nType == STICKY_BLOCK_VALUE )
      return 0;

   return 1;
}

int ShouldGravnixPieceDisappear(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nType = GetGravnixSpotValue(api, nX, nY);

   if( nType <= 0 )
      return GRAVNIXLIB_NO_DISAPPEAR;

   if( nType == BOARD_VALUE )
      return GRAVNIXLIB_NO_DISAPPEAR;

   if( nType == GLASS_BLOCK_VALUE )
      return GRAVNIXLIB_NO_DISAPPEAR;

   if( nType == RAINBOW_BLOCK_VALUE ) {
      if( (nX >= 1) && (IsNormalPieceType(GetGravnixSpotValue(api, nX-1, nY))==1) ) {
         return GRAVNIXLIB_SHOULD_DISAPPEAR_LEFT;
      }
      if( (nY >= 1) && (IsNormalPieceType(GetGravnixSpotValue(api, nX, nY-1))==1) ) {
         return GRAVNIXLIB_SHOULD_DISAPPEAR_TOP;
      }
      if( (nX < (GetGravnixBoardWidth(api)-1)) && (IsNormalPieceType(GetGravnixSpotValue(api, nX+1, nY))==1) ) {
         return GRAVNIXLIB_SHOULD_DISAPPEAR_RIGHT;
      }
      if( (nY < (GetGravnixBoardHeight(api)-1)) && (IsNormalPieceType(GetGravnixSpotValue(api, nX, nY+1))==1) ) {
         return GRAVNIXLIB_SHOULD_DISAPPEAR_DOWN;
      }
   }

   //Testing with all four sides
   //Test Left
   if( nX >= 1 && (GetGravnixSpotValue(api, nX-1, nY) == nType || GetGravnixSpotValue(api, nX-1, nY) == RAINBOW_BLOCK_VALUE) ) {
      return GRAVNIXLIB_SHOULD_DISAPPEAR_LEFT;
   }
   //Test Top
   if( nY >= 1 && (GetGravnixSpotValue(api, nX, nY-1) == nType || GetGravnixSpotValue(api, nX, nY-1) == RAINBOW_BLOCK_VALUE) ) {
      return GRAVNIXLIB_SHOULD_DISAPPEAR_TOP;
   }
   //Test Right
   if( nX < (GetGravnixBoardWidth(api)-1) && (GetGravnixSpotValue(api, nX+1, nY) == nType || GetGravnixSpotValue(api, nX+1, nY) == RAINBOW_BLOCK_VALUE) ) {
      return GRAVNIXLIB_SHOULD_DISAPPEAR_RIGHT;
   }
   //Test Down
   if( nY < (GetGravnixBoardHeight(api)-1) && (GetGravnixSpotValue(api, nX, nY+1) == nType || GetGravnixSpotValue(api, nX, nY+1) == RAINBOW_BLOCK_VALUE) ) {
      return GRAVNIXLIB_SHOULD_DISAPPEAR_DOWN;
   }

   return GRAVNIXLIB_NO_DISAPPEAR;
}

int ShouldGravnixPieceShrink(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nType = GetGravnixSpotValue(api, nX, nY);

   if( nType <= 0 )
      return GRAVNIXLIB_NO_SHRINK;

   struct GravnixItem* pItem = GetBoardItem(pH->m_pBoard, nX, nY);
   if( pItem && pItem->m_nValue == BLACKHOLE_BLOCK_VALUE ) {
      pItem->m_nValue = NO_PIECE;

      GetAt(pH->m_pBoard, nX, nY)->m_nValue = NO_PIECE;
      return GRAVNIXLIB_SHOULD_SHRINK;
   }

   return GRAVNIXLIB_NO_SHRINK;
}

int IsGravnixPieceStuck(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nType = GetGravnixSpotValue(api, nX, nY);

   if( nType <= 0 )
      return GRAVNIXLIB_NOT_STUCK;

   struct GravnixItem* pItem = GetBoardItem(pH->m_pBoard, nX, nY);
   if( pItem && pItem->m_nValue == STICKY_BLOCK_VALUE ) {
      return GRAVNIXLIB_STUCK;
   }

   return GRAVNIXLIB_NOT_STUCK;
}

int DoGravnixShrinking(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nAnyChanges = 0;
   int nX = 0, nY = 0;
   for(nY =0; nY<GetGravnixBoardHeight(api); nY++) {
      for(nX = 1; nX<GetGravnixBoardWidth(api); nX++) {
         if( GRAVNIXLIB_SHOULD_SHRINK == ShouldGravnixPieceShrink(api, nX, nY) ) {
            nAnyChanges = 1;
         }
      }
   }
   return nAnyChanges;
}

struct GravnixMatches
{
   int m_nX;
   int m_nY;
   struct GravnixMatches* m_pNext;
};

int GravnixRemoveMatches(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixMatches* pRoot = NULL;

   int nAnyChanges = 0;
   int nX = 0, nY = 0;
   for(nX = 0; nX<GetGravnixBoardWidth(api); nX++) {
      for(nY =0; nY<GetGravnixBoardHeight(api); nY++) {

         if( GRAVNIXLIB_SHOULD_DISAPPEAR == ShouldGravnixPieceDisappear(api, nX, nY) ) {
            nAnyChanges = 1;
            struct GravnixMatches* pNode = malloc(sizeof(struct GravnixMatches));
            //Assume always succeeds
            pNode->m_pNext = pRoot;
            pNode->m_nX = nX;
            pNode->m_nY = nY;
            pRoot = pNode;
         }
      }
   }

   while(pRoot != NULL) {
      struct GravnixMatches* pNode = pRoot;
      pRoot = pNode->m_pNext;
      GetAt(pH->m_pBoard, pNode->m_nX, pNode->m_nY)->m_nValue = 0;
      free(pNode);
   }
   return nAnyChanges;
}

int SlideGravnixSpots(GravnixLib api, int nDirection, int nIsUndo)
{
   DEBUG_FUNC_NAME;

   if( nDirection == GRAVNIXLIB_DIRECTION_NONE )
      return GRAVNIXLIB_CANNOT_SLIDE_NO_DIRECTION;

   struct Gravnix* pH = (struct Gravnix*)api;
   if( GetGravnixMovesMadeSoFar(api) >= pH->m_nNumberOfMoves )
      return GRAVNIXLIB_CANNOT_SLIDE_MOVE_LIMIT;

   if( nDirection == pH->m_nDirection )
      return GRAVNIXLIB_CANNOT_SLIDE_SAME_DIRECTION;

   if( !nIsUndo ) {
      ClearRedos(api);

      AddUndo(api, nDirection);
   }

   pH->m_nDirection = nDirection;

   return GRAVNIXLIB_CAN_SLIDE;
}

int GetGravnixDirection(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;
   return pH->m_nDirection;
}

void DoUndo(GravnixLib api, struct GravnixAction* pUndo)
{
   if( pUndo == NULL )
      return;

   DoUndo(api, pUndo->m_pNext);

   SlideGravnixSpots(api, pUndo->m_nDirection, 1);
   while(1) {
      if( DoGravnixDropping(api) )
         continue;

      if( DoGravnixShrinking(api) )
         continue;

      if( GravnixRemoveMatches(api) )
         continue;
      
      break;
   }
}

int GravnixUndo(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixAction* pRoot = pH->m_pUndoActions;
   if( pRoot == NULL )
      return GRAVNIXLIB_CANNOT_UNDO;

   pH->m_pUndoActions = pRoot->m_pNext;
   AddRedo(api, pRoot->m_nDirection);

   GravnixLoadBoard(api, pH->m_pstrFile);   

   //Now loop over the undos; except the first one
   DoUndo(api, pH->m_pUndoActions);

   free(pRoot);
   pRoot = NULL;

   return GRAVNIXLIB_OK;
}

int GravnixRedo(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   struct GravnixAction* pRoot = pH->m_pRedoActions;
   if( pRoot == NULL )
      return GRAVNIXLIB_CANNOT_REDO;

   pH->m_pRedoActions = pRoot->m_pNext;
   AddUndo(api, pRoot->m_nDirection);

   SlideGravnixSpots(api, pRoot->m_nDirection, 1);

   free(pRoot);
   pRoot = NULL;

   return GRAVNIXLIB_OK;
}


