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
tmp = ptr3

ByteOfCharMatrixIdx = 2
ByteOfSpriteMatrixIdx = 3

    lsr $01

    jsr fillGarbage

    lax #0
    sax ptr2
    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1

gensprite0:
    lda #<(charset+'9'*8)+6; (spriteCount-1)
    sta ByteOfCharMatrixIdx

    lda #<(sprites+(spriteCount-1)*64)+62
    sta ByteOfSpriteMatrixIdx

gensprite:
    ldy ByteOfCharMatrixIdx
    ldx charnum
    lda src+1
    cmp #>(charset+'9'*8)
    bcc :+
    cpy #<(charset+'0'*8)
    bcs :+
    ldy #<(charset+('z'*8))+6
    sty ByteOfCharMatrixIdx
    dec src+1
:
    lda #0
    sta hi+1

src = *+1
    lda 256*>(charset+(spriteCount-1)*8), y
    lsr
    and #%00111111
    ;sta tmp1
    ;sre tmp1
    ;asl
    ;ora tmp1
    ;slo tmp1
    ldx #3
    axs #0
    ldy expandtbl,x
    sty lo
    lsr
    lsr
    ldx #3
    axs #0
    ldy expandtbl,x
    sty hi
    lsr
    lsr
    tax
    ldy expandtbl,x
    sty hi+1
    ;asl
    ;rol hi+1
    ;;asl
    ;rol hi+1
    ;asl
    ;rol hi+1
    ;asl
    ;rol hi+1
    ;ora hi
    ;sta hi
    ;lda expandhi,x

    ;ldx ByteOfCharMatrixIdx
    ;ldx #20
    ;and #7
    ;tay
    ;ldx reptable,y
    lda #2
    sta tmp1
    ldx lineindex
setsprite0:
    ldy ByteOfSpriteMatrixIdx
setsprite:
    lda lo
    and raster,x
    ora tmp
    sta (ptr2),y
    dey
    lda hi
    and raster,x
    sta (ptr2),y
    dey
    lda hi+1
    and raster,x
    sta (ptr2),y
    dex
    dey
    dec tmp1
    bpl setsprite
    stx lineindex
    ;dex
    ;dex
    ;bpl setsprite
    ;dey
    sty ByteOfSpriteMatrixIdx

    lda ByteOfCharMatrixIdx
    bne :+

    lda #$cf
    dcp src+1
    bcc notfinished
    rts
notfinished:
    lda #0
:   dec ByteOfCharMatrixIdx
    and #7
    jne gensprite
    dec ByteOfCharMatrixIdx

    ;lda #0
    ;sta (ptr2),y
    ;;dey
    ;sta (ptr2),y
    ;dey
    ;sta (ptr2),y
    ;dey
    ;sta (ptr2),y
    ;dey
    ;tya
    ;sec
        ;sbc #64-21*3
    dey
    ;sec
    ;sbc #4;64-8*3
    sty ByteOfSpriteMatrixIdx
    lda #19
    sta lineindex

    cpy #$fe
    bne :+
    dec ptr2+1
:
    dec charnum
    jpl gensprite
    rts
bitmask:
    .byte $20
reptable:
    .byte 0,1,2,2,2,1,0,0
charnum:
    .byte 36

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

    bitcharset:
    .byte %01111101, %10000111, %11111110, %110000011
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