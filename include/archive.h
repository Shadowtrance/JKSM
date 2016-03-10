#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "titledata.h"

bool openSaveArch(FS_Archive *out, const titleData dat, bool showError);
bool openExtdata(FS_Archive *out, const titleData dat, bool showError);
bool openSharedExt(FS_Archive *out, u32 id);
bool openSysSave(FS_Archive *out, const titleData dat);
bool openBossExt(FS_Archive *out, const titleData dat);

#endif // ARCHIVE_H
