#include "vic2_bitmap_objects_internal.h"
#include <algorithm>

using namespace std;

VIC2_StorableBitmapBase::VIC2_StorableBitmapBase (size_t N) {
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

bool VIC2_StorableBitmapBase::fwrite (ofstream &file) {
    file.write ( (const char *) bitmap.data(), bitmap.size() );

    return file.fail();
}

bool VIC2_StorableBitmapBase::fread (ifstream &file) {
    //file.getsize();
    file.read ( (char *) bitmap.data(), bitmap.size() );

    return file.fail();
}

VIC2_Bitmap_Byte_t &VIC2_StorableBitmapBase::operator[] (size_t index) {
    return bitmap[index];
}

VIC2_Bitmap_Byte_t VIC2_StorableBitmapBase::get (size_t index) {
    return bitmap[index];
}

void VIC2_StorableBitmapBase::set (VIC2_Bitmap_Byte_t fillvalue) {
    std::fill (bitmap.begin(), bitmap.end(), fillvalue);
}

void VIC2_StorableBitmapBase::set (VIC2_Bitmap_Byte_t *buf) {
    std::copy (buf, buf + bitmap.size(), bitmap.begin() );
}

void VIC2_StorableBitmapBase::set (VIC2_StorableBitmapBase &other ) {
    bitmap.resize (other.size() );
    *this = other;
}

size_t VIC2_StorableBitmapBase::size() {
    return bitmap.size();
}

std::vector<VIC2_Bitmap_Byte_t>::iterator VIC2_StorableBitmapBase::begin() {
    return bitmap.begin();
}

std::vector<VIC2_Bitmap_Byte_t>::iterator VIC2_StorableBitmapBase::end() {
    return bitmap.end();
}

VIC2_StorableBitmapBase &VIC2_StorableBitmapBase::operator= (VIC2_StorableBitmapBase &other) {
    if (&other != this) {
        bitmap.resize (other.size() );
        std::copy (other.begin(), other.end(), bitmap.begin() );
    }

    return *this;
}

VIC2_StorableBitmapBase &VIC2_StorableBitmapBase::operator= (VIC2_StorableBitmapBase &&other) {
    return operator = (other);
}

uint8_t VIC2_StorableBitmapBase::lsr (size_t index, int n) {
    uint8_t bits;

    if (n >= 0) {
        bits = bitmap[index] & ( (2 << n) - 1);
    }
    else {
        bits = (bitmap[index] & ( ( (2 << n) - 1) << (8 - n) ) ) >> (8 - n);
    }

    bitmap[index] >>= n;
    return bits;
}

bool VIC2_StorableBitmapBase::operator == (VIC2_StorableBitmapBase &other) {
    return std::equal (other.begin(), other.end(), bitmap.begin() );
}

