#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "global.h"
#include "date.h"

sftd_font *yashi;

sf2d_texture *bar;

FS_Archive sdArch;

void drawTopBar(const std::u32string nfo)
{
    sf2d_draw_texture_scale(bar, 0, 0, 25, 1);
    sftd_draw_wtext(yashi, 0, 0, RGBA8(0, 0, 0, 255), 12, (wchar_t *)nfo.data());
    sftd_draw_text(yashi, 360, 0, RGBA8(0, 0, 0, 255), 12, RetTime().c_str());
}
