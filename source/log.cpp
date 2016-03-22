#include <3ds.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "log.h"

Handle logHandle;
u64 logOffset = 0;

void logOpen()
{
    FSUSER_DeleteFile(sdArch, fsMakePath(PATH_ASCII, "/SaveDataManager/SaveDataManager.log"));
    FSUSER_OpenFile(&logHandle, sdArch, fsMakePath(PATH_ASCII, "/SaveDataManager/SaveDataManager.log"), FS_OPEN_CREATE | FS_OPEN_WRITE, 0);
}

void logClose()
{
    FSFILE_Close(logHandle);
}

void logWrite(const char *w)
{
    u32 written;
    FSFILE_Write(logHandle, &written, logOffset, w, strlen(w), FS_WRITE_FLUSH);
    logOffset += written;
}

void logWriteError(const char *w, Result r)
{
    char tmp[256];
    sprintf(tmp, "%s : %08X\n", w, (unsigned)r);

    u32 written;
    FSFILE_Write(logHandle, &written, logOffset, tmp, strlen(tmp), FS_WRITE_FLUSH);
    logOffset += written;
}
