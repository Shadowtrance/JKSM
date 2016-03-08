#ifndef BUTTON_H
#define BUTTON_H

#include <3ds.h>
#include <sf2d.h>
#include <string>

#include "textbox.h"

class button
{
    public:
        button(const char *sText, int sX, int sY);
        ~button();
        void draw();
        bool isOver(touchPosition p);
        bool released(touchPosition p);

    private:
        bool Pressed;
        int X, Y;
        textbox *Box;
        touchPosition Prev;
};

#endif // BUTTON_H
