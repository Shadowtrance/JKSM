#include <3ds.h>
#include <string>
#include <string.h>

#include "global.h"
#include "util.h"
#include "titledata.h"
#include "dir.h"
#include "ui.h"

Handle fs;

std::u32string tou32(const std::u16string t)
{
    char32_t *tmp = new char32_t[256];
    memset(tmp, 0, sizeof(char32_t) * 256);

    utf16_to_utf32((uint32_t *)tmp, (uint16_t *)t.data(), 256);

    std::u32string ret;
    ret.assign(tmp);

    delete[] tmp;

    return ret;
}

std::u32string modeText(int mode)
{
    std::u32string ret;
    switch(mode)
    {
        case MODE_SAVE:
            ret = tou32(tou16(" : Save"));
            break;
        case MODE_EXTDATA:
            ret = tou32(tou16(" : ExtData"));
            break;
        case MODE_BOSS:
            ret = tou32(tou16(" : Boss ExtData"));
            break;
        case MODE_SYSSAVE:
            ret = tou32(tou16(" : System Save"));
            break;
        case MODE_SHARED:
            ret = tou32(tou16(" : Shared ExtData"));
            break;
    }
    return ret;
}

std::u16string tou16(const char *t)
{
    char16_t *tmp = new char16_t[256];
    memset(tmp, 0, sizeof(char16_t) * 256);

    utf8_to_utf16((uint16_t *)tmp, (uint8_t *)t, 256);

    std::u16string ret;
    ret.assign(tmp);

    delete[] tmp;

    return ret;
}

std::string toString(const std::u16string t)
{
    std::string ret;

    for(unsigned i = 0; i < t.length(); i++)
        ret += t[i];

    return ret;
}

void createTitleDir(const titleData t, int mode)
{
    std::u16string create = getPath(mode) + t.nameSafe;

    FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_UTF16, create.data()), 0);
}

void deleteSV(const titleData t)
{
    u64 in = ((u64)SECUREVALUE_SLOT_SD << 32) | (t.unique << 8);
    u8 out;

    Result res = FSUSER_ControlSecureSave(SECURESAVE_ACTION_DELETE, &in, 8, &out, 1);
    if(res)
    {
        showMessage("Failed to delete secure value!");
        logWriteError("DeleteSV", res);
    }
}

std::u16string getPath(int mode)
{
    std::u16string ret;
    switch(mode)
    {
        case MODE_SAVE:
            ret = tou16("/JKSV/Saves/");
            break;
        case MODE_EXTDATA:
            ret = tou16("/JKSV/ExtData/");
            break;
        case MODE_BOSS:
            ret = tou16("/JKSV/Boss/");
            break;
        case MODE_SYSSAVE:
            ret = tou16("/JKSV/SysSave/");
            break;
        case MODE_SHARED:
            ret = tou16("/JKSV/Shared/");
            break;
    }
    return ret;
}

bool runningUnder()
{
    aptOpenSession();
    u64 id;
    APT_GetProgramID(&id);
    aptCloseSession();

    //check if it's using its own ID
    if(id==0x0004000002c23200)
        return false;

    return true;
}

//Gets the handle of what it's running under and tries to use it
//I haven't found a title that works yet. Maybe I'm doing this wrong?
void fsStart()
{
   srvGetServiceHandleDirect(&fs, "fs:USER");
   FSUSER_Initialize(fs);
   fsUseSession(fs, false);
}

void fsEnd()
{
    fsEndUseSession();
}
