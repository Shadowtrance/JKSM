#ifndef UTIL_H
#define UTIL_H

#include <string>

#include "titledata.h"

std::u32string tou32(const std::u16string t);
std::u16string tou16(const char *t);

void createTitleDir(const titleData t, int mode);
void deleteSV(const titleData t);

#endif // UTIL_G
