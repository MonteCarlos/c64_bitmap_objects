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

const double PI = 3.1415926536;

//#include <cstdlib>
//Defines for uint8_t and so on, seems not to work with gcc. uint8_t and so on defined elsewhere by gcc

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

    bool setquarter ( VIC2_Bitmap_Byte_t b1, VIC2_Bitmap_Byte_t b2,
                      VIC2_Bitmap_Byte_t b3, VIC2_Bitmap_Byte_t b4, uint8_t x, uint8_t y);

    void expandquarter (VIC2_Charblock &cblk, int edge);

};

bool VIC2_Charblock::setquarter (VIC2_Bitmap_Byte_t b1, VIC2_Bitmap_Byte_t b2,
                                 VIC2_Bitmap_Byte_t b3, VIC2_Bitmap_Byte_t b4, uint8_t x, uint8_t y
                                ) {
    uint8_t rowstart = y * 4;
    uint8_t shift = (x ^ 1) * 4;

    if ( (x > 1) || (y > 1) ) return false;

    Rows[rowstart] |= b1 << shift;
    Rows[rowstart + 1] |= b2 << shift;
    Rows[rowstart + 2] |= b3 << shift;
    Rows[rowstart + 3] |= b4 << shift;

    return true;
}

void VIC2_Charblock::expandquarter (VIC2_Charblock &cblk, int edge) {
    VIC2_Bitmap_Byte_t byte;
    static uint8_t zoomtable[] = {
        0x00, 0x03, 0x0c, 0x0f, 0x30, 0x33, 0x3c, 0x3f,
        0xc0, 0xc3, 0xcc, 0xcf, 0xf0, 0xf3, 0xfc, 0xff
    };

    for (int i = 0; i < 4; ++i) {
        switch (edge) {
            case 0:
                byte = zoomtable[cblk.Rows[i] >> 4];
                break;

            case 1:
                byte = zoomtable[cblk.Rows[i] & 15];
                break;

            case 2:
                byte = zoomtable[cblk.Rows[i + 4] >> 4];
                break;

            case 3:
                byte = zoomtable[cblk.Rows[i + 4] & 15 ];

                break;

            default:
                break;
        }

        Rows[2 * i] = Rows[2 * i + 1] = byte;
    }
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
    VIC2_Bitmap bitmap (0);
    VIC2_Charblock tmpCharblock;
    int x,y,z;
    char temp;

    ofstream wfile;


    cout << "*** C64 Bitmap objects ***\n";

    try {
        cout << "Reading File chargen" << endl;
        ifstream rfile ("chargen", ios::binary);
        //rfile.read (&temp, 1);
        //rfile.read (&temp, 1);
        //Overread startaddress

        bitmap.fread (&rfile, 27 * 8);
        rfile.seekg (21 * 8, rfile.cur);
        bitmap.fread (&rfile, 10 * 8);
        rfile.close();
    }
    catch (...) {
        cout << "Error reading bitmap!" << endl;
        return 1;
    }

    try {
        cout << "Linking startup code" << endl;
        ifstream rfile ("viewbitmap.prg", ios::binary);
        wfile.open ("chargen_c64.run", ios::binary);

        while (rfile.read (&temp, 1) ) {
            wfile.write (&temp, 1);
        }

        rfile.close();
    }
    catch (...) {
        cout << "Error copying startup code!";
        return 1;
    }

    z = 0;

    for (x = 0; x<40; ++x){
        for (y = 0; y<=z; ++y){
            tmpCharblock.expandquarter(bitmap.getcharblock(x,0), 0);
            if (bitmap.getcharblock(y,2) == tmpCharblock){
                break;
            }
        }
        if (y > z){
            bitmap.getcharblock(z,2) = tmpCharblock;
            ++z;
        }
    }

    z = 0;

    for (x = 0; x<40; ++x){
        for (y = 0; y<=z; ++y){
            tmpCharblock.expandquarter(bitmap.getcharblock(x,0), 1);
            if (bitmap.getcharblock(y,3) == tmpCharblock){
                break;
            }
        }
        if (y > z){
            bitmap.getcharblock(z,3) = tmpCharblock;
            ++z;
        }
    }

    z = 0;

    for (x = 0; x<40; ++x){
        for (y = 0; y<=z; ++y){
            tmpCharblock.expandquarter(bitmap.getcharblock(x,0), 2);
            if (bitmap.getcharblock(y,4) == tmpCharblock){
                break;
            }
        }
        if (y > z){
            bitmap.getcharblock(z,4) = tmpCharblock;
            ++z;
        }
    }

    z = 0;

    for (x = 0; x<40; ++x){
        for (y = 0; y<=z; ++y){
            tmpCharblock.expandquarter(bitmap.getcharblock(x,0), 3);
            if (bitmap.getcharblock(y,5) == tmpCharblock){
                break;
            }
        }
        if (y > z){
            bitmap.getcharblock(z,5) = tmpCharblock;
            ++z;
        }
    }

    if ( !bitmap.fwrite (&wfile) ) {
        cout << "Fehler beim Schreiben der Datei " << "bitmap_c64.run";
        return 1;
    }

    wfile.close();

    try {
        cout << "Writing binary file" << endl;
        wfile.open ("chargen_c64.prg", ios::binary);
        wfile.put (0);
        wfile.put (0x20);
        bitmap.fwrite (&wfile);
        wfile.close();
    }
    catch (...) {
        cout << "Error writing binary!" << endl;
    }
}
