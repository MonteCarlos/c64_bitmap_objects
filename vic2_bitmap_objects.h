#pragma once

#include <vector>
#include <iostream>
#include <string>

typedef uint8_t VIC2_Bitmap_Byte_t;

class VIC2_StorableBitmapBase {
private:
    std::vector<VIC2_Bitmap_Byte_t> bitmap;
public:
    VIC2_StorableBitmapBase (size_t N);

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

    virtual VIC2_Bitmap_Byte_t &operator[] (size_t index);
    virtual VIC2_StorableBitmapBase &operator= (VIC2_StorableBitmapBase &&other);
    virtual VIC2_StorableBitmapBase &operator= (VIC2_StorableBitmapBase &other);

    virtual VIC2_Bitmap_Byte_t get (size_t index);

    virtual void set (VIC2_Bitmap_Byte_t fillvalue);
    virtual void set (VIC2_StorableBitmapBase &other );

    void set (VIC2_Bitmap_Byte_t *buf);

    virtual size_t size();

    virtual std::vector<VIC2_Bitmap_Byte_t>::iterator begin();
    virtual std::vector<VIC2_Bitmap_Byte_t>::iterator end();

};