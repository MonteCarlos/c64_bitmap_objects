
#include <vic2_charblock_base.h>
#include <vic2_charblock.h>
#include <vic2_charset.h>
#include <vic2_bitmap.h>

const int spriteCount = 26;
const int xwidth = 6;
const int ywidth = 7;
const int bitsperchar = xwidth * ywidth; //42
const int totalbitcnt = bitsperchar * spriteCount; //1092
const int totalbytecount = 138;//(totalbitcnt + 4) / 8; //add 4 for proper rounding -> 137




/*int openfiles(string basename)
{
}*/

class PixelStream : public VIC2_BitmapObjectsEx {
public:
    PixelStream(size_t N) : VIC2_BitmapObjectsEx(N){}
};

int main (void) {
    VIC2_Charset srccharset;
    PixelStream pixels(totalbytecount);
    uint8_t histo[256] = { 0 };
    uint8_t mappedValues[256] = { 0 };
    uint8_t countOfUniqueValues = 0;
    uint8_t bits;
    uint16_t destIndex = totalbytecount-3;

    ofstream wfile;


    cout << "*** C64 Bitmap objects ***\n";

    try {
        cout << "Reading File chargen" << endl;
        ifstream rfile ("6x7pixcharset.bin", ios::binary);
        srccharset.fread (rfile);
        rfile.close();
    }
    catch (...) {
        cout << "Error reading charset!" << endl;
        return 1;
    }

    int bitCnt = 0;

    for (int ch = 'z' - 'a'; ch >= 0; --ch) {
        printf ("Converting Char %d\n", ch);

        for (int row = 6; row >= 0; --row) {
            printf ("  %d, %d # ", row, bitCnt);
            //srccharset[ch + 1].lsr1 (row);

            for (int t = 2; t >= 0; --t) {
                bits = srccharset[ch].lsr2 (row);
                pixels.rotateRightBy2(destIndex+t, bits);
            }

            ++ bitCnt;

            if (bitCnt >= 4) {
                destIndex -= 3;
                bitCnt = 0;
            }
        }

        printf ("\n");
    }

    for (; bitCnt < 4; ++bitCnt) {
        //for (int row = 6; row >= 0; --row) {
        for (int t = 2; t >= 0; --t) {
            bits = srccharset[0].lsr2 (0);
            pixels.rotateRightBy2(destIndex+t, bits);
        }

        //}
    }

    wfile.open ("bitstream", ios::binary);
    cout << "Writing output file !" << endl;
    if ( pixels.fwrite(wfile) ) {
        cout << "Fehler beim Schreiben der Datei " << "bitstream" << endl;
        return 1;
    }

    wfile.close();
    
    int col = 0;

    for ( int i = 0; i < totalbytecount; ++i ) {
        cout << hex << setw (2) << (int) pixels[i] << ", ";

        if (!col) {
            cout << endl;
        }

        col = (col + 1) % 6;

        if (0 == histo[pixels[i]]) {
            mappedValues[pixels[i]] = countOfUniqueValues;
            ++countOfUniqueValues;
        }

        ++histo[pixels[i]];
    }

    cout << "Unique value count: " << dec << (int) countOfUniqueValues << endl;
    cout << "** Histogramme: " << endl;

    for ( int i = 255; i >= 0; --i ) {
        if ( histo[i] ) {
            cout << hex << setw (2) << i << ": " << (int) histo[i] << endl;
        }
    }

    cout << endl;


    cout << "Mapped values: " << endl;

    for ( int i = 0; i < totalbytecount; ++i ) {
        cout << (int) mappedValues[pixels[i]] << " ";
    }

    cout << endl;

    char pattern[7] = {0};
    destIndex = totalbytecount - 3;
    bitCnt = 0;

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

            puts (pattern);
        }

        putchar ('\n');
    }

}
