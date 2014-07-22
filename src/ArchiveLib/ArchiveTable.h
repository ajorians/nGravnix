#ifndef ARCHIVETABLE_H
#define ARCHIVETABLE_H

#include "include/ArchiveLib.h"
#include "ArchiveEntry.h"

struct ArchiveTable
{
   char m_strIdentifierName[MAX_IDENTIFIER_LENGTH];
   struct ArchiveEntry* m_pEntries;
   struct ArchiveTable* m_pNext;
};

void FreeArchiveTables(struct ArchiveTable* pRoot);

int ReadArchiveTables(struct ArchiveTable** ppRoot, const char* pstr);
int PersistArchiveTables(struct ArchiveTable* pRoot, FILE* fp);

int GetNumberOfArchiveTables(struct ArchiveTable* pRoot);

struct ArchiveTable* GetArchiveTableFromIndex(struct ArchiveTable* pRoot, int nIndex);

struct ArchiveTable* GetArchiveTable(struct ArchiveTable* pRoot, Identifier ID);

struct ArchiveTable* CreateArchiveTable(struct ArchiveTable** ppRoot, Identifier ID);

int GetNumberOfEntries(struct ArchiveTable* pTable);

int UpdateArchiveItemEntry(struct ArchiveTable* pRoot, const char* pstrName, const char* pstrValue, int* pnIndex);

int ClearAllEntries(struct ArchiveTable* pRoot);

int RemoveArchiveIndex(struct ArchiveTable* pRoot, int nIndex);

const char* GetArchiveName(struct ArchiveTable* pRoot, int nIndex);
const char* GetArchiveValue(struct ArchiveTable* pRoot, int nIndex);


#endif

