#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "smdh.h"
#include "global.h"

smdh_s *loadSMDH(u32 Low, u32 High, u8 Media)
{
    //This is for logging stuff
    char Tmp[256];
    //Pretty much stolen from hb_menu. It's the only thing I could find with how to open this.
    Handle FileHandle;

    u32 ArchPath[] = {Low, High, Media, 0x0};
    static const u32 FilePath[] = { 0x0, 0x0, 0x2, 0x6E6F6369, 0x0};
    smdh_s *Ret = new smdh_s;

    //Source formatter keeps eating these. I gave up.
    FS_Archive Arch =
    {
        ARCHIVE_SAVEDATA_AND_CONTENT, (FS_Path)
        {
            PATH_BINARY, 0x10, (u8 *)ArchPath
        }
    };
    FS_Path Path = {PATH_BINARY, 0x14, (u8 *)FilePath};

    Result Res = FSUSER_OpenFileDirectly(&FileHandle, Arch, Path, FS_OPEN_READ, 0);
    if(Res == 0)
    {
        //For bytes read.
        u32 Read;

        //Read it into Out
        FSFILE_Read(FileHandle, &Read, 0, Ret, sizeof(smdh_s));
    }
    else
    {
        sprintf(Tmp, "\tError opening smdh for %08X", (unsigned)Low);
        logWriteError(Tmp, Res);
        delete Ret;
        Ret = NULL;
    }

    FSFILE_Close(FileHandle);

    return Ret;
}
