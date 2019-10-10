#pragma once
/**
 * @class VIC2_BitmapObjectsEx
 * @author Monte Carlos
 * @date 09/10/19
 * @file vic2_bitmap_objectsex.h
 * @brief Implements some additional convenience methods which i did not
 * want to put into the base class
 */
#include "vic2_bitmap_objects_internal.h"

/// 
/// 
class VIC2_BitmapObjectsEx : public VIC2_BitmapObjectsBase{
protected:    
    inline size_t getBitIndexFromX_Y(size_t x, size_t y);
public:
    using VIC2_BitmapObjectsBase::VIC2_BitmapObjectsBase;

    virtual uint8_t shiftRightBy1 (size_t index);
    virtual uint8_t shiftRightBy2 (size_t index);
    virtual uint8_t rotateRightBy1 (size_t index, uint8_t newBits);
    virtual uint8_t rotateRightBy2 (size_t index, uint8_t newBits);
    virtual uint8_t shiftLeftBy1 (size_t index);
    virtual uint8_t shiftLeftBy2 (size_t index);
    virtual uint8_t rotateLeftBy1 (size_t index, uint8_t newBits);
    virtual uint8_t rotateLeftBy2 (size_t index, uint8_t newBits);
    
    virtual uint8_t shiftRightBy1 (const std::vector<uint8_t>::iterator &it);
    virtual uint8_t shiftRightBy2 (const std::vector<uint8_t>::iterator &it);
    virtual uint8_t rotateRightBy1 (const std::vector<uint8_t>::iterator &it, uint8_t newBits);
    virtual uint8_t rotateRightBy2 (const std::vector<uint8_t>::iterator &it, uint8_t newBits);
    virtual uint8_t shiftLeftBy1 (const std::vector<uint8_t>::iterator &it);
    virtual uint8_t shiftLeftBy2 (const std::vector<uint8_t>::iterator &it);
    virtual uint8_t rotateLeftBy1 (const std::vector<uint8_t>::iterator &it, uint8_t newBits);
    virtual uint8_t rotateLeftBy2 (const std::vector<uint8_t>::iterator &it, uint8_t newBits);

    virtual void setBit(int x, int y);
    virtual void clrBit(int x, int y);
    virtual void setBit(int x, int y, bool value);
    virtual bool getBit(int x, int y);
};

