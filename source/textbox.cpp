#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string.h>

#include "global.h"
#include "textbox.h"
#include "img.h"

sf2d_texture *tbox, *tboxdown;

void textboxInit()
{
    tbox = sf2d_create_texture_mem_RGBA8(tbox_img.pixel_data, tbox_img.width, tbox_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    tboxdown = sf2d_create_texture_mem_RGBA8(tboxdown_img.pixel_data, 48, 48, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void textboxExit()
{
    sf2d_free_texture(tbox);
    sf2d_free_texture(tboxdown);
}

textbox::textbox(int x, int y, int width, int height, const char *text)
{
    Text.assign(text);

    X = x;
    Y = y;
    Width = width;
    Height = height;
}

void textbox::draw(bool Pressed)
{
    float xScale, yScale;

    xScale = (float)((Width - 32) / 16);
    yScale = (float)((Height - 32) / 16);

    sf2d_texture *UseTex;

    if(Pressed)
        UseTex = tboxdown;
    else
        UseTex = tbox;
    //Top
    sf2d_draw_texture_part(UseTex, X, Y, 0, 0, 16, 16);
    sf2d_draw_texture_part_scale(UseTex, X + 16, Y, 16, 0, 16, 16, xScale, 1);
    sf2d_draw_texture_part(UseTex, (X + 16) + (16 * xScale), Y, 32, 0, 16, 16);

    //Mid
    sf2d_draw_texture_part_scale(UseTex, X, Y + 16, 0, 16, 16, 16, 1, yScale);
    sf2d_draw_texture_part_scale(UseTex, X + 16, Y + 16, 16, 16, 16, 16, xScale, yScale);
    sf2d_draw_texture_part_scale(UseTex, (X + 16) + (16 * xScale), Y + 16, 32, 16, 16, 16, 1, yScale);

    //Bottom
    sf2d_draw_texture_part(UseTex, X, (Y + 16) + (16 * yScale), 0, 32, 16, 16);
    sf2d_draw_texture_part_scale(UseTex, X + 16, (Y + 16) + (16 * yScale), 16, 32, 16, 16, xScale, 1);
    sf2d_draw_texture_part(UseTex, (X + 16) + (16 * xScale), (Y + 16) + (16 * yScale), 32, 32, 16, 16);

    sftd_draw_text_wrap(yashi, X + 8, Y + 6, RGBA8(0, 0, 0, 255), 14, (X + Width - 8), Text.c_str());

}
