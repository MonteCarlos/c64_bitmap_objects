#pragma once

/// \file 
/// 
#include "vic2_bitmap_objects.h"
#include "vic2_charblock_base.h"
#include "vic2_charblock.h"


/**********************************************************/
/* Class for defining operations on a c64 hires/mc bitmap */
/**********************************************************/
class VIC2_Bitmap {
protected:
    array <array <VIC2_Charblock, 40>, 25> Charblocks;
    size_t readpos = 0;
public:
    VIC2_Charblock& getcharblock (uint8_t charblockx, uint8_t charblocky);

    VIC2_Bitmap();
    //Default constructor

    uint8_t getxsize();

    uint8_t getysize();

    VIC2_Bitmap (VIC2_Bitmap_Byte_t fillvalue);

    bool fwrite (ofstream &file);
    bool fread (ifstream &file);
    bool fread (ifstream &file, size_t count);
};

