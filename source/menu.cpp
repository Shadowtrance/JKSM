#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>
#include <string.h>

#include "global.h"
#include "menu.h"
#include "img.h"

#define RED RGBA8(255, 0, 0, 255)
#define BLUE RGBA8(0, 0, 255, 255)
#define GREEN RGBA8(0, 255, 0, 255)
#define DARK_GRAY RGBA8(128, 128, 128, 255)
#define FONT_SIZE 12

sf2d_texture *arrow;

void loadArrow()
{
    arrow = sf2d_create_texture_mem_RGBA8(arrow_img.pixel_data, arrow_img.width, arrow_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void freeArrow()
{
    sf2d_free_texture(arrow);
}

menuItem::menuItem(const std::u32string s)
{
    selected = false;
    text.assign(s);
}

menu::menu(unsigned sx, unsigned sy, bool sMulti)
{
    x = sx;
    y = sy;
    multi = sMulti;
    selected = 0;
    start = 0;
}

menu::~menu()
{
    opts.clear();
}

void menu::addItem(const char *a)
{
    char32_t *tmp = new char32_t[128];
    memset(tmp, 0, sizeof(char32_t) * 128);
    utf8_to_utf32((uint32_t *)tmp, (uint8_t *)a, 128);

    std::u32string t;
    t.assign(tmp);

    menuItem add(t);

    opts.push_back(add);

    delete[] tmp;
}

void menu::addItem(const std::u16string a)
{
    char32_t *tmp = new char32_t[128];
    memset(tmp, 0, sizeof(char32_t) * 128);
    utf16_to_utf32((uint32_t *)tmp, (uint16_t *)a.data(), 128);

    std::u32string t;
    t.assign(tmp);

    menuItem add(t);

    opts.push_back(add);

    delete[] tmp;
}

void menu::draw()
{
    int i, length;

    //Set length for printing
    if((opts.size() - 1) < 15)
        length = opts.size();
    else
        //Scrolling needs start set
        length = start + 15;

    //For when using multiple menus
    for(i = start; i < length; i++)
    {
        //\x1b[COLOR;1m = bold color.
        if(i == selected)
        {
            sf2d_draw_texture(arrow, x, (y + 3) + (i - start) * 14);
            sftd_draw_wtext(yashi, x + 24, y + ((i - start) * 14), GREEN, FONT_SIZE, (wchar_t *)opts[i].text.data());
        }
        else if(opts[i].selected)
            sftd_draw_wtext(yashi, x + 24, y + ((i - start) * 14), RGBA8(200, 0, 0, 255), 12, (wchar_t *)opts[i].text.data());
        else
            sftd_draw_wtext(yashi, x + 24, y + ((i - start) * 14), DARK_GRAY, FONT_SIZE, (wchar_t *)opts[i].text.data());
    }

}

void menu::handleInput(u32 key)
{
    //Honestly. I don't even know anymore.
    //I stopped touching this. It works, but it's a mess.
    int size = opts.size() - 1;
    if(key & KEY_UP)
    {
        selected--;
        if(selected < 0)
            selected = size;

        if((start > selected)  && (start > 0))
            start--;
        if(size < 15)
            start = 0;
        else if(selected == size && size > 15)
            start = size - 14;
    }
    else if(key & KEY_DOWN)
    {
        selected++;
        if(selected > size)
            selected = 0;

        if((selected > (start + 14)) && ((start + 14) < size))
            start++;
        if(selected == 0)
            start = 0;
    }
    else if(key & KEY_RIGHT)
    {
        selected += 7;
        if(selected > size)
            selected = size;
        if((selected - 14) > start)
            start = selected - 14;
    }
    else if(key & KEY_LEFT)
    {
        selected -= 7;
        if(selected < 0)
            selected = 0;
        if(selected < start)
            start = selected;
    }
    else if((key & KEY_L) && multi)
    {
        if(opts[selected].selected)
            opts[selected].selected = false;
        else
            opts[selected].selected = true;
    }
    else if((key & KEY_R) && multi)
    {
        if(opts[0].selected && opts[1].selected)
        {
            for(unsigned i = 0; i < opts.size(); i++)
                opts[i].selected = false;
        }
        else
        {
            for(unsigned i = 0; i < opts.size(); i++)
                opts[i].selected = true;
        }
    }

}

void menu::reset()
{
    selected = 0;
    opts.clear();
}

int menu::getSelected()
{
    return selected;
}

unsigned menu::getSize()
{
    return opts.size();
}

unsigned menu::getSelectCount()
{
    unsigned ret = 0;
    for(unsigned i = 0; i < opts.size(); i++)
    {
        if(opts[i].selected)
            ret++;
    }

    return ret;
}

bool menu::optSelected(int i)
{
    return opts[i].selected;
}
