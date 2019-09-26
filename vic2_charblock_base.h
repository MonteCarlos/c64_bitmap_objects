#pragma once

#include "vic2_bitmap_objects.h"
#include <cstring>

/*********************************************************/
/*Class for defining operations on a c64 bitmap charblock*/
/*********************************************************/
class VIC2_CharblockBase : public VIC2_StorableBitmapBase {
protected:
    enum { BYTESPERCHARBLOCK };
    VIC2_Bitmap_Byte_t Rows[8];

public:
    // Default constructor
    VIC2_CharblockBase() : VIC2_CharblockBase (0) {
    }

    // Fill constructor
    VIC2_CharblockBase (VIC2_Bitmap_Byte_t fillvalue) : VIC2_StorableBitmapBase(BYTESPERCHARBLOCK) {
        VIC2_StorableBitmapBase::set(fillvalue);
    }

    /*VIC2_Bitmap_Byte_t get (uint8_t RowNr) {
        return bitmap[RowNr];
    }*/

    void set (VIC2_Bitmap_Byte_t (*buf[8]) ) {
        memcpy ( Rows, buf, sizeof (Rows) );
    }

    /*void set (VIC2_Bitmap_Byte_t fillvalue) {
        set ( Rows, fillvalue );
    }*/

    bool fwrite (std::ofstream &file) {
        return VIC2_StorableBitmapBase::fwrite(file);
        //file.write ( (char *) Rows, sizeof (Rows) );
        //return !file.fail();
    }

    bool fread (std::ifstream &file) {
        //file.read ( (char *) Rows, sizeof (Rows) );
        //return !file.fail();
        return VIC2_StorableBitmapBase::fread(file);
    }

    void operator *= (uint8_t factor);
    bool operator == (VIC2_CharblockBase &block);
};


