#ifndef SLOT_H
#define SLOT_H

#include <string>
#include "titledata.h"

//nSlot = whether to allow new dir creation
//Data = TitleData object containing info
//Ext = Using ExtData
std::string GetSlot(bool nSlot, const titleData dat, int Ext);

#endif // SLOT_H
