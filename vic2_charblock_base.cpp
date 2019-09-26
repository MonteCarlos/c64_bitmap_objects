#include "vic2_charblock_base.h"

void VIC2_CharblockBase::operator *= (uint8_t factor) {
    for (uint8_t rowidx = 0; rowidx < sizeof (Rows); rowidx++) {
        Rows[rowidx] *= factor;
    }
}

bool VIC2_CharblockBase::operator == (VIC2_CharblockBase &block) {
    for (uint8_t rowidx = 0; rowidx < sizeof (Rows); rowidx++) {
        if (Rows[rowidx] != block.get (rowidx) ) return false;
    }

    return true;
}
