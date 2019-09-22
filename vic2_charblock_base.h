#pragma once

#include "vic2_bitmap_objects_internal.h"

typedef uint8_t VIC2_Bitmap_Byte_t;

/*********************************************************/
/*Class for defining operations on a c64 bitmap charblock*/
/*********************************************************/
class VIC2_CharblockBase {
protected:
    VIC2_Bitmap_Byte_t Rows[8];

public:
    // Default constructor
    VIC2_CharblockBase() : VIC2_CharblockBase (0) {
    }

    // Fill constructor
    VIC2_CharblockBase (VIC2_Bitmap_Byte_t fillvalue) {
        memset ( Rows, fillvalue, sizeof (Rows) );
    }

    VIC2_Bitmap_Byte_t getbyte (uint8_t RowNr) {
        return Rows[RowNr];
    }

    uint8_t getsize (void) {
        return sizeof (Rows);
    }

    void set (VIC2_Bitmap_Byte_t (*buf[8]) ) {
        memcpy ( Rows, buf, sizeof (Rows) );
    }

    void set (VIC2_Bitmap_Byte_t fillvalue) {
        memset ( Rows, fillvalue, sizeof (Rows) );
    }

    bool fwrite (ofstream *file) {
        file->write ( (char *) Rows, sizeof (Rows) );
        return !file->fail();
    }

    bool fread (ifstream *file) {
        file->read ( (char *) Rows, sizeof (Rows) );
        return !file->fail();
    }

    void operator *= (uint8_t factor);
    bool operator == (VIC2_CharblockBase &block);
};

void VIC2_CharblockBase::operator *= (uint8_t factor) {
    for (uint8_t rowidx = 0; rowidx < sizeof (Rows); rowidx++) {
        Rows[rowidx] *= factor;
    }
}

bool VIC2_CharblockBase::operator == (VIC2_CharblockBase &block) {
    for (uint8_t rowidx = 0; rowidx < sizeof (Rows); rowidx++) {
        if (Rows[rowidx] != block.getbyte (rowidx) ) return false;
    }

    return true;
}
