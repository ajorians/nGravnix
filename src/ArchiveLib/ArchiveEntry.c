#include <os.h>
#include "ArchiveEntry.h"
#include "Defines.h"

struct ArchiveEntry* UpdateArchiveItemEntryItem(struct ArchiveEntry** ppRoot, const char* pstrName, const char* pstrValue, int* pnIndex)
{
   DEBUG_FUNC_NAME;

   struct ArchiveEntry* pCurrent = *ppRoot;
   struct ArchiveEntry* pPrevious = NULL;

   int nIndex = 0;
   while(pCurrent != NULL) {
      int nCompare = strcmp(pCurrent->m_strName, pstrName);
      if( nCompare ==0 )
      {
         strcpy(pCurrent->m_strValue, pstrValue);
         if( pnIndex != NULL )
            *pnIndex = nIndex;
         return pCurrent;
      }
      else if( nCompare > 0 )
         break;
      pPrevious = pCurrent;
      pCurrent = pCurrent->m_pNext;
      nIndex++;
   }

   struct ArchiveEntry* pEntry = malloc(sizeof(struct ArchiveEntry));
   if( pEntry == NULL ) {
      return NULL;
   }

   strcpy(pEntry->m_strName, pstrName);
   strcpy(pEntry->m_strValue, pstrValue);

   //Add node into linked list
   pEntry->m_pNext = pCurrent;
   if( *ppRoot == NULL || nIndex == 0 ) {
      *ppRoot = pEntry;
   }
   else
   {
      pPrevious->m_pNext = pEntry;
   }
   if( pnIndex != NULL )
      *pnIndex = nIndex;

   return pEntry;
}

int ReadArchiveEntries(struct ArchiveEntry** ppRoot, const char* pstr, const char* pstrEnd)
{
   DEBUG_FUNC_NAME;

   int nRet = ARCHIVE_OK;
   int nStartSearch = 0;
   char* pstrFoundStart;
   char* pstrFoundName;

   char* pstrFoundValue;
   char* pstrFoundValueEnd;

   char* pstrFoundEnd;
   while(1) {
      pstrFoundStart = strstr(pstr+nStartSearch, "<Entry name=\"");
      if( pstrFoundStart == NULL || pstrFoundStart > pstrEnd )
         break;
      pstrFoundName = strstr(pstrFoundStart+strlen("<Entry name=\""), "\"");
      if( pstrFoundName == NULL ) {
         nRet = ARCHIVE_READ_ERROR;
         break;
      }

      pstrFoundValue = strstr(pstr+nStartSearch, "value=\"");
      if( pstrFoundValue == NULL ) {
         nRet = ARCHIVE_READ_ERROR;
         break;
      }
      pstrFoundValueEnd = strstr(pstrFoundValue+strlen("value=\""), "\"");
      if( pstrFoundValueEnd == NULL ) {
         nRet = ARCHIVE_READ_ERROR;
         break;
      }

      pstrFoundEnd = strstr(pstrFoundValueEnd, "/>");
      if( pstrFoundEnd == NULL ) {
         nRet = ARCHIVE_READ_ERROR;
         break;
      }

      nStartSearch = pstrFoundEnd - pstr;

      char strBuffer[MAX_NAME_LENGTH];
      int nNameLength = pstrFoundName-pstrFoundStart-strlen("<Entry name=\"");
      strncpy(strBuffer, pstrFoundStart+strlen("<Entry name=\""), nNameLength);
      strBuffer[nNameLength] = '\0';

      char strValue[MAX_NAME_LENGTH];
      int nValueLength = pstrFoundValueEnd-pstrFoundValue-strlen("value=\"");
      strncpy(strValue, pstrFoundValue+strlen("value=\""), nValueLength);
      strValue[nValueLength] = '\0';

      struct ArchiveEntry* pEntry = UpdateArchiveItemEntryItem(ppRoot, strBuffer, strValue, NULL/*Index*/);
      if( pEntry == NULL ) {
         break;
      }
   }

   return nRet;
}

int PersistArchiveEntry(struct ArchiveEntry* pRoot, FILE* fp)
{
   DEBUG_FUNC_NAME;

   char buffer[16];
   while(pRoot != NULL) {
      ARCHIVE_WRITE("<Entry name=\"", strlen("<Entry name=\""), fp);
      if( strlen(pRoot->m_strName) > 0 ) {
         ARCHIVE_WRITE(pRoot->m_strName, strlen(pRoot->m_strName), fp);
      }
      ARCHIVE_WRITE("\" value=\"", strlen("\" value=\""), fp);
      if( strlen(pRoot->m_strValue) > 0 ) {
         ARCHIVE_WRITE(pRoot->m_strValue, strlen(pRoot->m_strValue), fp);
      }
      ARCHIVE_WRITE("\"/>\n", strlen("\"/>\n"), fp);

      pRoot = pRoot->m_pNext;
   }

   return ARCHIVE_OK;
}

void FreeArchiveEntries(struct ArchiveEntry* pRoot)
{
   DEBUG_FUNC_NAME;

   struct ArchiveEntry* pNode = pRoot;
   while(pNode != NULL) {
       pRoot = pNode->m_pNext;
       free(pNode);
       pNode = pRoot;
   }
}

int GetNumberArchiveEntries(struct ArchiveEntry* pEntry)
{
   DEBUG_FUNC_NAME;

   int nCount = 0;

   while(pEntry != NULL) {
      nCount++;
      pEntry = pEntry->m_pNext;
   }

   return nCount;
}

int RemoveEntry(struct ArchiveEntry** ppRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   int nCount = 0;
   struct ArchiveEntry* pPrevious = NULL;
   struct ArchiveEntry* pCurrent = *ppRoot;
   while(pCurrent != NULL) {
      if( nCount == nIndex )//Found it
         break;
      nCount++;
      pPrevious = pCurrent;
      pCurrent = pCurrent->m_pNext;
   }

   if( nCount < nIndex ) {//Not nIndex nodes
      return ARCHIVE_INVALID_INDEX;
   }

   if( pPrevious == NULL ) {
      *ppRoot = pCurrent->m_pNext;
   }
   else {
      pPrevious->m_pNext = pCurrent->m_pNext;
   }
   free(pCurrent);

   return ARCHIVE_OK;
}

struct ArchiveEntry* GetArchiveEntry(struct ArchiveEntry* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   int nCount = 0;
   while(pRoot != NULL) {
      if( nCount == nIndex )
         return pRoot;
      nCount++;
      pRoot = pRoot->m_pNext;
   }

   return NULL;
}

const char* GetAName(struct ArchiveEntry* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct ArchiveEntry* pHSE = GetArchiveEntry(pRoot, nIndex);
   if( pHSE == NULL )
      return "";

   return pHSE->m_strName;
}

const char* GetAValue(struct ArchiveEntry* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct ArchiveEntry* pHSE = GetArchiveEntry(pRoot, nIndex);
   if( pHSE == NULL )
      return "";

   return pHSE->m_strValue;
}


