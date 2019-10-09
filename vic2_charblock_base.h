#pragma once
/**
 * @class VIC2_CharblockBase
 * @author Monte Carlos
 * @date 09/10/19
 * @file vic2_charblock_base.h
 * @brief Class for defining operations on a c64 char matrices or charblocks of a larger bitmap
 */
#include "vic2_bitmap_objectsex.h"
#include <cstring>

class VIC2_CharblockBase : public VIC2_BitmapObjectsEx {
protected:
    enum { BYTESPERCHARBLOCK = 8 };
public:
    // Default constructor
    VIC2_CharblockBase() : VIC2_CharblockBase (0) {
    }

    // Fill constructor
    VIC2_CharblockBase (VIC2_Bitmap_Byte_t fillvalue) : VIC2_BitmapObjectsEx(BYTESPERCHARBLOCK) {
        VIC2_BitmapObjectsBase::set(fillvalue);
    }
    
    virtual std::string ToString();
};


