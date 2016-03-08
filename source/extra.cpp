#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "extra.h"
#include "global.h"
#include "menu.h"
#include "archive.h"
#include "ui.h"

void maxPlayCoins()
{
    FS_Archive shared;
    if(openSharedExt(&shared, 0xf000000b))
    {
        Handle coin;
        FSUSER_OpenFile(&coin, shared, fsMakePath(PATH_ASCII, "/gamecoin.dat"), FS_OPEN_READ | FS_OPEN_WRITE, 0);

        u64 size;
        FSFILE_GetSize(coin, &size);

        u8 *buff = new u8[size];

        FSFILE_Read(coin, NULL, 0, buff, size);

        unsigned coinAmount = 300;
        buff[0x4] = coinAmount;
        buff[0x5] = coinAmount >> 8;

        FSFILE_Write(coin, NULL, 0, buff, size, FS_WRITE_FLUSH);

        delete[] buff;

        showMessage("Play coins set to 300!");
        FSUSER_CloseArchive(&shared);
    }
}

void zeroPlayCoins()
{
    FS_Archive shared;
    if(openSharedExt(&shared, 0xf000000b))
    {
        Handle coin;
        FSUSER_OpenFile(&coin, shared, fsMakePath(PATH_ASCII, "/gamecoin.dat"), FS_OPEN_READ | FS_OPEN_WRITE, 0);

        u64 size;
        FSFILE_GetSize(coin, &size);

        u8 *buff = new u8[size];

        FSFILE_Read(coin, NULL, 0, buff, size);

        buff[0x4] = 0;
        buff[0x5] = 0;

        FSFILE_Write(coin, NULL, 0, buff, size, FS_WRITE_FLUSH);

        delete[] buff;

        showMessage("Play coins set to 0.");

        FSUSER_CloseArchive(&shared);
    }
}

enum
{
    _maxPlay,
    _noPlay,
    _back
};

void extrasMenu()
{
    menu extra(128, 80, false);
    extra.addItem("300 Play coins");
    extra.addItem("0 Play coins");
    extra.addItem("Back");

    bool loop = true;

    std::u32string info = U"Extras";
    while(aptMainLoop() && loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        extra.handleInput(up);

        if(up & KEY_A)
        {
            switch(extra.getSelected())
            {
                case _maxPlay:
                    maxPlayCoins();
                    break;
                case _noPlay:
                    zeroPlayCoins();
                    break;
                case _back:
                    loop = false;
                    break;
            }
        }
        else if(up & KEY_B)
            break;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            extra.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}
