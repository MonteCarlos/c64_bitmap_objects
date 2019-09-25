#pragma once

#include "vic2_bitmap_objects_internal.h"
#include "vic2_charblock_base.h"
#include "vic2_charblock.h"

class VIC2_StorableBitmapBase {
private:
    vector<VIC2_Bitmap_Byte_t> bitmap;
public:
    VIC2_StorableBitmapBase(size_t N);

    virtual int fwrite (ofstream &file, size_t N);
    virtual int fwrite (ofstream &file, size_t N, size_t offset);
    virtual int fwrite (ofstream &file);

    virtual int fwrite (string &filename, size_t N);
    virtual int fwrite (string &filename, size_t N, size_t offset);
    virtual int fwrite (string &filename);

    ifstream &openRead(string &filename);
    ofstream &openWrite(string &filename);

    virtual int fread (ifstream &file, size_t N);
    virtual int fread (ifstream &file, size_t N, size_t offset);
    virtual int fread (ifstream &file);

    virtual int fread (string &filename, size_t N);
    virtual int fread (string &filename, size_t N, size_t offset);
    virtual int fread (string &filename);

    virtual VIC2_Bitmap_Byte_t &operator[](
};

int VIC2_StorableBitmapBase::fwrite (ofstream &file) {
    file.write ( (const char*)bitmap.data(), bitmap.size());

    return 0;
}

int VIC2_StorableBitmapBase::fwrite (ofstream &file, size_t N) {
    file.write ( (const char*)bitmap.data(), N);

    return 0;
}

int VIC2_StorableBitmapBase::fwrite (ofstream &file, size_t N, size_t offset)  {
    file.write ( (const char *) bitmap.data()+offset, N);
    return 0;
}

/*********************************************************/
/*Class for defining operations on a c64 bitmap                  */
/*********************************************************/
class VIC2_Bitmap {
private:
    array <array <VIC2_Charblock, 40>, 25> Charblocks;
    size_t readpos = 0;
public:
    VIC2_Charblock& getcharblock (uint8_t charblockx, uint8_t charblocky) {
        return Charblocks[charblocky][charblockx];
    }

    VIC2_Bitmap() {
    }
    //Default constructor

    uint8_t getxsize() {
        return Charblocks[0].size();
    }

    uint8_t getysize() {
        return Charblocks.size();
    }

    VIC2_Bitmap (VIC2_Bitmap_Byte_t fillvalue) {

        for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ) {
            for (auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ) {
                *xit = VIC2_Charblock (fillvalue);
            }
        }
    }

    bool fwrite (ofstream *file);
    bool fread (ifstream *file);
    bool fread (ifstream *file, size_t count);
};

bool VIC2_Bitmap::fwrite (ofstream *file) {
    for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ) {
        for (auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ) {
            if ( ! ( (*xit).fwrite (file) ) ) return false;
        }
    }

    return true;
}

bool VIC2_Bitmap::fread (ifstream *file, size_t count) {
    file->read ( ( (char *) &Charblocks[0][0]) + readpos, count);
    readpos += count;
    return file->fail();
}

bool VIC2_Bitmap::fread (ifstream *file) {
    for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ) {
        for ( auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ) {
            if ( ! ( (*xit).fread (file) ) ) return false;
        }
    }

    return true;
}

