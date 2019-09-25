#pragma once

#include "vic2_charblock_base.h"

class VIC2_Charblock: public VIC2_CharblockBase {
public:
    using VIC2_CharblockBase::VIC2_CharblockBase;

    uint8_t lsr2 (int row) {
        uint8_t bits = Rows[row] & 3;
        Rows[row] >>= 2;
        return bits;
    }

    uint8_t lsr1 (int row) {
        uint8_t bit = Rows[row] & 1;
        Rows[row] >>= 1;
        return bit;
    }
};
