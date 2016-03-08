#include <3ds.h>
#include <string>
#include <string.h>

#include "global.h"
#include "util.h"
#include "titledata.h"
#include "dir.h"

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

void createTitleDir(const titleData t, int mode)
{
    std::u16string create;
    switch(mode)
    {
        case MODE_SAVE:
            create = tou16("/JKSV/Saves/");
            break;
        case MODE_EXTDATA:
            create = tou16("/JKSV/ExtData/");
            break;
        case MODE_BOSS:
            create = tou16("/JKSV/Boss/");
            break;
        case MODE_SYSSAVE:
            create = tou16("/JKSV/SysSave/");
            break;
    }

    create += t.nameSafe;

    FSUSER_CreateDirectory(sdArch, fsMakePath(PATH_UTF16, create.data()), 0);
}

void deleteSV(const titleData t)
{
    u64 in = ((u64)SECUREVALUE_SLOT_SD << 32) | (t.unique << 8);
    u8 out;

    FSUSER_ControlSecureSave(SECURESAVE_ACTION_DELETE, &in, 8, &out, 1);
}
