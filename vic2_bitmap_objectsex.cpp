#include "vic2_bitmap_objects_internal.h"
#include <algorithm>


uint8_t VIC2_BitmapObjectsEx::shiftRightBy1 (size_t index){
    return shiftRight(index, 1);
}

uint8_t VIC2_BitmapObjectsEx::shiftRightBy2 (size_t index){
    return shiftRight(index, 2);
}

uint8_t VIC2_BitmapObjectsEx::rotateRightBy1 (size_t index, uint8_t newBits){
    return rotateRight(index, 1, newBits);
}

uint8_t VIC2_BitmapObjectsEx::rotateRightBy2 (size_t index, uint8_t newBits){
    return rotateRight(index, 2, newBits);
}

uint8_t VIC2_BitmapObjectsEx::shiftLeftBy1 (size_t index){
    return shiftRight(index, -1);
}

uint8_t VIC2_BitmapObjectsEx::shiftLeftBy2 (size_t index){
    return shiftRight(index, -2);
}

uint8_t VIC2_BitmapObjectsEx::rotateLeftBy1 (size_t index, uint8_t newBits){
    return rotateRight(index, -1, newBits);
}

uint8_t VIC2_BitmapObjectsEx::rotateLeftBy2 (size_t index, uint8_t newBits){
    return rotateRight(index, -2, newBits);
}

uint8_t VIC2_BitmapObjectsEx::shiftRightBy1 (const std::vector<uint8_t>::iterator &it){
    return shiftRightBy1(it-begin());
}

uint8_t VIC2_BitmapObjectsEx::shiftRightBy2 (const std::vector<uint8_t>::iterator &it){
    return shiftRightBy2(it-begin());
}

uint8_t VIC2_BitmapObjectsEx::rotateRightBy1 (const std::vector<uint8_t>::iterator &it, uint8_t newBits){
    return rotateRightBy1(it-begin(), newBits);
}

uint8_t VIC2_BitmapObjectsEx::rotateRightBy2 (const std::vector<uint8_t>::iterator &it, uint8_t newBits){
    return rotateRightBy2(it-begin(), newBits);
}

uint8_t VIC2_BitmapObjectsEx::shiftLeftBy1 (const std::vector<uint8_t>::iterator &it){
    return shiftLeftBy1(it-begin());
}

uint8_t VIC2_BitmapObjectsEx::shiftLeftBy2 (const std::vector<uint8_t>::iterator &it){
    return shiftLeftBy2(it-begin());
}

uint8_t VIC2_BitmapObjectsEx::rotateLeftBy1 (const std::vector<uint8_t>::iterator &it, uint8_t newBits){
    return rotateLeftBy1(it-begin(), newBits);
}

uint8_t VIC2_BitmapObjectsEx::rotateLeftBy2 (const std::vector<uint8_t>::iterator &it, uint8_t newBits){
    return rotateLeftBy2(it-begin(), newBits);
}
