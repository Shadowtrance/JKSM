#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

class menuItem
{
    public:
        menuItem(const std::u32string s);

        bool selected;
        std::u32string text;
};

class menu
{
    public:
        menu(unsigned sx, unsigned sy, bool sMulti);
        ~menu();

        void addItem(const char *a);
        void addItem(const std::u16string a);
        void addItem(const std::u32string a);
        int getSelected();
        void reset();

        void draw();

        void handleInput(u32 key);

    private:
        bool multi;
        std::vector<menuItem> opts;
        int x, y, selected, start;
};

size_t u8Length(const uint8_t *d);
size_t u16Length(const uint16_t *d);
size_t u32Length(const uint32_t *d);

#endif // MENU_H
