#include <3ds.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "global.h"
#include "backup.h"
#include "dir.h"
#include "util.h"
#include "slot.h"
#include "ui.h"

//this is the buffer size used for copying
#define buff_size 51200

void copyFileToSD(FS_Archive save, const std::u16string from, const std::u16string to)
{
    Handle sdFile, saveFile;

    Result res = FSUSER_OpenFile(&saveFile, save, fsMakePath(PATH_UTF16, from.data()), FS_OPEN_READ, 0);
    if(res)
    {
        //error
        return;
    }

    res = FSUSER_OpenFile(&sdFile, sdArch, fsMakePath(PATH_UTF16, to.data()), FS_OPEN_CREATE | FS_OPEN_WRITE, 0);
    if(res)
    {
        //error
        return;
    }

    //store how much was read
    u32 read = 0;

    //keep track of offset
    u64 offset = 0;

    //buffer for data
    u8 *buff = new u8[buff_size];

    //loop through file until finished
    do
    {
        FSFILE_Read(saveFile, &read, offset, buff, buff_size);

        FSFILE_Write(sdFile, NULL, offset, buff, read, FS_WRITE_FLUSH);

        offset += read;
    }while(read > 0);

    delete[] buff;

    FSFILE_Close(saveFile);
    FSFILE_Close(sdFile);
}

void copyDirToSD(FS_Archive save, const std::u16string from, const std::u16string to)
{
    //Use dirList to get list of dir
    dirList list(save, from);

    progressBar prog((float)list.count(), "Copying data...");
    for(unsigned i = 0; i < list.count(); i++)
    {
        if(list.isDir(i))
        {
            std::u16string newFrom = from;
            newFrom += list.retItem(i);
            newFrom += U'/';

            std::u16string newTo = to;
            newTo += list.retItem(i);
            FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_UTF16, newTo.data()), 0);
            newTo += U'/';

            copyDirToSD(save, newFrom, newTo);
        }
        else
        {
            std::u16string fullFrom = from;
            fullFrom += list.retItem(i);

            std::u16string fullTo = to;
            fullTo += list.retItem(i);

            copyFileToSD(save, fullFrom, fullTo);
        }

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            prog.draw(i);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}

bool backupData(const titleData dat, FS_Archive arch, int mode)
{
    std::u16string pathOut;
    std::u16string slot = tou16(GetSlot(true, dat, mode).c_str());

    if(slot.data()[0]==0)
        return false;

    switch(mode)
    {
        case MODE_SAVE:
            pathOut = tou16("/JKSV/Saves/");
            break;
        case MODE_EXTDATA:
            pathOut = tou16("/JKSV/ExtData/");
            break;
        case MODE_BOSS:
            pathOut = tou16("/JKSV/Boss/");
            break;
        case MODE_SYSSAVE:
            pathOut = tou16("/JKSV/SysSave/");
            break;
    }

    pathOut += dat.nameSafe;
    pathOut += L'/';

    pathOut += slot;

    FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_UTF16, pathOut.data()), 0);

    pathOut += L'/';

    std::u16string pathIn;
    pathIn += L'/';

    copyDirToSD(arch, pathIn, pathOut);

    showMessage("Complete!");

    return true;
}
