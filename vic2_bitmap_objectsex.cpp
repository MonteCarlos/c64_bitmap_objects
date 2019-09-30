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

