#ifndef ARCHIVELIB_H_INCLUDED
#define ARCHIVELIB_H_INCLUDED

typedef void* ArchiveLib;
typedef const char* Identifier;

#define ARCHIVE_OK			(0)
#define ARCHIVE_BADARGUMENT		(-1)
#define ARCHIVE_UNABLE_OPEN_FILE	(-2)
#define ARCHIVE_OUT_OF_MEMORY		(-3)
#define ARCHIVE_UNABLE_DELETE_FILE	(-4)
#define ARCHIVE_TABLE_NOT_CREATED	(-5)
#define ARCHIVE_INVALID_INDEX		(-6)
#define ARCHIVE_READ_ERROR		(-7)
#define ARCHIVE_WRITE_ERROR		(-8)

#define ARCHIVE_BATCHING_OK		(2)

#define ARCHIVE_NO_LIMIT		(0)

#define ARCHIVE_ENABLE_BATCH		(1)
#define ARCHIVE_DISABLE_BATCH		(0)

#define MAX_IDENTIFIER_LENGTH		(32)//If not big enough; let me know; in meantime adjust and recompile!
#define MAX_NAME_LENGTH			(64)
#define MAX_EXTRA_DATA_LENGTH		(64)

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int ArchiveCreate(ArchiveLib* api);
int ArchiveFree(ArchiveLib* api);

int OpenArchiveFile(ArchiveLib api, const char* pstrFile);
int GetArchiveError(ArchiveLib api);
void ClearArchiveError(ArchiveLib api);

int ArchiveSetCurrentDirectory(const char* pstr);

int ArchiveSetBatchMode(ArchiveLib api, int nState);

int ArchiveCopyFile(const char* pstrCopyFrom, const char* pstrCopyTo);
int ArchiveDeleteFile(const char* pstrFile);

//////////////////////////////////////////////
//Category related functions
//////////////////////////////////////////////
const char* GetCategoryID(ArchiveLib api);
int SetCategoryID(ArchiveLib api, Identifier ID);
int ClearCurrentCategoryID(ArchiveLib api);

//////////////////////////////////////////////
//Archive related functions
//////////////////////////////////////////////
int GetNumberArchives(ArchiveLib api, Identifier ID);

int UpdateArchiveEntry(ArchiveLib api, Identifier ID, const char* pstrName, const char* pstrValue, int* pnIndex);

int ClearAllArchiveItems(ArchiveLib api, Identifier ID);

int RemoveArchiveItem(ArchiveLib api, Identifier ID, int nIndex);

const char* GetName(ArchiveLib api, Identifier ID, int nIndex);
const char* GetValue(ArchiveLib api, Identifier ID, int nIndex);

#endif //ARCHIVELIB_H_INCLUDED
