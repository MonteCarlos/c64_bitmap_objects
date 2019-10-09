#pragma once

/**
\file 

\mainpage
 
\author Monte Carlos / Cascade
\date 08.09.2019
\version 0.5.0

vic2 Bitmap Objects base class: provides functionality to access bitmaps in common. 
the methods are NOT specialized for a certain bitmap format like char or sprite or hires/mc.
to construct a bitmap object just pass the constructor the needed number of bytes.
f.e of you want to store sprite data, reserve 64 bytes, for a char reserve 8 bytes. 
specialized functionality is provided by the derived classes, too. so normally, the burden of specifying numeric literals is taken away from you.

The following base functionality is implemented:

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
It is possible to define own derived classes, too. This is shown in the demo for the csdb sprite font compo.
The base class has been used also in plasmascascade with some modifications: 
See the file PlasmaIntoBitmap.cpp, which however is not up-to-date anymore.

  
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
    // Constructors / Assignment
    VIC2_BitmapObjectsBase (size_t N);
    virtual VIC2_BitmapObjectsBase &operator= (VIC2_BitmapObjectsBase &&other);
    virtual VIC2_BitmapObjectsBase &operator= (VIC2_BitmapObjectsBase &other);
    
    // Accessors
    virtual void set (VIC2_Bitmap_Byte_t fillvalue);
    virtual void set (VIC2_BitmapObjectsBase &other );
    virtual void set (size_t index, VIC2_Bitmap_Byte_t value);
    void set (VIC2_Bitmap_Byte_t *buf); // Not virtual. Should be used only on low-level basis

    virtual VIC2_Bitmap_Byte_t &operator[] (size_t index);
    virtual VIC2_Bitmap_Byte_t get (size_t index);

    // Provided for access to bitmap
    virtual std::vector<VIC2_Bitmap_Byte_t>::iterator begin();
    virtual std::vector<VIC2_Bitmap_Byte_t>::iterator end();
    virtual size_t size();

    // Modifiers
    virtual uint8_t shiftRight (size_t index, int n);
    virtual uint8_t rotateRight (size_t index, int n, uint8_t newBits);
    virtual void setBit(size_t bitindex);
    virtual void clrBit(size_t bitindex);
    virtual void setBit(size_t bitindex, bool value);
    virtual bool getBit(size_t bitindex);

    // Comparison
    virtual bool operator == (VIC2_BitmapObjectsBase &other);

    // File reading/writing
    virtual bool fwrite (std::ofstream &file);
    virtual bool fwrite (const std::string &filename);
    virtual bool fwrite (std::ofstream &file, size_t N, size_t offset = 0);
    virtual bool fwrite (const std::string &filename, size_t N, size_t offset = 0);

    virtual bool fread (std::ifstream &file);
    virtual bool fread (std::ifstream &file, size_t N, size_t offset=0);
    virtual bool fread (std::string &filename);
    virtual bool fread (std::string &filename, size_t N, size_t offset=0);
    
    std::ifstream &openRead (std::string &filename);
    std::ofstream &openWrite (std::string &filename);
    
    // For displaying
    virtual std::string ToString(size_t width);
   
};



