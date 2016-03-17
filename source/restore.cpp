#include <3ds.h>
#include <string>
#include <sf2d.h>
#include <sftd.h>

#include "restore.h"
#include "archive.h"
#include "slot.h"
#include "global.h"
#include "dir.h"
#include "util.h"
#include "ui.h"

void copyFiletoArch(FS_Archive arch, const std::u16string from, const std::u16string to, int mode)
{
    Handle sdFile, archFile;
    FSUSER_OpenFile(&sdFile, sdArch, fsMakePath(PATH_UTF16, from.data()), FS_OPEN_READ, 0);
    //OpenFile will fail FS_OPEN_CREATE in ExtData
    if(mode != MODE_EXTDATA && mode != MODE_BOSS && mode != MODE_SHARED)
        FSUSER_OpenFile(&archFile, arch, fsMakePath(PATH_UTF16, to.data()), FS_OPEN_CREATE | FS_OPEN_WRITE, 0);
    else
        FSUSER_OpenFile(&archFile, arch, fsMakePath(PATH_UTF16, to.data()), FS_OPEN_WRITE, 0);

    u32 read = 0;
    u64 offset = 0;

    u64 size;
    FSFILE_GetSize(sdFile, &size);

    u8 *buff = new u8[buff_size];
    std::string copyString = "Copying " + toString(from) + "...";
    progressBar fileProg((float)size, copyString.c_str());
    do
    {
        FSFILE_Read(sdFile, &read, offset, buff, buff_size);
        FSFILE_Write(archFile, NULL, offset, buff, read, FS_WRITE_FLUSH);
        offset += read;

        //I only do this so people don't think it froze on larger files.
        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            fileProg.draw(offset);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }while(read > 0);

    delete[] buff;

    FSFILE_Close(sdFile);
    FSFILE_Close(archFile);
}

void copyDirToArch(FS_Archive arch, const std::u16string from, const std::u16string to, int mode)
{
    dirList list(sdArch, from);
    for(unsigned i = 0; i < list.count(); i++)
    {
        if(list.isDir(i))
        {
            std::u16string newFrom = from;
            newFrom += list.retItem(i);
            newFrom += L'/';

            std::u16string newTo = to;
            newTo += list.retItem(i);
            newTo += L'/';

            copyDirToArch(arch, newFrom, newTo, mode);
        }
        else
        {
            std::u16string sdPath = from;
            sdPath += list.retItem(i);

            std::u16string archPath = to;
            archPath += list.retItem(i);

            copyFiletoArch(arch, sdPath, archPath, mode);
        }
    }
}

bool restoreData(const titleData dat, FS_Archive arch, int mode)
{
    std::u16string sdPath;

    std::string keepName = GetSlot(false, dat, mode);
    if(keepName=="")
        return false;

    std::string ask = "Really restore " + keepName + "?";

    if(!confirm(ask.c_str()))
        return false;

    sdPath = getPath(mode);
    sdPath += dat.nameSafe;
    sdPath += L'/';
    sdPath += tou16(keepName.c_str());
    sdPath += L'/';

    std::u16string archPath;
    archPath += L'/';

    deleteSV(dat);

    FSUSER_DeleteDirectoryRecursively(arch, fsMakePath(PATH_ASCII, "/"));

    copyDirToArch(arch, sdPath, archPath, mode);

    //If we're not restoring some kind of extdata, commit save data
    if(mode!=MODE_EXTDATA && mode!=MODE_BOSS && mode != MODE_SHARED)
    {
        Result res = FSUSER_ControlArchive(arch, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
        if(res)
        {
            showMessage("Error committing save data!");
            logWriteError("ControlArchive Error", res);
        }
    }

    showMessage("Complete!");

    return true;
}
