#pragma once

/**
 * @class PixelStream
 * @author mc78
 * @date 09/10/19
 * @file charset2bitstream.h
 * @brief The Pixelstream class is a special derivation of the vic2_bitmap_objects base class.
 * It was created to support the conversion of a normal C64 1x1 Font into a format
 * suitable for efficient display in my contributions to the CSDB Sprite Font Compo 2019.
 * It adds methods for the conversion, histogram creation and disk storing.
 * The specialized ToString method displayes the converted font onto the screen (destructively!!!)
 * So make sure to call this ToString method at the end of the code not in the middle.
 */
#include "vic2_bitmap_objectsex.h"
#include "vic2_charset.h"

class PixelStream : public VIC2_BitmapObjectsEx {
public:
    PixelStream(size_t N) : VIC2_BitmapObjectsEx(N){}
    
    std::string ToString(void);
    void ConvertCharset(VIC2_Charset &srccharset);
    void WriteCharset(const string &filename);
    int MakeHisto(vector<uint8_t> &histo);
};
