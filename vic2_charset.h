#pragma once

#include "vic2_bitmap_objects_internal.h"
#include "vic2_charblock_base.h"
#include "vic2_charblock.h"

class VIC2_Charset {
    VIC2_Charblock chars[256];
public:
    VIC2_Charblock &operator[] (int index) {
        return chars[index];
    }
    bool fread (ifstream &file);
};


