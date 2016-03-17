#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

class textbox
{
    public:
        textbox(int x, int y, int width, int height, const char *text);
        void draw(bool Pressed);

        int X, Y;
    protected:
        int Width, Height;
        std::string Text;
};

void textboxInit();
void textboxExit();

#endif // TEXTBOX_H
