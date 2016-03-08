#include <string>

#include <3ds.h>
#include <stdio.h>
#include <sf2d.h>

#include "key.h"
#include "gstr.h"
#include "global.h"
#include "date.h"
#include "textbox.h"

using namespace std;

string GetString()
{
    KeyBoard Str;
    textbox Back(0, 0, 320, 240, "");

    bool Loop = true;

    std::u32string info = U"Enter a name. Press A or Start when finished.";
    while(aptMainLoop() && Loop)
    {
        hidScanInput();

        u32 KeyUp = hidKeysUp();

        Str.HandleInput();

        if((KeyUp & KEY_START) || (KeyUp & KEY_A))
        {
            return Str.RetString();
        }
        else if(KeyUp & KEY_L)
        {
            Str.Add(GetDate(FORMAT_DMY));
        }
        else if(KeyUp & KEY_R)
        {
            Str.Add(GetDate(FORMAT_MDY));
        }
        else if(KeyUp & KEY_B)
        {
            return "";
        }

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
            drawTopBar(info);
        sf2d_end_frame();
        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            Back.draw(false);
            sftd_draw_text(yashi, 32, 32, RGBA8(0, 0, 0, 255), 12, Str.RetString().c_str());
            Str.Print();
        sf2d_end_frame();

        sf2d_swapbuffers();

    }
    return "";
}
