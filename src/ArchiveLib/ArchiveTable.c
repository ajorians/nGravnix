#include <os.h>
#include "ArchiveTable.h"
#include "ArchiveEntry.h"
#include "Defines.h"

void FreeArchiveTables(struct ArchiveTable* pRoot)
{
   DEBUG_FUNC_NAME;

   struct ArchiveTable* pNode = pRoot;
   while(pNode != NULL) {
       pRoot = pNode->m_pNext;
       FreeArchiveEntries(pNode->m_pEntries);
       free(pNode);
       pNode = pRoot;
   }
}

int ReadArchiveTables(struct ArchiveTable** ppRoot, const char* pstr)
{
   DEBUG_FUNC_NAME;

   int nRet = ARCHIVE_OK;
   int nStartSearch = 0;
   char* pstrFoundStart;
   char* pstrFoundName;
   char* pstrFoundEnd;
   while(1) {
      pstrFoundStart = strstr(pstr+nStartSearch, "<Category name=\"");
      if( pstrFoundStart == NULL )
         break;
      pstrFoundName = strstr(pstrFoundStart+strlen("<Category name=\""), "\"");
      pstrFoundEnd = strstr(pstrFoundStart, "</Category>\n");
      nStartSearch = pstrFoundEnd - pstr;
      if( pstrFoundName == NULL || pstrFoundEnd == NULL ) {
         nRet = ARCHIVE_READ_ERROR;
         break;
      }

      char strCategory[MAX_IDENTIFIER_LENGTH];
      int nNameLength = pstrFoundName-strlen("<Category name=\"")-pstrFoundStart;
      strncpy(strCategory, pstrFoundStart+strlen("<Category name=\""), nNameLength);
      strCategory[nNameLength] = '\0';
      struct ArchiveTable* pHST = CreateArchiveTable(ppRoot, strCategory);

      nRet = ReadArchiveEntries(&pHST->m_pEntries, pstrFoundName, pstrFoundEnd);
   }

   return nRet;
}

int PersistArchiveTables(struct ArchiveTable* pRoot, FILE* fp)
{
   DEBUG_FUNC_NAME;

   int nRet = ARCHIVE_OK;

   while( pRoot != NULL ) {
      ARCHIVE_WRITE("<Category name=\"", strlen("<Category name=\""), fp);
      if( strlen(pRoot->m_strIdentifierName) > 0 ) {
         ARCHIVE_WRITE(pRoot->m_strIdentifierName, strlen(pRoot->m_strIdentifierName), fp);
      }
      ARCHIVE_WRITE("\">\n", strlen("\">\n"), fp);

      nRet = PersistArchiveEntry(pRoot->m_pEntries, fp);
      if( nRet != ARCHIVE_OK ) {
         return nRet;
      }

      ARCHIVE_WRITE("</Category>\n", strlen("</Category>\n"), fp);

      pRoot = pRoot->m_pNext;
   }
   return nRet;
}

int GetNumberOfArchiveTables(struct ArchiveTable* pRoot)
{
   DEBUG_FUNC_NAME;

   int nCount = 0;
   while( pRoot != NULL ) {
      nCount++;
      pRoot = pRoot->m_pNext;
   }
   return nCount;
}

struct ArchiveTable* GetArchiveTableFromIndex(struct ArchiveTable* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   int nCount = 0;
   while( pRoot != NULL ) {
      if( nCount == nIndex )
         return pRoot;
      nCount++;
      pRoot = pRoot->m_pNext;
   }
   return NULL;
}

struct ArchiveTable* GetArchiveTable(struct ArchiveTable* pRoot, Identifier ID)
{
   DEBUG_FUNC_NAME;

   if( pRoot == NULL ) {//Not found
      return NULL;
   }

   if( !strcmp(pRoot->m_strIdentifierName, ID) )//Matches
      return pRoot;

   return GetArchiveTable(pRoot->m_pNext, ID);
}

struct ArchiveTable* CreateArchiveTable(struct ArchiveTable** ppRoot, Identifier ID)
{
   DEBUG_FUNC_NAME;

   struct ArchiveTable* pCurrent = *ppRoot;
   struct ArchiveTable* pPrevious = NULL;

   while(pCurrent != NULL) {
      pPrevious = pCurrent;
      pCurrent = pCurrent->m_pNext;
   }

   struct ArchiveTable* pTable = malloc(sizeof(struct ArchiveTable));
   if( pTable == NULL ) {
      return NULL;
   }

   strcpy(pTable->m_strIdentifierName, ID);
   pTable->m_pEntries = NULL;

   //Add node into linked list
   pTable->m_pNext = pCurrent;
   if( *ppRoot == NULL ) {
      *ppRoot = pTable;
   }
   else
   {
      pPrevious->m_pNext = pTable;
   }

   return pTable;
}

int GetNumberOfEntries(struct ArchiveTable* pTable)
{
   DEBUG_FUNC_NAME;

   return GetNumberArchiveEntries(pTable->m_pEntries);
}

int UpdateArchiveItemEntry(struct ArchiveTable* pRoot, const char* pstrName, const char* pstrValue, int* pnIndex)
{
   DEBUG_FUNC_NAME;

   struct ArchiveEntry* pEntry = UpdateArchiveItemEntryItem(&pRoot->m_pEntries, pstrName, pstrValue, pnIndex);

   if( pEntry == NULL ) {
      return ARCHIVE_OUT_OF_MEMORY;
   }

   return ARCHIVE_OK;
}

int ClearAllEntries(struct ArchiveTable* pRoot)
{
   DEBUG_FUNC_NAME;

   while(GetNumberOfEntries(pRoot) > 0) {
      RemoveArchiveIndex(pRoot, 0);
   }

   return ARCHIVE_OK;
}

int RemoveArchiveIndex(struct ArchiveTable* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   return RemoveEntry(&pRoot->m_pEntries, nIndex);
}

const char* GetArchiveName(struct ArchiveTable* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   return GetAName(pRoot->m_pEntries, nIndex);
}

const char* GetArchiveValue(struct ArchiveTable* pRoot, int nIndex)
{
   DEBUG_FUNC_NAME;

   return GetAValue(pRoot->m_pEntries, nIndex);
}

