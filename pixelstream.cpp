#include "charset2bitstream.h"

using namespace std;

string PixelStream::ToString(void){
    stringstream strstr;
    char pattern[7] = {0};
    auto index = bitmap.end() - bitmap.begin() - 3;
    int bitCnt = 0;

    for (int ch = 25; ch >= 0; --ch) {
        for (int row = 6; row >= 0; --row) {
            pattern[4] = bitmap[index+2] & 2 ? '*' : ' ';
            pattern[5] = bitmap[index+2] & 1 ? '*' : ' ';
            pattern[2] = bitmap[index+1] & 2 ? '*' : ' ';
            pattern[3] = bitmap[index+1] & 1 ? '*' : ' ';
            pattern[0] = bitmap[index+0] & 2 ? '*' : ' ';
            pattern[1] = bitmap[index+0] & 1 ? '*' : ' ';

            shiftRightBy2(index);
            shiftRightBy2(index+1);
            shiftRightBy2(index+2);

            ++ bitCnt;

            if (bitCnt >= 4) {
                index -= 3;
                bitCnt = 0;
            }

            strstr << pattern << endl; 
        }

        strstr << endl;
    }
    return move(strstr.str());
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
void PixelStream::WriteCharset(const string &filename){
    cout << "Writing output file !" << endl;
    if ( fwrite(filename) ) {
        cout << "Fehler beim Schreiben der Datei " << filename << endl;
        exit(-1);
    }
}


// Calculated occurence count for each byte value in the "bitmap"
// Populates "histo" with the counts. Returns number of unique values.
int PixelStream::MakeHisto(vector<uint8_t> &histo){
    uint8_t countOfUniqueValues = 0;
    int count = bitmap.size();
    
    for ( int i = 0; i < count; ++i ) {
        if (0 == histo[bitmap[i]]) {
            ++countOfUniqueValues;
        }

        ++histo[bitmap[i]];
    }
    
    return countOfUniqueValues;
}