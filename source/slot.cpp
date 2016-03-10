#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "slot.h"
#include "titledata.h"
#include "gstr.h"
#include "dir.h"
#include "menu.h"
#include "global.h"
#include "util.h"
#include "ui.h"
#include "button.h"

std::string ConvertToString(const std::u16string c)
{
    std::string Ret;
    for(unsigned i = 0; i < c.length(); i++)
        Ret += c[i];
    return Ret;
}

std::string GetSlot(bool nSlot, const titleData dat, int Mode)
{
	//String to return slot name
    std::string Ret = "";

    //Path
    std::u16string Path;
    Path = getPath(Mode);
    Path += dat.nameSafe;
    Path += L'/';

    dirList list(sdArch, Path);
    if(list.count()==0 && !nSlot)
        return "";

    menu getSlot(80, 20, false);
    //Go through dirs found and add them to menu
    for(unsigned i = 0; i < list.count(); i++)
    {
        getSlot.addItem(list.retItem(i));
    }
    //If we're allowing new slots
    if(nSlot)
        getSlot.addItem("New");

    button help("Help", 224, 208);

    bool Loop = true;
    std::u32string info = tou32(dat.name) + modeText(Mode);
    std::string helpText = "Select Folder. X = Rename. Y = Delete. B = Cancel";
    while(Loop)
    {
        hidScanInput();
        u32 KeyUp = hidKeysUp();

        touchPosition pos;
        hidTouchRead(&pos);

        getSlot.handleInput(KeyUp);

        if(KeyUp & KEY_A)
        {
            if((u32)(getSlot.getSelected() + 1) > list.count())
            {
                Ret = GetString();
                return Ret;
            }
            else
            {
                Ret = ConvertToString(list.retItem(getSlot.getSelected()));
                return Ret;
            }
        }
        else if(KeyUp & KEY_Y)
        {
            unsigned Sel = getSlot.getSelected();

            if(confirm("Delete selected save folder?") && Sel<list.count())
            {
                std::u16string delPath = Path;
                delPath += list.retItem(Sel);

                FSUSER_DeleteDirectoryRecursively(sdArch, fsMakePath(PATH_UTF16, delPath.data()));

                list.reassign(Path);
                if(list.count()==0 && !nSlot)
                    Loop = false;
                else
                {
                    getSlot.reset();
                    for(unsigned i = 0; i < list.count(); i++)
                        getSlot.addItem(list.retItem(i));
                    if(nSlot)
                        getSlot.addItem("New");
                }
            }
        }
        else if(KeyUp & KEY_X)
        {
            unsigned Sel = getSlot.getSelected();
            if(Sel < list.count())
            {
                std::u16string oldPath = Path;
                oldPath += list.retItem(Sel);

                std::u16string newPath = Path;
                std::string newName = GetString();
                newPath.append(tou16(newName.c_str()));

                FSUSER_RenameDirectory(sdArch, fsMakePath(PATH_UTF16, oldPath.data()), sdArch, fsMakePath(PATH_UTF16, newPath.data()));

                list.reassign(Path);

                getSlot.reset();
                for(unsigned i = 0; i < list.count(); i++)
                    getSlot.addItem(list.retItem(i));

                if(nSlot)
                    getSlot.addItem("New");
            }
        }
        else if(help.released(pos))
        {
            showMessage(helpText.c_str());
        }
        else if(KeyUp & KEY_B)
        {
            return "";
        }

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
            getSlot.draw();
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            help.draw();
        sf2d_end_frame();

        sf2d_swapbuffers();
    }
    return "";
}
