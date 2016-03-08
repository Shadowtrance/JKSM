#ifndef TITLEDATA_H
#define TITLEDATA_H

#include <3ds.h>
#include <string>
#include "smdh.h"

class titleData
{
    public:
        bool init(u64 _id, u8 mediaType);
        void initId();

        void printInfo();

        u64 id;
        u32 high, low, unique, extdata;
        std::string prodCode;
        std::u16string name, nameSafe;
        u8 media;

    private:
        smdh_s *smdh;
};

#endif // TITLEDATA_H
