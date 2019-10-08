# c64_bitmap_objects
Classes to deal with VIC (C64) bitmap formats like chars, charsets, sprites, hires or generic ones.

vic2 Bitmap Objects base class: provides functionality to access bitmaps in common. 
the methods are NOT specialized for a certain bitmap format like char or sprite or hires/mc.
to construct a bitmap object just pass the constructor the number of bytes the normal needs.
f.e of you want to store sprite data, reserve 64 bytes, for a char reserve 8 bytes. 
specialized functionality is provided by the derived classes, too. 
so normally this burden is taken from you.

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
f.e. the (hires) bitmap class provides stream reading/writing for a certain char of the bitmap 
but also for the complete bitmap
it is possible to define own derived classes. 
this is shown in the demo for the csdb sprite font compo.
the base class has been used also in plasmascascade with some modifications
