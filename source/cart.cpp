#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "archive.h"
#include "cart.h"
#include "menu.h"
#include "backup.h"
#include "restore.h"
#include "ui.h"
#include "util.h"
#include "titledata.h"
#include "global.h"

enum
{
    _expSave,
    _impSave,
    _delSave,
    _expExt,
    _impExt,
    _back
};

void cartManager()
{
    //Check if there is a cart
    bool cartInserted;
    FSUSER_CardSlotIsInserted(&cartInserted);
    if(!cartInserted)
    {
        showMessage("No cartridge inserted!");
        return;
    }

    //Make sure it's a 3ds game
    FS_CardType type;
    FSUSER_GetCardType(&type);
    if(type!=CARD_CTR)
    {
        showMessage("No DS support yet.");
        return;
    }

    //get cart's id
    u64 cartID;
    AM_GetTitleIdList(MEDIATYPE_GAME_CARD, 1, &cartID);

    //use titledata to take care of everything
    titleData cartData;
    if(!cartData.init(cartID, MEDIATYPE_GAME_CARD))
    {
        showMessage("Error getting info for cart!");
        return;
    }

    //top bar info
    std::u32string info = tou32(cartData.name);
    info += U" : Cart";

    //menu
    menu cartMenu(128, 80, false);
    cartMenu.addItem("Export Save");
    cartMenu.addItem("Import Save");
    cartMenu.addItem("Delete Save Data");
    cartMenu.addItem("Export ExtData");
    cartMenu.addItem("Import ExtData");
    cartMenu.addItem("Back");

    bool loop = true;

    while(aptMainLoop() && loop)
    {
        hidScanInput();

        u32 up = hidKeysUp();

        cartMenu.handleInput(up);

        if(up & KEY_A)
        {
            FS_Archive archive;
            switch(cartMenu.getSelected())
            {
                case _expSave:
                    if(openSaveArch(&archive, cartData, true))
                    {
                        createTitleDir(cartData, MODE_SAVE);
                        backupData(cartData, archive, MODE_SAVE, false);
                    }
                    break;
                case _impSave:
                    if(openSaveArch(&archive, cartData, true))
                    {
                        restoreData(cartData, archive, MODE_SAVE);
                    }
                    break;
                case _delSave:
                    if(openSaveArch(&archive, cartData, true) && confirm("This will delete save data present on cart. Continue?"))
                    {
                        FSUSER_DeleteDirectoryRecursively(archive, fsMakePath(PATH_ASCII, "/"));
                        FSUSER_ControlArchive(archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
                    }
                    break;
                case _expExt:
                    if(openExtdata(&archive, cartData, true))
                    {
                        createTitleDir(cartData, MODE_EXTDATA);
                        backupData(cartData, archive, MODE_EXTDATA, false);
                    }
                    break;
                case _impExt:
                    if(openExtdata(&archive, cartData, true))
                    {
                        restoreData(cartData, archive, MODE_EXTDATA);
                    }
                    break;
                case _back:
                    loop = false;
                    break;
            }
        }
        else if(up & KEY_B)
            break;

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            cartMenu.draw();
            drawTopBar(info);
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            cartData.printInfo();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
}
