#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <string>
#include <windows.h>
#include <cmath>
#include <cstring>
#include <array>
#include <cassert>

const double PI = 3.1415926536;

//#include <cstdlib>
//Defines for uint8_t and so on, seems not to work with gcc. uint8_t and so on defined elsewhere by gcc

using namespace std;

typedef uint8_t VIC2_Bitmap_Byte_t;
//typedef VIC2_Bitmap_Byte_t VIC2_Bitmap_Charblock_t[8];

/*********************************************************/
/*Class for defining operations on a c64 bitmap charblock*/
/*********************************************************/
class VIC2_Bitmap_Charblock
{
	private:
		VIC2_Bitmap_Byte_t Rows[8];

	public:
		VIC2_Bitmap_Byte_t getbyte(uint8_t RowNr)
		{
			return Rows[RowNr];
		}

		VIC2_Bitmap_Charblock()
		{
		}
		//Default constructor

    uint8_t getsize(void)
    {
    	return sizeof(Rows);
    }

		VIC2_Bitmap_Charblock(VIC2_Bitmap_Byte_t fillvalue)
		{
			memset( Rows, fillvalue, sizeof(Rows) );
		}

		void set(VIC2_Bitmap_Byte_t (*buf[8]))
		{
			memcpy( Rows, buf, sizeof(Rows));
		}

		void set(VIC2_Bitmap_Byte_t fillvalue)
		{
			memset( Rows, fillvalue, sizeof(Rows) );
		}

		bool setquarter( VIC2_Bitmap_Byte_t b1, VIC2_Bitmap_Byte_t b2,
										 VIC2_Bitmap_Byte_t b3, VIC2_Bitmap_Byte_t b4, uint8_t x, uint8_t y);

		bool fwrite(ofstream *file)
		{
			return file->write((char*)Rows, sizeof(Rows));
		}

		bool fread(ifstream *file)
		{
			return file->read((char*)Rows, sizeof(Rows));
		}

		void operator *= (uint8_t factor);
};

void VIC2_Bitmap_Charblock::operator *= (uint8_t factor)
{
	for (uint8_t rowidx=0; rowidx < sizeof(Rows); rowidx++)
	{
		Rows[rowidx] *= factor;
	}
}

bool VIC2_Bitmap_Charblock::setquarter(VIC2_Bitmap_Byte_t b1, VIC2_Bitmap_Byte_t b2,
																			 VIC2_Bitmap_Byte_t b3, VIC2_Bitmap_Byte_t b4, uint8_t x, uint8_t y
																			 )
{
	uint8_t rowstart = y*4;
	uint8_t shift = (x^1)*4;

	if ( (x>1) || (y>1) ) return false;
	Rows[rowstart] |= b1<<shift;
	Rows[rowstart+1] |= b2<<shift;
	Rows[rowstart+2] |= b3<<shift;
	Rows[rowstart+3] |= b4<<shift;

	return true;
}

/*********************************************************/
/*Class for defining operations on a c64 bitmap  				 */
/*********************************************************/
class VIC2_Bitmap
{
	private:
		array <array <VIC2_Bitmap_Charblock, 40>, 25> Charblocks;
	public:
		VIC2_Bitmap_Charblock& getcharblock(uint8_t charblockx, uint8_t charblocky)
		{
			return Charblocks[charblocky][charblockx];
		}

		VIC2_Bitmap()
		{
		}
		//Default constructor

		uint8_t getxsize()
		{
			return Charblocks[0].size();
		}

		uint8_t getysize()
		{
			return Charblocks.size();
		}

		VIC2_Bitmap(VIC2_Bitmap_Byte_t fillvalue)
		{
			for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ){
				for (auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ){
					*xit = VIC2_Bitmap_Charblock(fillvalue);
				}
			}
		}

		bool fwrite(ofstream *file);
		bool fread(ifstream *file);
};

bool VIC2_Bitmap::fwrite(ofstream *file)
{
	for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ){
		for (auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ){
			if ( !( (*xit).fwrite(file) ) ) return false;
		}
	}

	return true;
}

bool VIC2_Bitmap::fread(ifstream *file)
{
	for ( auto yit = Charblocks.begin(); yit < Charblocks.end(); ++yit ){
		for ( auto xit = (*yit).begin(); xit < (*yit).end(); ++xit ){
			if ( !( (*xit).fread(file) ) ) return false;
			//cout << '.';
			//read charblock. fread also exists in class VIC2_Bitmap_Charblock
		}
		//cout << '\n';
	}

	return true;
}

/*int openfiles(string basename)
{
}*/

int main(void)
{
	VIC2_Bitmap bitmap(0);

	uint8_t x,y, bitindex;
	char temp;
	VIC2_Bitmap_Byte_t bitcombos1[] = {0x0f, 0x0f, 0x0e, 0x0e,
																		0x0a, 0x0a, 0x09, 0x09,
																		0x05, 0x05, 0x04, 0x04,
																		0x00, 0x00, 0x00, 0x00,
																		0x04, 0x04, 0x05, 0x05,
																		0x09, 0x09, 0x0a, 0x0a,
																		0x0e, 0x0e, 0x0f, 0x0f};

	VIC2_Bitmap_Byte_t bitcombos2[] = {0x0f, 0x0f, 0x0b, 0x0b,
																		0x0a, 0x0a, 0x06, 0x06,
																		0x05, 0x05, 0x01, 0x01,
																		0x00, 0x00, 0x00, 0x00,
																		0x01, 0x01, 0x05, 0x05,
																		0x06, 0x06, 0x0a, 0x0a,
																		0x0b, 0x0b, 0x0f, 0x0f};
	//VIC2_Bitmap_Byte_t bitbuffer[8];

	float bmx, bmy;
	ofstream wfile;

	assert (sizeof(bitcombos1) == sizeof(bitcombos2));
	//These bitcombinations for the c64 bitmap depend on each other (for dithering) and the size must be equal!

	cout << "*** C64 Bitmap objects ***\n";

	try
	{
		ifstream rfile("cascade - crl.prg", ios::binary);
		rfile.read(&temp,1);
		rfile.read(&temp,1);
		//Overread startaddress

		bitmap.fread(&rfile);
		rfile.close();
	}
	catch (...)
	{
		cout << "Error reading bitmap!"; return 1;
	}

	try
	{
		ifstream rfile("viewbitmap.prg", ios::binary);
		wfile.open("bitmap_c64.run", ios::binary);
		while(rfile.read(&temp, 1))
		{
			wfile.write(&temp, 1);
		}
		rfile.close();
	}
	catch(...)
	{
		cout << "Error copying startup code!"; return 1;
	}

	for (y = 0; y < 2*bitmap.getysize(); y++)
	{
		bmy = PI*y/bitmap.getysize();
		for (x = 12; x < 2*bitmap.getxsize(); x++)
		{
			bmx = PI*x/bitmap.getxsize();
			bitindex = ( (int)( 21.+7.5*sin(2.5*bmy)+3.5*sin(3*bmy)+6*cos(3*bmx)+4*cos(2*bmx)) ) % sizeof(bitcombos1);

			bitmap.getcharblock(
					(int)x/2,(int)y/2 ).
					setquarter( bitcombos1[bitindex], bitcombos2[bitindex], bitcombos1[bitindex], bitcombos2[bitindex],
											x&1, y&1 )
			;
		}
	}

	if ( !bitmap.fwrite(&wfile) ){cout << "Fehler beim Schreiben der Datei " << "bitmap_c64.run"; return 1;}
	wfile.close();
	wfile.open("bitmap_c64.prg", ios::binary);
	wfile.put(0);
	wfile.put(0x20);
	bitmap.fwrite(&wfile);
	wfile.close();
	
}
