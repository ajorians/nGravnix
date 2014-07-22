#ifndef ARCHIVEENTRY_H
#define ARCHIVEENTRY_H

#include "include/ArchiveLib.h"

struct ArchiveEntry
{
   char	 m_strName[MAX_NAME_LENGTH];
   char  m_strValue[MAX_EXTRA_DATA_LENGTH];
   struct ArchiveEntry* m_pNext;
};

struct ArchiveEntry* UpdateArchiveEntryItem(struct ArchiveEntry** ppRoot, const char* pstrName, const char* pstrValue, int* pnIndex);

int ReadArchiveEntries(struct ArchiveEntry** ppRoot, const char* pstr, const char* pstrEnd);
int PersistArchiveEntry(struct ArchiveEntry* pRoot, FILE* fp);

void FreeArchiveEntries(struct ArchiveEntry* pRoot);

int GetNumberArchiveEntries(struct ArchiveEntry* pEntry);

int RemoveEntry(struct ArchiveEntry** ppRoot, int nIndex);

const char* GetAName(struct ArchiveEntry* pRoot, int nIndex);
const char* GetAValue(struct ArchiveEntry* pRoot, int nIndex);

#endif

