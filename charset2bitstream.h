#pragma once

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
