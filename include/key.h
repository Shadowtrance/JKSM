#ifndef KEY_H
#define KEY_H

#include <sf2d.h>

//My shitty attempt at a keyboad.
//This is probably getting replaced sometime.

class Key
{
    public:
        void Set(int sX, int sY, char cCh);
        void SetWH(int nW, int nH);
        void ChangeChar(char nCh);

        void Print(sf2d_texture *Back);

        void DisplayChar(char nCh);
        void DisplayChar(const char *nCh);

        bool Released();
        char GetChar();
        bool On;

    private:
        int X, Y, Width, Height;
        char Ch;
        char Disp[8];
};

class KeyBoard
{
    public:
        KeyBoard();
        ~KeyBoard();
        void PrintString();
        void Print();
        bool HandleInput();
        std::string RetString();
        void Add(const char *Add);

    private:
        Key Keys[43];
        std::string Enter;
        sf2d_texture *KeyBack, *KeyBackDown, *SpaceBack, *SpaceBackDown;
};
#endif // KEY_H
