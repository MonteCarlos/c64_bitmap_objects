#pragma once

/**

\author Monte Carlos / Cascade
\date 08.09.2019
\version 0.5.0
    
vic2 Bitmap Objects base class: provides functionality to access bitmaps in common. 
the methods are NOT specialized for a certain bitmap format like char or sprite or hires/mc.
to construct a bitmap object just pass the constructor the number of bytes the normal needs.
f.e of you want to store sprite data, reserve 64 bytes, for a char reserve 8 bytes. 
specialized functionality is provided by the derived classes, too. so normally this burden is taken from you.

following base functionality is implemented:

- constructing bitmap object with given byte count, load/store bitmaps to io stream. manipulating bits.
- filling bitmap with a certain byte value
- setting/getting bytes of bitmap
- iterators to iterate over bytes of bitmap
- shifting bytes

the derived classes are bitmap Objects ex, charblock base, charblock, (hires)bitmap
they provide the necessary byte count to store a full char, a full charset or a full hires bitmap. 
you don't need to specify that manually.
furthermore, the methods are more specialized for the kind of objects they hold.
f.e. the (hires) bitmap class provides stream reading/writing for a certain char of the bitmap but also for the complete bitmap
it is possible to define own derived classes. this is shown in the demo for the csdb sprite font compo.
the base class has been used also in plasmascascade with some modifications
  
  \tableofcontents
**/

#include <vector>
#include <iostream>
#include <string>

/// Alias definition for a byte of a bitmap
typedef uint8_t VIC2_Bitmap_Byte_t;

/// The base class which provides common bitmap functionality like
/// - Creating a new bitmap of given size
/// - Filling of bitmap with certain byte value
/// - Setting / clearing bits
/// - Getting byte of a bitmap
/// - Getting size of the bitmap in bytes
/// - Storing bitmap to disk / Reading bitmap from disk
/// - Shifting bytes of the bitmap left/right
class VIC2_BitmapObjectsBase {
protected:
    std::vector<VIC2_Bitmap_Byte_t> bitmap;
public:
    VIC2_BitmapObjectsBase (size_t N);

    // Accessors
    virtual void set (VIC2_Bitmap_Byte_t fillvalue);
    virtual void set (VIC2_BitmapObjectsBase &other );
    void set (VIC2_Bitmap_Byte_t *buf); // Not virtual. Should be used only on low-level basis
    virtual void set (size_t index, VIC2_Bitmap_Byte_t value);

    virtual void setBit(size_t bitindex);
    virtual void clrBit(size_t bitindex);
    virtual void setBit(size_t bitindex, bool value);
    virtual bool getBit(size_t bitindex);

    virtual VIC2_Bitmap_Byte_t &operator[] (size_t index);
    virtual VIC2_Bitmap_Byte_t get (size_t index);

    // Provided for access to bitmap
    virtual std::vector<VIC2_Bitmap_Byte_t>::iterator begin();
    virtual std::vector<VIC2_Bitmap_Byte_t>::iterator end();
    virtual size_t size();

    // Modifiers
    virtual VIC2_BitmapObjectsBase &operator= (VIC2_BitmapObjectsBase &&other);
    virtual VIC2_BitmapObjectsBase &operator= (VIC2_BitmapObjectsBase &other);
    virtual uint8_t shiftRight (size_t index, int n);
    virtual uint8_t rotateRight (size_t index, int n, uint8_t newBits);

    // Comparison
    virtual bool operator == (VIC2_BitmapObjectsBase &other);

    // File reading/writing

    //virtual int fwrite (ofstream &file, size_t N);
    //virtual int fwrite (ofstream &file, size_t N, size_t offset);
    virtual bool fwrite (std::ofstream &file);

    virtual bool fwrite (const std::string &filename);
    //virtual int fwrite (string &filename, size_t N);
    //virtual int fwrite (string &filename, size_t N, size_t offset);

    std::ifstream &openRead (std::string &filename);
    std::ofstream &openWrite (std::string &filename);

    //virtual int fread (ifstream &file, size_t N);
    //virtual int fread (ifstream &file, size_t N, size_t offset);
    virtual bool fread (std::ifstream &file);
    
    virtual std::string ToString(size_t width);
    
    //virtual int fread (string &filename, size_t N);
    //virtual int fread (string &filename, size_t N, size_t offset);
    //virtual int fread (string &filename);


};



