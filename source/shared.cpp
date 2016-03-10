#include <3ds.h>
#include <string>

#include "shared.h"
#include "archive.h"
#include "backup.h"
#include "restore.h"
#include "global.h"
#include "menu.h"
#include "util.h"

//I stole these from 3dbrew. Kill me.
std::string descs[] =
{
    "From 3DBrew.org: Home Menu attempts to open this archive during boot, if FS:OpenArchive doesn't return an error Home Menu seems to then launch the System Transfer application. Home Menu doesn't actually use this archive at all except for checking whether it exists.",
    "From 3DBrew.org: NAND JPEG/MPO files and phtcache.bin from the camera application are stored here. This also contains UploadData.dat. ",
    "From 3DBrew.org: NAND M4A files from the sound application are stored here ",
    "From 3DBrew.org: ???",
    "From 3DBrew.org: Contains idb.dat, idbt.dat, gamecoin.dat, ubll.lst, CFL_DB.dat, and CFL_OldDB.dat. These files contain cleartext Miis and some data relating (including cached ICN data) to Play/Usage Records.",
    "From 3DBrew.org: Contains bashotorya.dat and bashotorya2.dat.",
    "From 3DBrew.org: ???",
    "From 3DBrew.org: Contains versionlist.dat, used by Home Menu for the software update notification added with 7.0.0-13.",
    "This means go back."
};

enum
{
    e0,
    f1,
    f2,
    f9,
    fb,
    fc,
    fd,
    fe,
    back
};

enum
{
    _exp,
    _imp,
    _back
};

void sharedBackupMenu(const titleData dat, FS_Archive arch)
{
    menu backupMenu(128, 80, false);
    backupMenu.addItem("Export");
    backupMenu.addItem("Import");
    backupMenu.addItem("Back");

    std::u32string info = tou32(dat.nameSafe) + U" : Shared Extdata";

    bool loop = true;
    while(aptMainLoop() && loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        backupMenu.handleInput(up);

        if(up & KEY_A)
        {
            switch(backupMenu.getSelected())
            {
                case _exp:
                    createTitleDir(dat, MODE_SHARED);
                    backupData(dat, arch, MODE_SHARED, false);
                    break;
                case _imp:
                    restoreData(dat, arch, MODE_SHARED);
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
            backupMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}

void sharedExtManager()
{
    menu sharedMenu(128, 72, false);
    sharedMenu.addItem("E0000000");
    sharedMenu.addItem("F0000001");
    sharedMenu.addItem("F0000002");
    sharedMenu.addItem("F0000009");
    sharedMenu.addItem("F000000B");
    sharedMenu.addItem("F000000C");
    sharedMenu.addItem("F000000D");
    sharedMenu.addItem("F000000E");
    sharedMenu.addItem("Back");

    std::u32string info = U"Shared ExtData";
    bool loop = true;
    while(aptMainLoop() && loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        sharedMenu.handleInput(up);

        if(up & KEY_A)
        {
            FS_Archive shared;
            titleData sharedDat;
            bool opened = false;
            switch(sharedMenu.getSelected())
            {
                case e0:
                    opened = openSharedExt(&shared, 0xE0000000);
                    sharedDat.nameSafe = tou16("E0000000");
                    break;
                case f1:
                    opened = openSharedExt(&shared, 0xF0000001);
                    sharedDat.nameSafe = tou16("F0000001");
                    break;
                case f2:
                    opened = openSharedExt(&shared, 0xF0000002);
                    sharedDat.nameSafe = tou16("F0000002");
                    break;
                case f9:
                    opened = openSharedExt(&shared, 0xF0000009);
                    sharedDat.nameSafe = tou16("F0000009");
                    break;
                case fb:
                    opened = openSharedExt(&shared, 0xF000000B);
                    sharedDat.nameSafe = tou16("F000000B");
                    break;
                case fc:
                    opened = openSharedExt(&shared, 0xF000000C);
                    sharedDat.nameSafe = tou16("F000000C");
                    break;
                case fd:
                    opened = openSharedExt(&shared, 0xF000000D);
                    sharedDat.nameSafe = tou16("F000000D");
                    break;
                case fe:
                    opened = openSharedExt(&shared, 0xF000000E);
                    sharedDat.nameSafe = tou16("F0000000E");
                    break;
                case back:
                    loop = false;
                    break;
            }

            if(opened)
            {
                sharedBackupMenu(sharedDat, shared);
            }
            FSUSER_CloseArchive(&shared);
        }
        else if(up & KEY_B)
            break;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            sharedMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            sftd_draw_text_wrap(yashi, 0, 0, RGBA8(255, 255, 255, 255), 12, 320, descs[sharedMenu.getSelected()].c_str());
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}
