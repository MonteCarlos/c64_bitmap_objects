#pragma once

/**
 * @class VIC2_Bitmap
 * @author Monte Carlos
 * @date 09/10/19
 * @file vic2_Bitmap.h
 * @brief Class for defining operations on a c64 hires/mc bitmap
 * @details The VIC2_Bitmap class which supports access to C64
 * Hires / Multicolor Bitmaps (only the pixels, not the color info)
 * The class does not inherit from the vic2_bitmap_objects base class.
 * However, it uses a composition of VIC2_Charblocks and a VIC2_Charblock
 * is indeed a descendant of the base class. 
 * **/
 
#include "vic2_charblock.h"

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

