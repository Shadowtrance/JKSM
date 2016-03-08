#include <3ds.h>
#include <stdio.h>
#include <string>

#include "db.h"
#include "titledata.h"

FILE *dbCreate(const char *path)
{
    FILE *db = fopen(path, "wb");
    return db;
}

FILE *dbOpen(const char *path)
{
    FILE *db = fopen(path, "rb");
    return db;
}

void dbWriteCount(FILE *db, u32 count)
{
    u8 tmp[2];

    tmp[0] = count;
    tmp[1] = count >> 8;

    fwrite(tmp, 1, 2, db);

    fputc(0, db);
}

u32 dbGetCount(FILE *db)
{
    u32 ret = 0;
    ret += fgetc(db);
    ret += fgetc(db) << 8;

    fgetc(db);

    return ret;
}

void dbWriteData(FILE *db, const titleData w)
{
    fwrite(w.name.data(), sizeof(char16_t), w.name.length(), db);
    fputc(0, db);
    fwrite(w.prodCode.c_str(), 1, w.prodCode.length(), db);
    fputc(0, db);

    u8 idOut[8];

    for(int i = 0; i < 8; i++)
    {
        idOut[i] = (w.id >> (8 * i));
    }
    fwrite(idOut, 1, 8, db);

    fputc(0, db);
}

titleData dbGetData(FILE *db)
{
    titleData ret;
    ret.low = 0, ret.high = 0;
    char16_t next;
    while((next = fgetc(db))!=0)
    {
        next += fgetc(db) << 8;
        ret.name += next;
    }

    u8 nextChar;
    while((nextChar = fgetc(db))!=0)
        ret.prodCode += nextChar;

    for(int i = 0; i < 4; i++)
        ret.low += (fgetc(db) << (8 * i));
    for(int i = 0; i < 4; i++)
        ret.high += (fgetc(db) << (8 * i));

    fgetc(db);

    ret.initId();

    return ret;
}
