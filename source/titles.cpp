#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>
#include <vector>
#include <algorithm>

#include "hbfilter.h"
#include "titles.h"
#include "db.h"
#include "ui.h"
#include "global.h"

std::vector<titleData> sdTitle;
std::vector<titleData> nandTitle;


struct
{
    bool operator()(const titleData a, const titleData b)
    {
        for(unsigned i = 0; i < a.name.length(); i++)
        {
            if(a.name[i]!=b.name[i])
                return a.name[i] < b.name[i];
        }

        return false;
    }
} sortTitles;

bool fexists(const char *path)
{
    FILE *test = fopen(path, "r");
    if(test==NULL)
        return false;

    fclose(test);

    return true;
}

void sdTitlesInit()
{
    //for refresh games
    sdTitle.clear();
    if(fexists("/SaveDataManager/titles"))
    {
        FILE *read = dbOpen("/SaveDataManager/titles");
        u32 count = dbGetCount(read);

        for(unsigned i = 0; i < count; i++)
            sdTitle.push_back(dbGetData(read));

        fclose(read);
    }
    else
    {
        //get title count for sdmc
        u32 count;
        AM_GetTitleCount(MEDIATYPE_SD, &count);

        //get ids
        u64 *ids = new u64[count];
        AM_GetTitleIdList(MEDIATYPE_SD, count, ids);

        progressBar load((float)count, "Loading titles...");
        for(unsigned i = 0; i < count; i++)
        {
            u32 high = (u32)(ids[i] >> 32);
            //0x00040000 = games
            //0x00040002 = demos
            if( ((high==0x00040000 || high==0x00040002) && !hbFilter(ids[i])) || devMode)
            {
                titleData newTitle;
                if(newTitle.init(ids[i], MEDIATYPE_SD))
                    sdTitle.push_back(newTitle);
            }

            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
                load.draw(i);
            sf2d_end_frame();

            sf2d_swapbuffers();
        }
        delete[] ids;

        std::sort(sdTitle.begin(), sdTitle.end(), sortTitles);

        FILE *db = dbCreate("/SaveDataManager/titles");
        dbWriteCount(db, sdTitle.size());
        for(unsigned i = 0; i < sdTitle.size(); i++)
            dbWriteData(db, sdTitle[i]);
        fclose(db);
    }
}

bool nandFilter(u64 id)
{
    u32 low = (u32)id;
    //camera applet
    if(low==0x00009002 || low==0x00008402 || low==0x00009902 || low==0x0000AA02 || low==0x0000B202)
        return true;

    return false;
}

void sysSaveRedirect(titleData *dat)
{
    //this is for browser and ar games
    if(dat->low > 0x20000000)
        dat->unique = (0x0000FFFF & dat->unique);
    if(dat->low==0x2002CF00)
        dat->unique = 0x0000008F;
}

void nandTitlesInit()
{
    nandTitle.clear();
    if(fexists("/SaveDataManager/nand"))
    {
        FILE *read = dbOpen("/SaveDataManager/nand");
        u32 count = dbGetCount(read);

        for(unsigned i = 0; i < count; i++)
        {
            titleData newNand = dbGetData(read);
            sysSaveRedirect(&newNand);
            nandTitle.push_back(newNand);
        }

        fclose(read);
    }
    else
    {
        u32 count;
        AM_GetTitleCount(MEDIATYPE_NAND, &count);

        u64 *ids = new u64[count];
        AM_GetTitleIdList(MEDIATYPE_NAND, count, ids);

        progressBar load((float)count, "Loading NAND titles...");
        for(unsigned i = 0; i < count; i++)
        {
            if(!(nandFilter(ids[i]) && ids[i]!=0) || devMode)
            {
                titleData newData;
                if(newData.init(ids[i], MEDIATYPE_NAND) && newData.name[0]!=0)
                {
                    sysSaveRedirect(&newData);
                    nandTitle.push_back(newData);
                }
            }

            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
                load.draw(i);
            sf2d_end_frame();

            sf2d_swapbuffers();
        }
        delete[] ids;

        std::sort(nandTitle.begin(), nandTitle.end(), sortTitles);

        FILE *nand = dbCreate("/SaveDataManager/nand");
        dbWriteCount(nand, nandTitle.size());
        for(unsigned i = 0; i < nandTitle.size(); i++)
            dbWriteData(nand, nandTitle[i]);
        fclose(nand);
    }
}
