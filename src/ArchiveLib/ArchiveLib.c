//Public domain :)

#include <os.h>
#include "include/ArchiveLib.h"
#include "ArchiveTable.h"
#include "Defines.h"

char g_strCurrentPath[MAX_PATH];

struct Archive
{
   int m_nLastError;					//The last error
   int m_nBatch;					//Whether setting many files at once
   char m_strArchiveFile[MAX_PATH];			//The filename of the archive file
   struct ArchiveTable* m_pRootArchiveTables;		//The root Archive table data

   char m_strCurrentCategory[MAX_IDENTIFIER_LENGTH];
};

int ArchiveCreate(ArchiveLib* api)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = malloc(sizeof(struct Archive));
   if( pHS == NULL ){//Out of memory
      return ARCHIVE_OUT_OF_MEMORY;
   }

   pHS->m_nLastError = ARCHIVE_OK;
   pHS->m_nBatch = ARCHIVE_DISABLE_BATCH;
   strcpy(pHS->m_strArchiveFile, "");
   pHS->m_pRootArchiveTables = NULL;
   strcpy(pHS->m_strCurrentCategory, "");

   *api = pHS;
   return ARCHIVE_OK;
}

int ArchiveFree(ArchiveLib* api)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = *api;

   struct ArchiveTable* pHST = pHS->m_pRootArchiveTables;
   FreeArchiveTables(pHST);

   free(pHS);
   *api = NULL;
   return ARCHIVE_OK;
}

int OpenArchiveFile(ArchiveLib api, const char* pstrFile)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   if( pstrFile == NULL ) {
      pHS->m_nLastError = ARCHIVE_BADARGUMENT;
      return ARCHIVE_BADARGUMENT;
   }

   char strPath[MAX_PATH];

   //Does it start with a '/'; if not think of it as a relative path
   if( (*pstrFile) == '/' ) {
      strcpy(strPath, "");
   }
   else {
      strcpy(strPath, g_strCurrentPath);
   }
   strcat(strPath, pstrFile);

   if( strlen(pHS->m_strArchiveFile) ) {//Called with a file already opened; in this case I just reset; though the behavior could be changed if desired
      struct ArchiveTable* pHST = pHS->m_pRootArchiveTables;
      FreeArchiveTables(pHST);

      pHS->m_nLastError = ARCHIVE_OK;
      pHS->m_nBatch = ARCHIVE_DISABLE_BATCH;
      strcpy(pHS->m_strArchiveFile, "");
      pHS->m_pRootArchiveTables = NULL;
      strcpy(pHS->m_strCurrentCategory, "");
   }

   strcpy(pHS->m_strArchiveFile, strPath);

   struct stat statbuf;
   if (stat(strPath, &statbuf) == -1) {
      pHS->m_nLastError = ARCHIVE_UNABLE_OPEN_FILE;
      return ARCHIVE_UNABLE_OPEN_FILE;//This isn't an error; I probably could have called this function something
      //better; open kinda implies an existing file.
   }

   if( statbuf.st_size <= 0 ) {
      pHS->m_nLastError = ARCHIVE_UNABLE_OPEN_FILE;
      return ARCHIVE_UNABLE_OPEN_FILE;//Again probably should call this SetHSFileName instead of Open; as this
      //isn't an error.
   }

   FILE *fp = fopen(strPath, "r");
   int nRet = ARCHIVE_READ_ERROR;
   if (fp) {
      char* pstrData = malloc(statbuf.st_size);
      if( pstrData == NULL ) {//Out of memory
         pHS->m_nLastError = ARCHIVE_OUT_OF_MEMORY;
         fclose(fp);
         return ARCHIVE_OUT_OF_MEMORY;
      }

      int nRead = fread(pstrData, 1, statbuf.st_size, fp);
      if( pstrData != NULL ) {
         if( NULL != strstr(pstrData, "<ArchiveTable version=\"1.0\"") ) {

            nRet = ReadArchiveTables(&pHS->m_pRootArchiveTables, pstrData);
         }
      }

      free(pstrData);
      fclose(fp);
   }
   return nRet;
}

int Persist(struct Archive* pHS)
{
   DEBUG_FUNC_NAME;

   int nRet = ARCHIVE_OK;

   FILE *fp = fopen(pHS->m_strArchiveFile, "w");
   if (fp) {
      const char* pstrXMLHeader = "<?xml version=\"1.0\"?>\n";
      fwrite(pstrXMLHeader, strlen(pstrXMLHeader), 1, fp);

      const char* pstrHeader = "<ArchiveTable version=\"1.0\"";
      fwrite(pstrHeader, strlen(pstrHeader), 1, fp);

      const char* pstrCloseTag = "\">\n";
      fwrite(pstrCloseTag, strlen(pstrCloseTag), 1, fp);

      nRet = PersistArchiveTables(pHS->m_pRootArchiveTables, fp);
      if( nRet != ARCHIVE_OK ) {
         pHS->m_nLastError = nRet;
         fclose(fp);
         return nRet;
      }

      const char* pstrFooter = "</ArchiveTable>\n";
      fwrite(pstrFooter, strlen(pstrFooter), 1, fp);

      fclose(fp);
   }
   else {
      pHS->m_nLastError = ARCHIVE_UNABLE_OPEN_FILE;
      return ARCHIVE_UNABLE_OPEN_FILE;
   }

   return nRet;
}

int GetArchiveError(ArchiveLib api)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;
   return pHS->m_nLastError;
}

void ClearArchiveError(ArchiveLib api)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;
   pHS->m_nLastError = ARCHIVE_OK;
}

int ArchiveSetCurrentDirectory(const char* pstr)
{
   DEBUG_FUNC_NAME;

   int nLastSlash = strlen(pstr) - 1;
   while(nLastSlash >= 0 ){
      if( *(pstr + nLastSlash) == '/' )
         break;//This had better break somewhere!
      nLastSlash--;
   }
   int n;
   for(n=0; n<=nLastSlash; n++)
      g_strCurrentPath[n] = *(pstr + n);

   return ARCHIVE_OK;
}

int ArchiveSetBatchMode(ArchiveLib api, int nState)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   if( pHS->m_nBatch == (nState ? ARCHIVE_ENABLE_BATCH : ARCHIVE_DISABLE_BATCH) )//Same mode; no effect
      return ARCHIVE_OK;

   pHS->m_nBatch = nState ? ARCHIVE_ENABLE_BATCH : ARCHIVE_DISABLE_BATCH;

   if( !pHS->m_nBatch ) {
      return Persist(pHS);//Not batching calls anymore; so persist any changes
   }

   return ARCHIVE_OK;
}

int ArchiveCopyFile(const char* pstrCopyFrom, const char* pstrCopyTo)
{
   DEBUG_FUNC_NAME;

   if( pstrCopyFrom == NULL || pstrCopyTo == NULL ) {
      return ARCHIVE_BADARGUMENT;
   }

   char strPathFrom[MAX_PATH];
   char strPathTo[MAX_PATH];

   //Does it start with a '/'; if not think of it as a relative path
   if( (*pstrCopyFrom) == '/' ) {
      strcpy(strPathFrom, "");
   }
   else {
      strcpy(strPathFrom, g_strCurrentPath);
   }
   strcat(strPathFrom, pstrCopyFrom);

   if( (*pstrCopyTo) == '/' ) {
      strcpy(strPathTo, "");
   }
   else {
      strcpy(strPathTo, g_strCurrentPath);
   }
   strcat(strPathTo, pstrCopyTo);

   FILE *fpRead = fopen(strPathFrom, "r");
   if( !fpRead ) {
      return ARCHIVE_READ_ERROR;
   }

   FILE *fpWrite = fopen(strPathTo, "w");
   if( !fpWrite ) {
      return ARCHIVE_WRITE_ERROR;
   }

   int nRet = ARCHIVE_OK;
   char buffer[256];
   while(1) {
      int nRead = fread(buffer, 1, 256, fpRead);
      if( nRead <= 0 )
         break;
      int nWrite = fwrite(buffer, 1, nRead, fpWrite);
      if( nRead != nWrite ) {
         nRet = ARCHIVE_WRITE_ERROR;
         break;
      }
   }

   fclose(fpRead);
   fclose(fpWrite);

   return nRet;
}

int ArchiveDeleteFile(const char* pstrFile)
{
   DEBUG_FUNC_NAME;

   if( pstrFile == NULL ) {
      return ARCHIVE_BADARGUMENT;
   }

   char strPath[MAX_PATH];

   //Does it start with a '/'; if not think of it as a relative path
   if( (*pstrFile) == '/' ) {
   }
   else {
      strcpy(strPath, g_strCurrentPath);
   }
   strcat(strPath, pstrFile);

   return remove(strPath) == 0 ? ARCHIVE_OK : ARCHIVE_UNABLE_DELETE_FILE;
}

// Category Related Section ///////////////////////////
const char* GetCategoryID(ArchiveLib api)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;
   return pHS->m_strCurrentCategory;
}

int SetCategoryID(ArchiveLib api, Identifier ID)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;
   strcpy(pHS->m_strCurrentCategory, ID);
   return ARCHIVE_OK;
}

int ClearCurrentCategoryID(ArchiveLib api)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;
   strcpy(pHS->m_strCurrentCategory, "");
   return ARCHIVE_OK;
}

// High Score Related Section /////////////////////////

Identifier FixIdentifier(struct Archive* pHS, Identifier ID)
{
   DEBUG_FUNC_NAME;

   if( ID == NULL ) {
      if( strlen(pHS->m_strCurrentCategory) != 0 ) {
         return pHS->m_strCurrentCategory;
      }
      return "";
   }

   return ID;
}

int GetNumberArchives(ArchiveLib api, Identifier ID)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   struct ArchiveTable* pHST = GetArchiveTable(pHS->m_pRootArchiveTables, 
       FixIdentifier(pHS, ID));
   if( pHST == NULL ) {
      pHS->m_nLastError = ARCHIVE_TABLE_NOT_CREATED;
      return 0;
   }

   int nRet = GetNumberOfEntries(pHST);

   return nRet;
}

int UpdateArchiveEntry(ArchiveLib api, Identifier ID, const char* pstrName, const char* pstrValue, int* pnIndex)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   struct ArchiveTable* pHST = GetArchiveTable(pHS->m_pRootArchiveTables, FixIdentifier(pHS, ID));
   if( pHST == NULL ) {
      pHST = CreateArchiveTable(&pHS->m_pRootArchiveTables, FixIdentifier(pHS, ID));
   }

   if( pHST == NULL ) { //If still NULL; ran out of memory
      pHS->m_nLastError = ARCHIVE_OUT_OF_MEMORY;
      return ARCHIVE_OUT_OF_MEMORY;
   }

   int nRet = UpdateArchiveItemEntry(pHST, pstrName, pstrValue, pnIndex);

   if( nRet != ARCHIVE_OK )
      return nRet;

   if( pHS->m_nBatch ) {
      return ARCHIVE_BATCHING_OK;
   }

   return Persist(pHS);
}

int ClearAllArchiveItems(ArchiveLib api, Identifier ID)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   struct ArchiveTable* pHST = GetArchiveTable(pHS->m_pRootArchiveTables, FixIdentifier(pHS, ID));
   if( pHST == NULL ) { //Doesn't exist
      pHS->m_nLastError = ARCHIVE_TABLE_NOT_CREATED;
      return ARCHIVE_TABLE_NOT_CREATED;
   }

   int nRet = ClearAllEntries(pHST);

   if( nRet != ARCHIVE_OK )
      return nRet;

   if( pHS->m_nBatch ) {
      return ARCHIVE_BATCHING_OK;
   }

   return Persist(pHS);
}

int RemoveArchiveItem(ArchiveLib api, Identifier ID, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   struct ArchiveTable* pHST = GetArchiveTable(pHS->m_pRootArchiveTables, FixIdentifier(pHS, ID));
   if( pHST == NULL ) { //Doesn't exist
      pHS->m_nLastError = ARCHIVE_TABLE_NOT_CREATED;
      return ARCHIVE_TABLE_NOT_CREATED;
   }

   int nRet = RemoveArchiveIndex(pHST, nIndex);

   if( nRet != ARCHIVE_OK )
      return nRet;

   if( pHS->m_nBatch ) {
      return ARCHIVE_BATCHING_OK;
   }

   return Persist(pHS);
}

const char* GetName(ArchiveLib api, Identifier ID, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   struct ArchiveTable* pHST = GetArchiveTable(pHS->m_pRootArchiveTables, FixIdentifier(pHS, ID));
   if( pHST == NULL ) { //Doesn't exist
      pHS->m_nLastError = ARCHIVE_TABLE_NOT_CREATED;
      return "";
   }

   return GetArchiveName(pHST, nIndex);
}

const char* GetValue(ArchiveLib api, Identifier ID, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Archive* pHS = (struct Archive*)api;

   struct ArchiveTable* pHST = GetArchiveTable(pHS->m_pRootArchiveTables, FixIdentifier(pHS, ID));
   if( pHST == NULL ) { //Doesn't exist
      pHS->m_nLastError = ARCHIVE_TABLE_NOT_CREATED;
      return "";
   }

   return GetArchiveValue(pHST, nIndex);
}

