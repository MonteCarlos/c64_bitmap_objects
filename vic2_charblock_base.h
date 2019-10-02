#pragma once

#include "vic2_bitmap_objectsex.h"
#include <cstring>

/*********************************************************/
/*Class for defining operations on a c64 bitmap charblock*/
/*********************************************************/
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
};


