#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>

#include "auto.h"
#include "menu.h"
#include "backup.h"
#include "restore.h"
#include "titles.h"
#include "ui.h"
#include "util.h"
#include "auto.h"
#include "archive.h"
#include "global.h"

void autoBackup(menu m)
{
    showMessage("This can take a few minutes depending on how many titles are selected.");


    progressBar autoDump((float)m.getSelectCount(), "Copying saves...");
    //Keep track of what's done
    float dumpCount = 0;
    for(unsigned i = 0; i < m.getSize(); i++)
    {
        //This is for titles with no save archive ex. Fantasy Life
        bool dumped = false;
        FS_Archive saveArch;
        if(m.optSelected(i) && openSaveArch(&saveArch, sdTitle[i], false))//if it's selected and we can open save archive
        {
            createTitleDir(sdTitle[i], MODE_SAVE);
            backupData(sdTitle[i], saveArch, MODE_SAVE, true);
            dumpCount++;
            dumped = true;
        }
        FSUSER_CloseArchive(&saveArch);

        FS_Archive extArch;
        if(m.optSelected(i) && openExtdata(&extArch, sdTitle[i], false))
        {
            createTitleDir(sdTitle[i], MODE_EXTDATA);
            backupData(sdTitle[i], extArch, MODE_EXTDATA, true);

            //check first to make sure we don't count it twice because no save arch
            if(!dumped)
                dumpCount++;
        }
        FSUSER_CloseArchive(&extArch);

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            autoDump.draw(i);
        sf2d_end_frame();
        sf2d_swapbuffers();
    }

    showMessage("Complete!");
}

void autoRestore(menu m)
{
    //This still needs user input.
    for(unsigned i = 0; i < m.getSize(); i++)
    {
        FS_Archive saveArch;
        if(m.optSelected(i) && openSaveArch(&saveArch, sdTitle[i], false))
            restoreData(sdTitle[i], saveArch, MODE_SAVE);
        FSUSER_CloseArchive(&saveArch);

        FS_Archive extArch;
        if(m.optSelected(i) && openExtdata(&extArch, sdTitle[i], false))
            restoreData(sdTitle[i], extArch, MODE_EXTDATA);
        FSUSER_CloseArchive(&extArch);
    }
}
