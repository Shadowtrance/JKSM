#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>
#include <vector>

#include "titles.h"
#include "menu.h"
#include "global.h"
#include "util.h"
#include "archive.h"
#include "backup.h"
#include "restore.h"
#include "ui.h"
#include "button.h"
#include "textbox.h"
#include "auto.h"

void sdBackupMenu(const titleData dat);

void sdStartSelect()
{
    if(sdTitle.size() < 1)
        return;

    menu titleMenu(88, 20, true);
    for(unsigned i = 0; i < sdTitle.size(); i++)
        titleMenu.addItem(sdTitle[i].name);

    bool loop = true;

    //Info draw on top of bar
    std::u32string info = U"Select a title";

    //help text for help button
    std::string helpText = "Press L to select multiple. Press R to select all. Press Y to backup selected. Press X to restore selected.";

    //Help button
    button help("Help", 224, 208);

    while(loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        titleMenu.handleInput(up);

        touchPosition pos;
        hidTouchRead(&pos);

        if(up & KEY_A)
        {
            unsigned selected = titleMenu.getSelected();
            sdBackupMenu(sdTitle[selected]);
        }
        else if(help.released(pos))
        {
            showMessage(helpText.c_str());
        }
        else if(up & KEY_Y)
        {
            autoBackup(titleMenu);
        }
        else if(up & KEY_X)
        {
            autoRestore(titleMenu);
        }
        else if(up & KEY_B)
            break;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            titleMenu.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            sdTitle[titleMenu.getSelected()].printInfo();
            help.draw();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}

enum
{
    _expSave,
    _impSave,
    _delSave,
    _expExt,
    _impExt,
    _back
};

void sdBackupMenu(const titleData dat)
{
    menu backupMenu(136, 80, false);
    backupMenu.addItem("Export Save");
    backupMenu.addItem("Import Save");
    backupMenu.addItem("Delete Save Data");
    backupMenu.addItem("Export ExtData");
    backupMenu.addItem("Import ExtData");
    backupMenu.addItem("Back");

    bool loop = true;

    std::u32string info = tou32(dat.name);
    info += U" : SD/CIA";
    while(loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        backupMenu.handleInput(up);

        if(up & KEY_A)
        {
            FS_Archive archive;
            switch(backupMenu.getSelected())
            {
                case _expSave:
                    if(openSaveArch(&archive, dat, true))
                    {
                        createTitleDir(dat, MODE_SAVE);
                        backupData(dat, archive, MODE_SAVE, false);
                    }
                    break;
                case _impSave:
                    if(openSaveArch(&archive, dat, true))
                    {
                        restoreData(dat, archive, MODE_SAVE);
                    }
                    break;
                case _delSave:
                    if(openSaveArch(&archive, dat, true) && confirm("This will delete game's current save data. Continue?"))
                    {
                        FSUSER_DeleteDirectoryRecursively(archive, fsMakePath(PATH_ASCII, "/"));
                        FSUSER_ControlArchive(archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
                    }
                    break;
                case _expExt:
                    if(openExtdata(&archive, dat, true))
                    {
                        createTitleDir(dat, MODE_EXTDATA);
                        backupData(dat, archive, MODE_EXTDATA, false);
                    }
                    break;
                case _impExt:
                    if(openExtdata(&archive, dat, true))
                    {
                        restoreData(dat, archive, MODE_EXTDATA);
                    }
                    break;
                case _back:
                    loop = false;
                    break;
            }
            FSUSER_CloseArchive(&archive);
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
