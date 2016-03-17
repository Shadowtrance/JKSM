#include <3ds.h>
#include <stdio.h>
#include <string>

#include "db.h"
#include "titledata.h"

//opens, creates and returns a file for writing to.
FILE *dbCreate(const char *path)
{
    FILE *db = fopen(path, "wb");
    return db;
}

//opens and returns file for reading
FILE *dbOpen(const char *path)
{
    FILE *db = fopen(path, "rb");
    return db;
}

//This is for writing the number of titles installed
void dbWriteCount(FILE *db, u32 count)
{
    u8 tmp[2];

    tmp[0] = count;
    tmp[1] = count >> 8;

    fwrite(tmp, 1, 2, db);

    fputc(0, db);
}

//this gets the count from the beginning
//should be used first after opening for reading.
u32 dbGetCount(FILE *db)
{
    u32 ret = 0;
    ret += fgetc(db);
    ret += fgetc(db) << 8;

    fgetc(db);

    return ret;
}

//writes needed data from titleData 'w' to file
void dbWriteData(FILE *db, const titleData w)
{
    //UTF16 title
    fwrite(w.name.data(), sizeof(char16_t), w.name.length(), db);
    fputc(0, db);
    //ASCII product code
    fwrite(w.prodCode.c_str(), 1, w.prodCode.length(), db);
    fputc(0, db);

    u8 idOut[8];

    //writes the ID
    for(int i = 0; i < 8; i++)
    {
        idOut[i] = (w.id >> (8 * i));
    }
    fwrite(idOut, 1, 8, db);

    fputc(0, db);
}

//reads and returns a titleData object from db
titleData dbGetData(FILE *db)
{
    titleData ret;
    ret.low = 0, ret.high = 0;

    //keep looping and copying until we hit a 0x00
    char16_t next;
    while((next = fgetc(db))!=0)
    {
        next += fgetc(db) << 8;
        ret.name += next;
    }

    //same as above, only isn't UTF16
    u8 nextChar;
    while((nextChar = fgetc(db))!=0)
        ret.prodCode += nextChar;

    //get high and low
    for(int i = 0; i < 4; i++)
        ret.low += (fgetc(db) << (8 * i));
    for(int i = 0; i < 4; i++)
        ret.high += (fgetc(db) << (8 * i));

    //last 0x00
    fgetc(db);

    //this takes care of a few things needed
    ret.initId();

    return ret;
}
