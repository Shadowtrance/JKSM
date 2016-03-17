#include <3ds.h>
#include <string>

#include "hbfilter.h"

//These are the homebrew IDs from the gbatemp thread - 3Dsurfer.
//3DSurfer and Picross E3 use the same ID
u32 filterID[] =
{
    0x110100,
    0x124200,
    0x133100,
    0x133600,
    0x133700,
    0x133900,
    0x192700,
    0x192800,
    0x193000,
    0x193200,
    0x210100,
    0x410200,
    0x555500,
    0x557700,
    0x610100,
    0x710200,
    0x771800,
    0x810100,
    0x910100,
    0x912500,
    0xABEE00,
    0xC1BA00,
    0xf2ee00,
    0xf69f00,
    0xfeba00,
    0x1f50400,
    0xc0d0000,
    0xda00100,
    0xda00200,
    0xda00300,
    0xe7a5a00,
    0xf002000,
    0xf002300,
    0xf002400,
    0xf002900,
    0xf003000,
    0xf0fc200,
    0xf0fc300,
    0xf104800,
    0xf718300,
    0xfaaaa00,
    0xff05000,
    0xff05100,
    0xff05300,
    0xff33300,
    0xffcfc00,
    0xc0ffee00,
    0x2c23200,
    0xf880300,
    0x2ff00,
    0xd921e00,
    0xb00000,
    0x000C9C00,
    0x000C9B00,
    0x0FF40A00,
    0x0F000700,
    0x00
};

bool hbFilter(u64 id)
{
    u32 low = (u32)id;
    //it's easier to filter retroarch this way
    char tmp[16];
    AM_GetTitleProductCode(MEDIATYPE_SD, id, tmp);
    std::string test;
    test.assign(tmp, 0, 5);
    if(test.compare("RARCH")==0)
        return true;

    int i = 0;
    while(filterID[i]!=0)
    {
        if(low==filterID[i])
            return true;

        i++;
    }

    return false;
}
