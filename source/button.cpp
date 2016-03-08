#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <string>

#include "global.h"
#include "button.h"
#include "img.h"

using namespace std;

button::button(const char *sText, int sX, int sY)
{
    Box = new textbox(sX, sY, 96, 32, sText);

    X = sX;
    Y = sY;

    Pressed = false;
}

button::~button()
{
    delete Box;
}

void button::draw()
{
    Box->draw(Pressed);
}

bool button::isOver(touchPosition p)
{
    if( (p.px > X && p.px < X + 96) && (p.py > Y && p.py < Y + 32) )
        return true;

    return false;
}

bool button::released(touchPosition p)
{
    Prev = p;
    if(isOver(p))
    {
        Pressed = true;
        return false;
    }
    else
    {
        if(Pressed)
        {
            Pressed = false;
            return true;
        }
    }
    return false;
}
