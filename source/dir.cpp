#include <3ds.h>
#include <string>
#include <vector>

#include "dir.h"

dirList::dirList(FS_Archive arch, const std::u16string p)
{
    a = arch;

    FSUSER_OpenDirectory(&d, a, fsMakePath(PATH_UTF16, p.data()));

    u32 read = 0;
    do
    {
        FS_DirectoryEntry getEnt;
        FSDIR_Read(d, &read, 1, &getEnt);
        entry.push_back(getEnt);
    }while(read > 0);

    FSDIR_Close(d);
}

dirList::~dirList()
{
    entry.clear();
}

void dirList::reassign(const std::u16string p)
{
    entry.clear();

    FSUSER_OpenDirectory(&d, a, fsMakePath(PATH_UTF16, p.data()));

    u32 read = 0;
    do
    {
        FS_DirectoryEntry getEnt;
        FSDIR_Read(d, &read, 1, &getEnt);
        entry.push_back(getEnt);
    }while(read > 0);

    FSDIR_Close(d);
}

unsigned dirList::count()
{
    return entry.size() - 1;
}

bool dirList::isDir(int i)
{
    if(entry[i].attributes==FS_ATTRIBUTE_DIRECTORY)
        return true;

    return false;
}

std::u16string dirList::retItem(int i)
{
    std::u16string ret;
    ret.assign((char16_t *)entry[i].name);
    return ret;
}
