#include <3ds.h>

#include "titledata.h"
#include "archive.h"
#include "ui.h"
#include "global.h"

//All the information used here was found on 3dbrew.org
//thank them too.
bool openSaveArch(FS_Archive *out, const titleData dat, bool showError)
{
    //binary path
    u32 path[3];
    //first is mediaType
    path[0] = dat.media;
    //low id
    path[1] = dat.low;
    //high id. this is usually 0x00040000
    path[2] = dat.high;

    //setup archive info
    Handle fs;
    *out = (FS_Archive){ARCHIVE_USER_SAVEDATA, (FS_Path){PATH_BINARY, 12, path}};
    Result res = FSUSER_OpenArchive(out);
    if(res)
    {
        if(showError)
            showMessage("Error opening save archive!");
        logWriteError("Error opening save archive", res);
        return false;
    }

    return true;
}

bool openCartArch(FS_Archive *out)
{
    *out = (FS_Archive){ARCHIVE_GAMECARD_SAVEDATA, (FS_Path){PATH_EMPTY, 0, ""}};
    Result res = FSUSER_OpenArchive(out);
    if(res)
    {
        return false;
    }

    return true;
}

bool openExtdata(FS_Archive *out, const titleData dat, bool showError)
{
    u32 path[3];
    path[0] = MEDIATYPE_SD;//always sd for extdata
    path[1] = dat.extdata;
    path[2] = 0;

    *out = (FS_Archive){ARCHIVE_EXTDATA, (FS_Path){PATH_BINARY, 12, path}};
    Result res = FSUSER_OpenArchive(out);
    if(res)
    {
        if(showError)
            showMessage("Error opening ExtData! Title may not use it.");
        return false;
    }

    return true;
}

bool openSharedExt(FS_Archive *out, u32 id)
{
    u32 path[3];
    path[0] = MEDIATYPE_NAND;
    path[1] = id;
    path[2] = 0x00048000;

    *out = (FS_Archive){ARCHIVE_SHARED_EXTDATA, (FS_Path){PATH_BINARY, 12, path}};
    Result res = FSUSER_OpenArchive(out);
    if(res)
    {
        showMessage("Error opening Shared Extdata!");
        logWriteError("Error opening shared", res);
        return false;
    }

    return true;
}

bool openBossExt(FS_Archive *out, const titleData dat)
{
    u32 path[3];
    path[0] = MEDIATYPE_SD;
    path[1] = dat.extdata;
    path[2] = 0;

    *out = (FS_Archive){ARCHIVE_BOSS_EXTDATA, (FS_Path){PATH_BINARY, 12, path}};
    Result res = FSUSER_OpenArchive(out);
    if(res)
    {
        showMessage("Error opening Boss Extdata! Title may not use it.");
        return false;
    }

    return true;
}

bool openSysSave(FS_Archive *out, const titleData dat)
{
    u32 path[2];
    path[0] = MEDIATYPE_NAND;
    path[1] = (0x00020000 | dat.unique);

    *out = (FS_Archive){ARCHIVE_SYSTEM_SAVEDATA, (FS_Path){PATH_BINARY, 8, path}};
    Result res = FSUSER_OpenArchive(out);
    if(res)
    {
        showMessage("Error opening system save data! Title may not use it.");
        return false;
    }

    return true;
}
