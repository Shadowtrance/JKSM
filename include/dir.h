#ifndef DIR_H
#define DIR_H

#include <string>
#include <vector>
#include <3ds.h>

class dirList
{
    public:
        dirList(FS_Archive arch, const std::u16string p);
        ~dirList();
        void reassign(const std::u16string p);

        unsigned count();

        bool isDir(int i);
        std::u16string retItem(int i);

    private:
        Handle d;
        FS_Archive a;
        std::vector<FS_DirectoryEntry> entry;
};

#endif // DIR_H
