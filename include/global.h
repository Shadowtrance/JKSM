#ifndef GLOBAL_H
#define GLOBAL_H

#include <sf2d.h>
#include <sftd.h>

#include <string>
#include "log.h"

//this is for backup/restore modes
#define MODE_SAVE 0
#define MODE_EXTDATA 1
#define MODE_SYSSAVE 2
#define MODE_BOSS 3

//This is the font used to print everything
extern sftd_font *yashi;

//This is the bar you see at the top
extern sf2d_texture *bar;

//SDMC Archive. Seems to write faster using this. Might just be me though
extern FS_Archive sdArch;

//This draws the bar you see at the top
void drawTopBar(const std::u32string nfo);

#endif
