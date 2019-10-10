#include "vic2_bitmap_objectsex.h"

class VIC2_Sprite : public VIC2_BitmapObjectsEx{
    VIC2_Sprite():VIC2_BitmapObjectsEx(64){}
    
    virtual int SetPixel(int x, int y);
    virtual int GetPixel(int x, int y, bool &pix);
    
};