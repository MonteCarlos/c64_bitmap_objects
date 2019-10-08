
#include <vic2_charblock_base.h>
#include <vic2_charblock.h>
#include <vic2_charset.h>
#include <vic2_bitmap.h>
#include "charset2bitstream.h"

#include <sstream>

using namespace std;

// Enumeration to specify format in tabular output (->PrintFormatted(..))
enum class Format_t:uint8_t {FMT_NOSPECIAL, FMT_SKIPZEROES};

// Calculations for necessary amount of bits/bytes
const int spriteCount = 26;
const int xwidth = 6;
const int ywidth = 7;
const int bitsperchar = xwidth * ywidth; //42
const int totalbitcnt = bitsperchar * spriteCount; //1092
const int totalbytecount = 138;//(totalbitcnt + 4) / 8; //add 4 for proper rounding -> 137

string PixelStream::ToString(void){
    stringstream strstr;
    char pattern[7] = {0};
    auto it = this->end() - 3;
    int bitCnt = 0;

    for (int ch = 25; ch >= 0; --ch) {
        for (int row = 6; row >= 0; --row) {
            pattern[4] = it[2] & 2 ? '*' : ' ';
            pattern[5] = it[2] & 1 ? '*' : ' ';
            pattern[2] = it[1] & 2 ? '*' : ' ';
            pattern[3] = it[1] & 1 ? '*' : ' ';
            pattern[0] = it[0] & 2 ? '*' : ' ';
            pattern[1] = it[0] & 1 ? '*' : ' ';

            shiftRightBy2(it);
            shiftRightBy2(it+1);
            shiftRightBy2(it+2);

            ++ bitCnt;

            if (bitCnt >= 4) {
                it -= 3;
                bitCnt = 0;
            }

            strstr << pattern << endl; 
        }

        strstr << endl;
    }
    return move(strstr.str());
}

// Reads charset from a file "filename" from the harddisk.
// Populates "charset" buffer with the file contents
void ReadCharset(const string &filename, VIC2_Charset &charset){
    cout << "Reading charset..." << endl;
    ifstream rfile (filename, ios::binary);
    if ( charset.fread (rfile) ){
        cout << "Error reading charset!" << endl;
        exit(-1);
    }
    rfile.close();
}

// Converts charset into a specialized compact bit order for
// efficient display in my contribution to the CSDB Sprite Font Compo 2019
void PixelStream::ConvertCharset(VIC2_Charset &srccharset){
    int bitCnt = 0;
    uint8_t bits;
    uint16_t destIndex = end()-begin()-3;
    
    for (int ch = 'z' - 'a'; ch >= 0; --ch) {
        cout << "Converting Char" << ch << endl;

        for (int row = 6; row >= 0; --row) {
            cout << row << ", " << bitCnt << " # ";

            for (int t = 2; t >= 0; --t) {
                bits = srccharset[ch].shiftRightBy2(row);
                rotateRightBy2(destIndex+t, bits);
            }

            ++ bitCnt;

            if (bitCnt >= 4) {
                destIndex -= 3;
                bitCnt = 0;
            }
        }

        cout << endl;
    }

    for (; bitCnt < 4; ++bitCnt) {
        for (int t = 2; t >= 0; --t) {
            bits = srccharset[0].shiftRightBy2(0);
            rotateRightBy2(destIndex+t, bits);
        }
    }    
}

// Writes converted charset from the "pixels" buffer 
// to disk to the file speicified by "filename"
void WriteCharset(const string &filename, PixelStream &pixels){
    ofstream wfile;
    wfile.open (filename, ios::binary);
    cout << "Writing output file !" << endl;
    if ( pixels.fwrite(wfile) ) {
        cout << "Fehler beim Schreiben der Datei " << filename << endl;
        exit(-1);
    }

    wfile.close();
}

// Outputs byte vector "vec" formatted as a table
// Prints "count" bytes with "width" bytes beside each other
// More specialized formatting options can be given as "format"
void PrintFormatted(const vector<uint8_t>::iterator &vec, int count, int width = 8, const Format_t format = Format_t::FMT_NOSPECIAL){
    int col = 0;
    
    cout << "     ";
    
    for ( int i = 0; i < width; ++i) {
        cout << hex << setw(2) << setfill('0') << i << "  ";
    }
    cout << endl;
    for ( int i = 0; i < count; ++i ) {
        if (!col) {
            cout << hex << setw(2) << setfill('0') << i << ":  ";
        }
        
        cout << setfill(' ');
        
        if ( (Format_t::FMT_SKIPZEROES == format) && (0 == (int)vec[i]) ){
            cout << setw(2) << ' ' << ", ";
        }else{
            cout << hex << setw (2) << (int)vec[i] << ", ";
        }
        
        col = (col + 1) % width;

        if (!col) {
            cout << endl;
        }
    }
}

// Calculated occurence count for each byte value in the "bitmap"
// Populates "histo" with the counts. Returns number of unique values.
int MakeHisto(const vector<uint8_t>::iterator &bitmap, int count, vector<uint8_t> &histo){
    uint8_t countOfUniqueValues = 0;
    
    for ( int i = 0; i < count; ++i ) {
        if (0 == histo[bitmap[i]]) {
            ++countOfUniqueValues;
        }

        ++histo[bitmap[i]];
    }
    
    return countOfUniqueValues;
}

// Reads converted charset and displays it on the screen
// char by char
void DisplayConvertedChars(PixelStream &pixels){
    char pattern[7] = {0};
    int destIndex = pixels.size() - 3;
    int bitCnt = 0;

    for (int ch = 25; ch >= 0; --ch) {
        for (int row = 6; row >= 0; --row) {
            pattern[4] = pixels[destIndex + 2] & 2 ? '*' : ' ';
            pattern[5] = pixels[destIndex + 2] & 1 ? '*' : ' ';
            pattern[2] = pixels[destIndex + 1] & 2 ? '*' : ' ';
            pattern[3] = pixels[destIndex + 1] & 1 ? '*' : ' ';
            pattern[0] = pixels[destIndex + 0] & 2 ? '*' : ' ';
            pattern[1] = pixels[destIndex + 0] & 1 ? '*' : ' ';

            pixels.shiftRightBy2(destIndex);
            pixels.shiftRightBy2(destIndex+1);
            pixels.shiftRightBy2(destIndex+2);

            ++ bitCnt;

            if (bitCnt >= 4) {
                destIndex -= 3;
                bitCnt = 0;
            }

            cout << pattern << endl; 
        }

        cout << endl;
    }
}

int main (void) {
    VIC2_Charset srccharset;
    PixelStream pixels(totalbytecount);
    vector<uint8_t> histo(256, 0);
    uint8_t countOfUniqueValues = 0; 
    ofstream wfile;


    cout << "*** C64 Bitmap objects ***\n\n";

    ReadCharset("6x7pixcharset.bin", srccharset);
    
    // Printout 'a' and 'z' to see that charset was read correctly
    cout << srccharset[0].ToString();
    cout << endl << srccharset[25].ToString();   

    pixels.ConvertCharset(srccharset);
    
    WriteCharset("bitstream", pixels);
    
    PrintFormatted(pixels.begin(), pixels.size(), 12);
    
    countOfUniqueValues = MakeHisto(pixels.begin(), pixels.size(), histo);

    cout << endl;
    cout << "Unique value count: " << dec << (int) countOfUniqueValues << endl << endl;
    
    cout << "** Histogramme: " << endl;
    PrintFormatted(histo.begin(), histo.size(), 16, Format_t::FMT_SKIPZEROES);
    
    cout << endl << endl;
    cout << pixels.ToString();
}
