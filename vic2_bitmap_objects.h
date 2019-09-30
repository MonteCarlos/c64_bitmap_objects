#pragma once

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
private:
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

    //virtual int fwrite (string &filename);
    //virtual int fwrite (string &filename, size_t N);
    //virtual int fwrite (string &filename, size_t N, size_t offset);

    std::ifstream &openRead (std::string &filename);
    std::ofstream &openWrite (std::string &filename);

    //virtual int fread (ifstream &file, size_t N);
    //virtual int fread (ifstream &file, size_t N, size_t offset);
    virtual bool fread (std::ifstream &file);

    //virtual int fread (string &filename, size_t N);
    //virtual int fread (string &filename, size_t N, size_t offset);
    //virtual int fread (string &filename);


};



