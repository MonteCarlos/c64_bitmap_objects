.include "cc65runtime.sh"
.include "myMacros.sh"

spriteCount = 37
charset = $d000
sprites = $2000

.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import _loBitsTable, _hiBitsTable
.import fillGarbage

lo = tmp2
hi = tmp3
tmp5 = ptr3+1

ByteOfSpriteMatrixIdx = 3

    lsr $01

    jsr fillGarbage

    lax #0
    sax ptr2
    sax ptr1
    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1
gensprite0:
    lda #<(sprites+(spriteCount-1)*64)+62
    sta ByteOfSpriteMatrixIdx

gensprite:
    ldy charnum

    ldx #3
    lda src1,y
    axs #0
    lsr
    lsr
    sta src1,y

    lda expandtbl,x
    sta lo

    ldx #3
    lda src2,y
    axs #0
    lsr
    lsr
    sta src2,y

    lda expandtbl,x
    sta hi

    ldx #3
    lda src3,y
    axs #0
    lsr
    lsr
    sta src3,y

    lda expandtbl,x
    sta hi+1

    lda #2
    sta tmp1
    ldx lineindex
setsprite0:
    ldy ByteOfSpriteMatrixIdx
setsprite:
    lda lo
    ;and raster,x
    sta (ptr2),y
    dey
    lda hi
    ;and raster,x
    sta (ptr2),y
    dey
    lda hi+1
    ;and raster,x
    sta (ptr2),y
    dex
    dey
    dec tmp1
    bpl setsprite

    txa
    bpl :+
    dec charnum
    bmi end
    ldx #19
    dey
    tya
    bpl :+
    dec ptr2+1
:   sty ByteOfSpriteMatrixIdx
    stx lineindex

    lsr bitmask
    jcc gensprite
    lda #%00000100
    sta bitmask
    jpl gensprite
end:
    rts
bitmask:
    .byte %00000100
reptable:
    .byte 0,1,2,2,2,1,0,0

.endproc

.SEGMENT "SETTINGS"
.export multicol = *
    .byte   $ff

.export bgcol = *
    .byte   $00

.export sprcol = *
    .byte   $05

.export sprmcol1 = *
    .byte   $0a

.export sprmcol2 = *
    .byte   $0e

.DATA
charnum:
    .byte 36/4
lineindex:
    .byte 19
raster:
    .byte %11111111 ;green
    .byte %11111111

    .byte %11101110 ;dither
    .byte %11111111 ;dither2

    .byte %10111011 ;rose
    .byte %11101110 ;green

    .byte %10101010 ;rose
    .byte %10111011

    .byte %10101010 ;rose
    .byte %10101010 ;rose

    .byte %10011001 ;rose
    .byte %10101010 ;dither 1

    .byte %01100110 ;dither 2
    .byte %10011001 ;grey

    .byte %01010101 ;rose
    .byte %01100110 ;grey

    .byte %01010101 ;grey
    .byte %01010101 ;grey

    .byte %01010101 ;grey
    .byte %01010101 ;grey
    .byte %01010101 ;grey
expandhi:
   ;         33  22    11  00
    .byte %00000000  ;%00000000
    .byte %00000000  ;%00000001
    .byte %00000000 ;%00000010
    .byte %00000000  ;%00000011

    .byte %00000110  ;%00000100
    .byte %00000110  ;%00000101
    .byte %00000110  ;%00000110
    .byte %00000110  ;%00000111

    .byte %01100100  ;%00001000
    .byte %01100100 ;%00001001
    .byte %01100100 ;%00001010
    .byte %01100110  ;%00001011

    .byte %01101010 ;%00001100
    .byte %01101010  ;%00001101
    .byte %01101010  ;%00001110
    .byte %01101010  ;%00001111

    src1:
    .byte %01111101, %10000111, %11111110, %11000001, %10000000
    src2:
    .byte %01111101, %10000111, %11111110, %11000001, %10000000
    src3:
    .byte %01111101, %10000111, %11111110, %11000001, %10000000

    expandtbl:
    .byte %00000000
    .byte %00001111
    .byte %11110000
    .byte %11111111

.byte  %00000000
.byte %00000110
.byte %01100100
.byte %01101010
.byte %01000000
.byte %01000110
.byte %10100100
.byte %10101010

.byte %00000000
.byte %00000110
.byte %01100100
.byte %01101010
.byte %01000000
.byte %01000110
.byte %10100100
.byte %10101010
