#ifndef LOG_H
#define LOG_H

#include <3ds.h>

void logOpen();
void logClose();
void logWrite(const char *w);
void logWriteError(const char *w, Result r);

#endif // LOG_H
