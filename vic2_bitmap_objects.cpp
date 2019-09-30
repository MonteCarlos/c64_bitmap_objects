#include "vic2_bitmap_objects_internal.h"
#include <algorithm>

using namespace std;

VIC2_BitmapObjectsBase::VIC2_BitmapObjectsBase (size_t N) {
    bitmap.resize (N);
}


/*int VIC2_StorableBitmapBase::fwrite (ofstream &file, size_t N) {
    file.write ( (const char*)bitmap.data(), N);

    return 0;
}

int VIC2_StorableBitmapBase::fwrite (ofstream &file, size_t N, size_t offset)  {
    file.write ( (const char *) bitmap.data()+offset, N);
    return 0;
}
*/

bool VIC2_BitmapObjectsBase::fwrite (ofstream &file) {
    file.write ( (const char *) bitmap.data(), bitmap.size() );

    return file.fail();
}

bool VIC2_BitmapObjectsBase::fread (ifstream &file) {
    //file.getsize();
    file.read ( (char *) bitmap.data(), bitmap.size() );

    return file.fail();
}

VIC2_Bitmap_Byte_t &VIC2_BitmapObjectsBase::operator[] (size_t index) {
    return bitmap[index];
}

VIC2_Bitmap_Byte_t VIC2_BitmapObjectsBase::get (size_t index) {
    return bitmap[index];
}

bool VIC2_BitmapObjectsBase::getBit (size_t bitindex) {
    return bitmap[bitindex / 8] & (0x80 >> (bitindex & 7) );
}

void VIC2_BitmapObjectsBase::set (VIC2_Bitmap_Byte_t fillvalue) {
    std::fill (bitmap.begin(), bitmap.end(), fillvalue);
}

void VIC2_BitmapObjectsBase::set (VIC2_Bitmap_Byte_t *buf) {
    std::copy (buf, buf + bitmap.size(), bitmap.begin() );
}

void VIC2_BitmapObjectsBase::set (VIC2_BitmapObjectsBase &other ) {
    if ( &other != this) {
        bitmap.resize (other.size() );
        *this = other;
    }
}

void VIC2_BitmapObjectsBase::set (size_t index, VIC2_Bitmap_Byte_t value) {
    bitmap[index] = value;
}

size_t VIC2_BitmapObjectsBase::size() {
    return bitmap.size();
}

std::vector<VIC2_Bitmap_Byte_t>::iterator VIC2_BitmapObjectsBase::begin() {
    return bitmap.begin();
}

std::vector<VIC2_Bitmap_Byte_t>::iterator VIC2_BitmapObjectsBase::end() {
    return bitmap.end();
}

VIC2_BitmapObjectsBase &VIC2_BitmapObjectsBase::operator= (VIC2_BitmapObjectsBase &other) {
    if (&other != this) {
        bitmap.resize (other.size() );

        // Needs copy as other is NOT a R-Value reference
        std::copy (other.begin(), other.end(), bitmap.begin() );
    }

    return *this;
}

VIC2_BitmapObjectsBase &VIC2_BitmapObjectsBase::operator= (VIC2_BitmapObjectsBase &&other) {
    if (&other != this) {
        bitmap.resize (other.size() );

        // Move is ok due to R-Value reference, which will be destroyed anyway
        std::move (other.begin(), other.end(), bitmap.begin() );
    }

    return *this;
}

uint8_t VIC2_BitmapObjectsBase::shiftRight (size_t index, int n) {
    uint8_t bits;
    // The mask is set-up using that 2^n-1 has all bits set below the nth bit
    uint8_t mask = (1 << n) - 1;

    // If n >=0 get the shifted out bits from right
    if (n >= 0) {
        bits = bitmap[index] & mask;
    }
    // If n < 0 get the shifted out bits from the left
    else {
        mask <<= 8-n;
        // mask out the n upper bits and make them right justified
        bits = (bitmap[index] & mask) >> (8 - n);
    }

    // Now the shift by n left or right can be performed as "lossy" bits were already saved
    bitmap[index] >>= n;
    return bits;
}

uint8_t VIC2_BitmapObjectsBase::rotateRight (size_t index, int n, uint8_t newBits) {
    // Perform shift an get lossy bits
    uint8_t bits = shiftRight (index, n);

    // Dependent on the shift direction complete the upper or lower bits with newBits
    if (n >= 0) {
        // Rotate right was performed so complete upper bits
        bitmap[index] |= (newBits << (8 - n) );
    }
    else {
        // RotateLeft was performed so complete lower bits
        bitmap[index] |= newBits;
    }

    return bits;
}

bool VIC2_BitmapObjectsBase::operator == (VIC2_BitmapObjectsBase &other) {
    return std::equal (other.begin(), other.end(), bitmap.begin() );
}

void VIC2_BitmapObjectsBase::setBit (size_t bitindex, bool value) {
    size_t byteindex = bitindex / 8;
    uint8_t mask = 0x80 >> (bitindex & 7);
    bitmap[byteindex] &= ~mask;

    if (value) {
        bitmap[byteindex] |= mask;
    }
}

void VIC2_BitmapObjectsBase::setBit (size_t bitindex) {
    setBit (bitindex, true);
}

void VIC2_BitmapObjectsBase::clrBit (size_t bitindex) {
    setBit (bitindex, false);
}
