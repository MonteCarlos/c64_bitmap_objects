#include "vic2_bitmap.h"

VIC2_Charblock& VIC2_Bitmap::getcharblock (uint8_t charblockx, uint8_t charblocky) {
    return Charblocks[charblocky][charblockx];
}

VIC2_Bitmap::VIC2_Bitmap() {
}
//Default constructor

uint8_t VIC2_Bitmap::getxsize() {
    return Charblocks[0].size();
}

uint8_t VIC2_Bitmap::getysize() {
    return Charblocks.size();
}

VIC2_Bitmap::VIC2_Bitmap (VIC2_Bitmap_Byte_t fillvalue) {

    for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ) {
        for (auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ) {
            *xit = VIC2_Charblock (fillvalue);
        }
    }
}
    
bool VIC2_Bitmap::fwrite (ofstream &file) {
    for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ) {
        for (auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ) {
            if ( ! ( (*xit).fwrite (file) ) ) return false;
        }
    }

    return true;
}

bool VIC2_Bitmap::fread (ifstream &file, size_t count) {
    file.read ( ( (char *) &Charblocks[0][0]) + readpos, count);
    readpos += count;
    return file.fail();
}

bool VIC2_Bitmap::fread (ifstream &file) {
    for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ) {
        for ( auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ) {
            if ( ! ( (*xit).fread (file) ) ) return false;
        }
    }

    return true;
}

