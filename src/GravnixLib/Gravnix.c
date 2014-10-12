//Public domain :)

#include <os.h>
#include "include/GravnixLib.h"
#include "Defines.h"

#define GRAVNIXLIB_RUN_SAFE

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
#ifdef GRAVNIXLIB_RUN_SAFE
   if( nX < 0 || nY < 0 || (nX >= pBoard->m_nWidth) || (nY >= pBoard->m_nHeight) ) {
      printf("Accessing non-existant element %d,%d\n", nX, nY);
      return NULL;
   }
#endif

   struct GravnixItem* pItem = pBoard->m_pItems + (pBoard->m_nWidth * nY + nX);

   return pItem;
}

struct GravnixItem* GetBoardItem(struct GravnixBoard* pBoard, int nX, int nY) {
#ifdef GRAVNIXLIB_RUN_SAFE
   if( nX < 0 || nY < 0 || (nX >= pBoard->m_nWidth) || (nY >= pBoard->m_nHeight) ) {
      printf("Accessing non-existant Board element %d,%d\n", nX, nY);
      return NULL;
   }
#endif

   struct GravnixItem* pItem = pBoard->m_pBoardItems + (pBoard->m_nWidth * nY + nX);

   return pItem;
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
   int m_bPendingAction;
   char* m_pstrFile;
   int m_nNumberOfMoves;
   struct GravnixAction* m_pUndoActions;//Also used for move count :)
   struct GravnixAction* m_pRedoActions;
   int m_nLastError;
};

int GravnixLoadBoard(GravnixLib api, char* pstrFile)
{
   DEBUG_FUNC_NAME;
   DEBUG_MSG("GravnixLoadBoard -- Started\n");

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
   pH->m_bPendingAction = 0;

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
                  SetGravnixSpotValue(api, x, y, NO_PIECE);
                  SetGravnixBoardValue(api, x, y, NO_PIECE);
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
#ifdef GRAVNIXLIB_RUN_SAFE
               if( nValue < NO_PIECE || nValue > STUCK_GLASS_BLOCK_VALUE ) {
                  printf("Invalid piece for spot %d,%d with value %d\n", nX, nY, nValue);
               }
#endif
               if( nValue == BOARD_VALUE || nValue == BLACKHOLE_BLOCK_VALUE || nValue == STICKY_BLOCK_VALUE ) {
                  SetGravnixBoardValue(api, nX, nY, nValue);
               } else {
                  SetGravnixSpotValue(api, nX, nY, nValue);
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
#ifdef GRAVNIXLIB_RUN_SAFE
      if( nValue < NO_PIECE || nValue > STUCK_GLASS_BLOCK_VALUE ) {
         printf("Invalid piece for spot %d,%d with value %d\n", nX, nY, nValue);
      }
#endif

      if( nValue == BOARD_VALUE || nValue == BLACKHOLE_BLOCK_VALUE || nValue == STICKY_BLOCK_VALUE ) {
         SetGravnixBoardValue(api, nX, nY, nValue);
      } else {
         SetGravnixSpotValue(api, nX, nY, nValue);
      }
   }

#ifdef GRAVNIXLIB_RUN_SAFE
   int x,y;
   for(x=0; x<nWidth; x++) {
      for(y=0; y<nHeight; y++) {
         if( GetGravnixSpotValue(api, x, y) < NO_PIECE || GetGravnixSpotValue(api, x, y) > STUCK_GLASS_BLOCK_VALUE ) {
            printf("Bad piece for GetAt at %d,%d with value %d\n", x,y,GetGravnixSpotValue(api, x, y));
         }
         if( GetGravnixBoardValue(api, x, y) < NO_PIECE || GetGravnixBoardValue(api, x, y) > STUCK_GLASS_BLOCK_VALUE ) {
            printf("Bad piece on board at %d,%d with value %d\n", x,y,GetGravnixBoardValue(api, x, y) );
         }
      }
   }
#endif

   DEBUG_MSG("GravnixLoadBoard -- Finished!\n");

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
   pH->m_bPendingAction = 0;

   int nRet = GravnixLoadBoard((GravnixLib)pH, pstrFile);
   if( nRet != GRAVNIXLIB_OK )
      return nRet;

   pH->m_pUndoActions = NULL;
   pH->m_pRedoActions = NULL;
   pH->m_nLastError = GRAVNIXLIB_OK;

   *api = pH;

   DEBUG_MSG("GravixLibCreate\n");

   return GRAVNIXLIB_OK;
}

int GravnixLibLevelCreate(GravnixLib* api, int nWidth, int nHeight, int nMoves)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = malloc(sizeof(struct Gravnix));
   if( pH == NULL ){//Out of memory
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   pH->m_pBoard = NULL;
   pH->m_nDirection = GRAVNIXLIB_DIRECTION_NONE;
   pH->m_bPendingAction = 0; 
   pH->m_pstrFile = NULL;
   pH->m_nNumberOfMoves = nMoves;
   pH->m_pUndoActions = NULL;
   pH->m_pRedoActions = NULL;
   pH->m_nLastError = GRAVNIXLIB_OK;
   pH->m_pBoard = malloc(sizeof(struct GravnixBoard));
   if( pH->m_pBoard == NULL ){//Out of memory
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }
   pH->m_pBoard->m_pItems = NULL;
   pH->m_pBoard->m_pBoardItems = NULL;

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

   pH->m_pBoard->m_pBoardItems = malloc(nWidth*nHeight*sizeof(struct GravnixItem));
   if( pH->m_pBoard->m_pBoardItems == NULL ) {//Out of memory
      free(pH->m_pBoard->m_pItems);
      free(pH->m_pBoard);
      pH->m_pBoard = NULL;
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   *api = pH;

   DEBUG_MSG("Initializing to NO_PIECE\n");
   int x,y;
   for(x=0; x<nWidth; x++) {
      for(y=0; y<nHeight; y++) {
         SetGravnixSpotValue(*api, x, y, NO_PIECE);
         SetGravnixBoardValue(*api, x, y, NO_PIECE);
      }
   }

   DEBUG_MSG("LibLevelCreate -- Finished\n");

   return GRAVNIXLIB_OK;
}

int GravnixLibCopy(GravnixLib api, GravnixLib* copy)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pOrig = (struct Gravnix*)api;

   struct Gravnix* pH = malloc(sizeof(struct Gravnix));
   if( pH == NULL ){//Out of memory
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }
   pH->m_pBoard = NULL;
   pH->m_nDirection = GRAVNIXLIB_DIRECTION_NONE;
   pH->m_bPendingAction = 0;
   pH->m_pstrFile = NULL;
   pH->m_nNumberOfMoves = GetGravnixMoveLimit(api);
   pH->m_pUndoActions = NULL;
   pH->m_pRedoActions = NULL;
   pH->m_nLastError = GRAVNIXLIB_OK;

   pH->m_pBoard = malloc(sizeof(struct GravnixBoard));
   if( pH->m_pBoard == NULL ){//Out of memory
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   pH->m_pBoard->m_pItems = NULL;
   pH->m_pBoard->m_pBoardItems = NULL;

   int nWidth = GetGravnixBoardWidth(api), nHeight = GetGravnixBoardHeight(api);
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

   pH->m_pBoard->m_pBoardItems = malloc(nWidth*nHeight*sizeof(struct GravnixItem));
   if( pH->m_pBoard->m_pBoardItems == NULL ) {//Out of memory
      free(pH->m_pBoard->m_pItems);
      free(pH->m_pBoard);
      pH->m_pBoard = NULL;
      free(pH);
      pH = NULL;
      return GRAVNIXLIB_OUT_OF_MEMORY;
   }

   *copy = pH;

   int x,y;
   for(x=0; x<nWidth; x++) {
      for(y=0; y<nHeight; y++) {
         SetGravnixSpotValue(*copy, x, y, GetGravnixSpotValue(api, x, y));
         SetGravnixBoardValue(*copy, x, y, GetGravnixBoardValue(api, x, y));
      }
   }
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

   free(pH->m_pBoard->m_pBoardItems);
   pH->m_pBoard->m_pBoardItems = NULL;

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
         int nSpotValue = GetGravnixSpotValue(api, x, y), nBoardValue = GetGravnixBoardValue(api, x, y);
         if( nSpotValue == NO_PIECE && nBoardValue == NO_PIECE )
            continue;

         int nValue = nSpotValue > NO_PIECE ? nSpotValue : nBoardValue;

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

#ifdef GRAVNIXLIB_RUN_SAFE
   switch(nValue)
   {
      default:
      case BOARD_VALUE:
      case BLACKHOLE_BLOCK_VALUE:
         printf("Unexpected SetGravnixSpotValue: %d\n", nValue);
         break;
      case NO_PIECE:
      case RED_BLOCK_VALUE...RAINBOW_BLOCK_VALUE:
      case STICKY_BLOCK_VALUE:
      case STUCK_GLASS_BLOCK_VALUE:
         break;
   }
#endif

   GetAt(pH->m_pBoard, nX, nY)->m_nValue = nValue;
   return GRAVNIXLIB_OK;
}

int GetGravnixBoardValue(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   return GetBoardItem(pH->m_pBoard, nX, nY)->m_nValue;
}

int SetGravnixBoardValue(GravnixLib api, int nX, int nY, int nValue)
{
   DEBUG_FUNC_NAME;

#ifdef GRAVNIXLIB_RUN_SAFE
   if( nValue < NO_PIECE || nValue > STUCK_GLASS_BLOCK_VALUE ) {
      printf("Attempting to set board value spot %d,%d to %d\n", nX, nY, nValue);
   }
#endif

   struct Gravnix* pH = (struct Gravnix*)api;

#ifdef GRAVNIXLIB_RUN_SAFE
   switch(nValue)
   {
      default:
      case RED_BLOCK_VALUE...RAINBOW_BLOCK_VALUE:
         printf("Unexpected SetGravnixBoardValue: %d\n", nValue);
         break;
      case NO_PIECE:
      case BOARD_VALUE:
      case BLACKHOLE_BLOCK_VALUE:
      case STICKY_BLOCK_VALUE:
      case STUCK_GLASS_BLOCK_VALUE:
         break;
   }
#endif

   GetBoardItem(pH->m_pBoard, nX, nY)->m_nValue = nValue;
   return GRAVNIXLIB_OK;
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

   if( GetGravnixSpotValue(api, nX, nY) == NO_PIECE ) {
      return GRAVNIXLIB_CANNOT_DROP;//TODO: Different return value
   }

   if( GetGravnixSpotValue(api, nX, nY) == BOARD_VALUE ) {
      return GRAVNIXLIB_CANNOT_DROP;//TODO: Different return value
   }

   if( GetGravnixBoardValue(api, nX, nY) == BLACKHOLE_BLOCK_VALUE ) {
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

   int nItem = GetGravnixBoardValue(api, nX, nY);
   if( nItem == STICKY_BLOCK_VALUE )
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

   if( GetGravnixSpotValue(api, nX+nOffsetX, nY+nOffsetY) == NO_PIECE ) {
      int nBoardValue = GetGravnixBoardValue(api, nX+nOffsetX, nY+nOffsetY);
      if( nBoardValue == NO_PIECE || nBoardValue == BLACKHOLE_BLOCK_VALUE || nBoardValue == STICKY_BLOCK_VALUE )
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

     SetGravnixSpotValue(api, nX+nOffsetX, nY+nOffsetY, GetGravnixSpotValue(api, nX, nY));
     SetGravnixSpotValue(api, nX, nY, NO_PIECE);
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
   if( nType == NO_PIECE || nType == BOARD_VALUE || nType == GLASS_BLOCK_VALUE || nType == RAINBOW_BLOCK_VALUE || nType == BLACKHOLE_BLOCK_VALUE || nType == STICKY_BLOCK_VALUE )
      return 0;

   return 1;
}

int ShouldGravnixPieceDisappear(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nType = GetGravnixSpotValue(api, nX, nY);

   if( nType <= NO_PIECE )
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

   if( nType <= NO_PIECE )
      return GRAVNIXLIB_NO_SHRINK;

   if( GetGravnixBoardValue(api, nX, nY) == BLACKHOLE_BLOCK_VALUE ) {
      SetGravnixBoardValue(api, nX, nY, NO_PIECE);

      SetGravnixSpotValue(api, nX, nY, NO_PIECE);
      return GRAVNIXLIB_SHOULD_SHRINK;
   }

   return GRAVNIXLIB_NO_SHRINK;
}

int IsGravnixPieceStuck(GravnixLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   int nType = GetGravnixSpotValue(api, nX, nY);

   if( nType <= NO_PIECE )
      return GRAVNIXLIB_NOT_STUCK;

   if( GetGravnixBoardValue(api, nX, nY) == STICKY_BLOCK_VALUE ) {
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
      SetGravnixSpotValue(api, pNode->m_nX, pNode->m_nY, NO_PIECE);
      free(pNode);
   }
   return nAnyChanges;
}

int SlideGravnixSpots(GravnixLib api, int nDirection, int nIsUndo)
{
   DEBUG_FUNC_NAME;

   DEBUG_MSG("SlideGravnixSpots %d; undo: %d\n", nDirection, nIsUndo);

   if( nDirection == GRAVNIXLIB_DIRECTION_NONE )
      return GRAVNIXLIB_CANNOT_SLIDE_NO_DIRECTION;

   struct Gravnix* pH = (struct Gravnix*)api;
   if( pH->m_bPendingAction == 1 )
      return GRAVNIXLIB_CANNOT_SLIDE_NO_DIRECTION;//TODO: New define

   if( GetGravnixMovesMadeSoFar(api) >= pH->m_nNumberOfMoves )
      return GRAVNIXLIB_CANNOT_SLIDE_MOVE_LIMIT;

   if( nDirection == pH->m_nDirection )
      return GRAVNIXLIB_CANNOT_SLIDE_SAME_DIRECTION;

   if( !nIsUndo ) {
      ClearRedos(api);

      AddUndo(api, nDirection);
   }

   pH->m_nDirection = nDirection;
   pH->m_bPendingAction = 1;

   return GRAVNIXLIB_CAN_SLIDE;
}

int GravnixMakeMove(GravnixLib api, int nDirection)
{
   SlideGravnixSpots(api, nDirection, 0);
   while(1) {
      if( DoGravnixDropping(api) )
         continue;

      if( DoGravnixShrinking(api) )
         continue;

      if( GravnixRemoveMatches(api) )
         continue;

      break;
   }
   GravnixCheckFinishedPendingAction(api);

   GRAVNIXLIB_OK;
}

int GravnixCheckFinishedPendingAction(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;
   if( pH->m_bPendingAction == 0 )
      return 0;

   //Check any should drop
   for(int nX=0; nX<GetGravnixBoardWidth(api); nX++) {
      for(int nY=0; nY<GetGravnixBoardHeight(api); nY++) {
         if( GRAVNIXLIB_SHOULD_DROP == ShouldGravnixPieceDrop(api, nX, nY ) )
            return 1;
      }
   }

   pH->m_bPendingAction = 0;
   return 0;
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
   GravnixCheckFinishedPendingAction(api);
}

int GravnixUndo(GravnixLib api)
{
   DEBUG_FUNC_NAME;

   struct Gravnix* pH = (struct Gravnix*)api;

   if( pH->m_pstrFile == NULL )
      return GRAVNIXLIB_OK;

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


