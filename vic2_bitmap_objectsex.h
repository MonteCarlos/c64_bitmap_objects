#pragma once
#include "vic2_bitmap_objects_internal.h"

/// Implements some additional convenience methods not absolutely
/// necessary for a base clas
class VIC2_BitmapObjectsEx : public VIC2_BitmapObjectsBase{
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
};
