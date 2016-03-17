#include <string>

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <sf2d.h>
#include <sftd.h>

#include "key.h"
#include "img.h"
#include "global.h"

//This is my shitty quick keyboard. Probably should've used hbkblib, but I wanted to play with the touch screen.
//I need to clean this up sometime
//Still a bit weird from first release

const char Qwerty[] =
{
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '_',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', '-', '+', '_'
};

void Key::Set(int sX, int sY, char sCh)
{
    X = sX;
    Y = sY;

    Width = 8;
    Height = 8;

    Ch = sCh;
    sprintf(Disp, "%c", sCh);

    On = false;
}

void Key::SetWH(int nW, int nH)
{
    Width = nW;
    Height = nH;
}

void Key::ChangeChar(char nCh)
{
    Ch = nCh;
    sprintf(Disp, "%c", nCh);
}

void Key::Print(sf2d_texture *Back)
{
    sf2d_draw_texture(Back, X, Y);
    sftd_draw_text(yashi, X + 12, Y + 8, RGBA8(0, 0, 0, 255), 12, Disp);
}

void Key::DisplayChar(char nCh)
{
    sprintf(Disp, "%c", nCh);
}

void Key::DisplayChar(const char *nCh)
{
    sprintf(Disp, "%s", nCh);

    //Redo width
    Width = strlen(nCh) * 8;
}

bool Key::Released()
{
    touchPosition Get;

    hidTouchRead(&Get);

    if((Get.px > X && Get.px < X + Width) && (Get.py > Y && Get.py < Y + Height))
    {
        On = true;
        return false;
    }
    else
    {
        if(On)
        {
            On = false;
            return true;
        }
    }

    return false;
}

char Key::GetChar()
{
    return Ch;
}

KeyBoard::KeyBoard()
{
    KeyBack = sf2d_create_texture_mem_RGBA8(key_img.pixel_data, key_img.width, key_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    KeyBackDown = sf2d_create_texture_mem_RGBA8(key_img_down.pixel_data, key_img_down.width, key_img_down.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);

    SpaceBack = sf2d_create_texture_mem_RGBA8(spc_img.pixel_data, spc_img.width, spc_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    SpaceBackDown = sf2d_create_texture_mem_RGBA8(spc_img_down.pixel_data, spc_img_down.width, spc_img_down.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    int cChar, X, Y;
    for(cChar = 0, X = 0, Y = 80; cChar < 40; cChar++, X += 32)
    {
        Keys[cChar].Set(X, Y, Qwerty[cChar]);
        Keys[cChar].SetWH(32, 32);
        //Probably shouldn't do it this way, but whatever
        if(Qwerty[cChar]=='0' || Qwerty[cChar]=='p' || Qwerty[cChar]=='_')
        {
            X = -32;
            Y += 32;
        }
    }

    Keys[40].Set(112, 208, ' ');
    Keys[40].SetWH(96, 32);

    Keys[41].Set(224, 208, ' ');
    Keys[41].DisplayChar("Back");
    Keys[41].SetWH(96, 32);

    Keys[42].Set(0, 208, ' ');
    Keys[42].DisplayChar("Shift");
    Keys[42].SetWH(96, 32);
}

KeyBoard::~KeyBoard()
{
    sf2d_free_texture(KeyBack);
    sf2d_free_texture(KeyBackDown);
    sf2d_free_texture(SpaceBack);
    sf2d_free_texture(SpaceBackDown);
}

void KeyBoard::Print()
{
    for(int i = 0; i < 40; i++)
    {
        if(Keys[i].On)
            Keys[i].Print(KeyBackDown);
        else
            Keys[i].Print(KeyBack);
    }

    for(int i = 40; i < 43; i++)
        if(Keys[i].On)
            Keys[i].Print(SpaceBackDown);
        else
            Keys[i].Print(SpaceBack);
}

bool KeyBoard::HandleInput()
{
    for(int i = 0; i < 41; i++)
    {
        if(Keys[i].Released())
        {
            Enter += Keys[i].GetChar();
            return true;
        }
    }

    //Backspace
    if(Keys[41].Released())
    {
        if(Enter.length() > 0)
            Enter.erase(Enter.length() - 1);
        return true;
    }

    //Shift
    if(Keys[42].Released())
    {
        if(Keys[10].GetChar()=='q')
        {
            for(int i = 10; i < 40; i++)
            {
                int nChar = Keys[i].GetChar() - 32;
                Keys[i].ChangeChar(nChar);
            }
        }
        else
        {
            for(int i = 10; i < 40; i++)
            {
                int nChar = Keys[i].GetChar() + 32;
                Keys[i].ChangeChar(nChar);
            }
        }
        return true;
    }

    return false;
}

void KeyBoard::Add(const char *Add)
{
    Enter += Add;
}

std::string KeyBoard::RetString()
{
    return Enter;
}
