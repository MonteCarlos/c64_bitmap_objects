
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

int main (void) {
    VIC2_Charset srccharset;
    uint8_t destarray[totalbytecount] = { 0 };
    uint8_t histo[256] = { 0 };
    uint8_t mappedValues[256] = { 0 };
    uint8_t countOfUniqueValues = 0;
    uint8_t *dest = destarray + totalbytecount - 3, *dest2;
    uint8_t bits;

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
                * (dest + t) >>= 2;
                * (dest + t) |= (bits << 6);
            }

            ++ bitCnt;

            if (bitCnt >= 4) {
                dest -= 3;
                bitCnt = 0;
            }
        }

        printf ("\n");
    }

    for (; bitCnt < 4; ++bitCnt) {
        //for (int row = 6; row >= 0; --row) {
        for (int t = 2; t >= 0; --t) {
            bits = srccharset[0].lsr2 (0);
            * (dest + t) >>= 2;
            * (dest + t) |= (bits << 6);
        }

        //}
    }

    wfile.open ("bitstream", ios::binary);
    cout << "Writing output file !" << endl;

    int col = 0;

    for ( int i = 0; i < totalbytecount; ++i ) {
        cout << hex << setw (2) << (int) dest[i] << ", ";

        if (!col) {
            cout << endl;
        }

        col = (col + 1) % 6;

        if (0 == histo[dest[i]]) {
            mappedValues[dest[i]] = countOfUniqueValues;
            ++countOfUniqueValues;
        }

        ++histo[dest[i]];
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
        cout << (int) mappedValues[dest[i]] << " ";
    }

    cout << endl;

    for ( int i = totalbytecount - 4; i >= 0; --i ) {
        for (; bitCnt < 4; ++bitCnt) {
            for (int t = 3; t >= 0; --t) {

            bits = mappedValues[i+t]&(3<<t*2)<<(8-2*t);
            * (dest2 + t) >>= 2;
            * (dest2 + t) |= (bits << 6);
            }
        }

        if ( histo[i] ) {
            cout << hex << setw (2) << i << ": " << (int) histo[i] << endl;
        }
    }

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

    //exit(0);

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
