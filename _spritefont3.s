.include "cc65runtime.sh"
.include "myMacros.sh"

; calculation 6x5 font:
; 30 bits per char, organized as 6 bits a row in pairs of two bits.
; 37 chars
; -> 1110 pixels in total -> 139 Bytes -> left appr. 120 bytes
; 3 tables, à 47 bytes approx (141 bytes).
; table 1+0 containing cols 5&4 as bitpairs,
; table 1+1 containing cols 3&2 as bitpairs,
; table 1+2 containing cols 1&0 as bitpairs

; currently: 138 byte code!
;          column
; line 1   5&4      3&2     1&0
; line 2

spriteCount = 37
charset = $d000
sprites = $2000
linecnt = 20
xwidth = 6
ywidth = 5
bitsperchar = xwidth*ywidth
totalbitcnt = bitsperchar*spriteCount

.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import fillGarbage

lo = tmp2
hi = tmp3
;hi+1 = tmp3+1 pseudo code!

ByteOfSpriteMatrixIdx = 3

    lsr $01

    jsr fillGarbage

    lax #0
    sax ptr2
    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1
gensprite0:
    lda #<(sprites+(spriteCount-1)*64)+5*4*3-1
    sta ByteOfSpriteMatrixIdx

gensprite:
    ldy bitTblIndex

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

    lda #3
    sta tmp1
    ldx lineindex
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
    dey
    dex
    dec tmp1
    bpl setsprite

    txa
    bpl :+
    ldx #19 ;reset value for lineindex
    dey     ;adjust spr ptr so that line 21 is skipped
    dey     ;because we use only 20 lines of the sprites
    dey
    dey
    cpy #$fb
    bcc :+
    dec ptr2+1
:   sty ByteOfSpriteMatrixIdx
    stx lineindex

    lsr bitmask
    jcc gensprite
    lda #%00001000
    sta bitmask
    dec bitTblIndex
    jpl gensprite
    rts
bitmask:
    .byte %00001000
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
bitTblIndex:
    .byte 46
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

src1:
    .byte %01111101, %10000111, %11111110, %11000001, %10000000
    .res 47-(*-src1), %10101010
src2:
    .byte %01111101, %10000111, %11111110, %11000001, %10000000
    .res 47-(*-src2), %11001100
src3:
    .byte %01111101, %10000111, %11111110, %11000001, %10000000
    .res 47-(*-src3), %11110000
expandtbl:
    .byte %00000000
    .byte %00001111
    .byte %11110000
    .byte %11111111
