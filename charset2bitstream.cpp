#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <string>
//#include <windows.h>
#include <cmath>
#include <cstring>
#include <array>
#include <cassert>

const int spriteCount = 26;
const int xwidth = 6;
const int ywidth = 7;
const int bitsperchar = xwidth * ywidth; //42
const int totalbitcnt = bitsperchar * spriteCount; //1092
const int totalbytecount = (totalbitcnt + 4) / 8; //add 4 for proper rounding -> 137

using namespace std;

typedef uint8_t VIC2_Bitmap_Byte_t;
//typedef VIC2_Bitmap_Byte_t VIC2_CharblockBase_t[8];

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

class VIC2_Charset {
    VIC2_Charblock chars[256];
public:
    VIC2_Charblock &operator[] (int index) {
        return chars[index];
    }
    bool fread (ifstream *file);
};

bool VIC2_Charset::fread (ifstream *file) {
    for (int i = 0; i < 256; ++i) {
        chars[i].fread (file);

        if (file->fail() ) {
            break;
            return true;
        }
    }

    return false;
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

/*int openfiles(string basename)
{
}*/

int main (void) {
    VIC2_Charset srccharset;
    uint8_t destarray[totalbytecount] = { 0 };
    uint8_t padding[] = {0, 0, 0};
    uint8_t *dest = destarray + totalbytecount - 3;
    uint8_t bits;

    ofstream wfile;


    cout << "*** C64 Bitmap objects ***\n";

    try {
        cout << "Reading File chargen" << endl;
        ifstream rfile ("chargen", ios::binary);
        srccharset.fread (&rfile);
        rfile.close();
    }
    catch (...) {
        cout << "Error reading charset!" << endl;
        return 1;
    }

    int bitCnt = 0;

    for (int ch = 'z' - 'a'; ch >= 0; --ch) {
        for (int row = 6; row >= 0; --row) {
            srccharset[ch + 1].lsr1 (row);

            for (int t = 2; t >= 0; --t) {
                bits = srccharset[ch + 1].lsr2 (row);
                * (dest + t) >>= 2;
                * (dest + t) |= (bits << 6);
            }

            ++ bitCnt;

            if (bitCnt >= 4) {
                dest -= 3;
                bitCnt = 0;
            }
        }
    }

    for (; bitCnt >=0; --bitCnt) {
        for (int t = 2; t >= 0; --t) {
            bits = srccharset[1].lsr2 (0);
            * (dest + t) >>= 2;
            * (dest + t) |= (bits << 6);
        }
    }

    wfile.open ("bitstream", ios::binary);
    cout << "Writing output file !" << endl;

    /*if (0 != totalbytecount % 3) {
        if ( !wfile.write ( (char *) padding, 3 - (totalbytecount % 3) ) ) {
            cout << "Fehler beim Schreiben der Datei " << "bitstream" << endl;
            return 1;
        }
    }*/

    if ( !wfile.write ( (char *) destarray, totalbytecount) ) {
        cout << "Fehler beim Schreiben der Datei " << "bitstream" << endl;
        return 1;
    }

    wfile.close();

    char pattern[7] = {0};
    dest = destarray + totalbytecount - 3;
    bitCnt = 0;

    for (int ch = 25; ch >= 0; --ch) {
        for (int row = 6; row >= 0; --row) {
            pattern[4] = * (dest + 2) & 2 ? '*' : ' ';
            pattern[5] = * (dest + 2) & 1 ? '*' : ' ';
            pattern[2] = * (dest + 1) & 2 ? '*' : ' ';
            pattern[3] = * (dest + 1) & 1 ? '*' : ' ';
            pattern[0] = * (dest) & 2 ? '*' : ' ';
            pattern[1] = * (dest) & 1 ? '*' : ' ';

            *dest >>= 2;
            * (dest + 1) >>= 2;
            * (dest + 2) >>= 2;

            ++ bitCnt;

            if (bitCnt >= 4) {
                dest -= 3;
                bitCnt = 0;
            }

            puts (pattern);
        }

        putchar ('\n');
    }

}
