#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string.h>
#include <string>

#include "titles.h"
#include "nand.h"
#include "backup.h"
#include "restore.h"
#include "util.h"
#include "ui.h"
#include "global.h"
#include "menu.h"
#include "titledata.h"
#include "archive.h"

void nandBackup(const titleData dat);

void nandStartSelect()
{
    menu nandMenu(80, 20, true);
    for(unsigned i = 0; i < nandTitle.size(); i++)
        nandMenu.addItem(nandTitle[i].name);

    std::u32string info = U"Select a title";
    bool loop = true;

    while(aptMainLoop() && loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        nandMenu.handleInput(up);

        if(up & KEY_A)
        {
            nandBackup(nandTitle[nandMenu.getSelected()]);
        }
        else if(up & KEY_B)
            break;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            nandMenu.draw();
            drawTopBar(info);
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            nandTitle[nandMenu.getSelected()].printInfo();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}

enum
{
    _expSys,
    _impSys,
    _expExt,
    _impExt,
    _expBoss,
    _impBoss,
    _back
};

void nandBackup(const titleData dat)
{
    menu backupMenu(112, 72, false);
    backupMenu.addItem("Export System Save");
    backupMenu.addItem("Import System Save");
    backupMenu.addItem("Export ExtData");
    backupMenu.addItem("Import ExtData");
    backupMenu.addItem("Export Boss ExtData");
    backupMenu.addItem("Import Boss ExtData");
    backupMenu.addItem("Back");

    bool loop = true;

    std::u32string info = tou32(dat.name);
    info += U" : NAND";

    while(aptMainLoop() && loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        backupMenu.handleInput(up);

        if(up & KEY_A)
        {
            FS_Archive arch;
            switch(backupMenu.getSelected())
            {
                case _expSys:
                    if(openSysSave(&arch, dat))
                    {
                        createTitleDir(dat, MODE_SYSSAVE);
                        backupData(dat, arch, MODE_SYSSAVE);
                    }
                    break;
                case _impSys:
                    if(openSysSave(&arch, dat))
                    {
                        restoreData(dat, arch, MODE_SYSSAVE);
                    }
                    break;
                case _expExt:
                    if(openExtdata(&arch, dat))
                    {
                        createTitleDir(dat, MODE_EXTDATA);
                        backupData(dat, arch, MODE_EXTDATA);
                    }
                    break;
                case _impExt:
                    if(openExtdata(&arch, dat))
                    {
                        restoreData(dat, arch, MODE_EXTDATA);
                    }
                    break;
                case _expBoss:
                    if(openBossExt(&arch, dat))
                    {
                        createTitleDir(dat, MODE_BOSS);
                        backupData(dat, arch, MODE_BOSS);
                    }
                    break;
                case _impBoss:
                    if(openBossExt(&arch, dat))
                    {
                        restoreData(dat, arch, MODE_BOSS);
                    }
                    break;
                case _back:
                    loop = false;
                    break;
            }
            FSUSER_CloseArchive(&arch);
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
