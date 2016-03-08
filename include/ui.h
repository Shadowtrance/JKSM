#ifndef UI_H
#define UI_H

#include <string>
#include "textbox.h"

bool confirm(const char *t);
void showMessage(const char *t);

class progressBar
{
    public:
        progressBar(float _max, const char *t);
        ~progressBar();
        void draw(float cur);

    private:
        textbox *back;
        float max;
};

void progressBarInit();
void progressBarExit();

#endif // UI_H
