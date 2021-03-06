#ifndef UTIL_H
#define UTIL_H

#include <string>

#include "titledata.h"

std::u32string tou32(const std::u16string t);
std::u32string modeText(int mode);
std::u16string tou16(const char *t);
std::string toString(const std::u16string t);

void createTitleDir(const titleData t, int mode);
void deleteSV(const titleData t);
std::u16string getPath(int mode);

//This detects if running under something as 3dsv
bool runningUnder();

//this is only used running under a title
void fsStart();

//same, only closing
void fsEnd();
#endif // UTIL_G
