.include "cc65runtime.sh"
.include "myMacros.sh"

.autoimport off

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

; with 6x7 font, 26 letters: 26*42bits = 1092 bits = 137 Bytes
spriteCount = 26
charset = $d000
sprites = $2000
linecnt = 20
xwidth = 6
ywidth = 7
bitsperchar = xwidth*ywidth ;42
totalbitcnt = bitsperchar*spriteCount ;1092
totalbytecount = (totalbitcnt+4)/8 ; add 4 for proper rounding -> 137
.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import fillGarbage

lo = tmp2
hi = tmp3
;hi+1 = tmp3+1 pseudo code!

    .ifdef FILL
    jsr fillGarbage
    .endif

    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1
bitTblIndex = *+1
    ldx #srclength-1
gensprite:
    ldy #2
    sty tmp1
getn:
    lda #0          ;2
    sta ptr2
    lsr src,x      ;5
    bcc :+          ;7
    ora #%00001111  ;9
:   lsr src,x      ;12
    bcc :+          ;14
    ora #%11110000  ;16
:   sta lo,y
    dex
    dey
    bpl getn

lineindex = *+1
    ldx #6;20
    lda raster,x
    ;pla
    ;stx lo+2
    sta andvalue
    txa
    pha
ByteOfSpriteMatrixIdx = *+1
    ldy #<(sprites+(spriteCount-1)*64)+62

    ; set one line of a sprite
setsprite:

; set each byte of a line of a sprite
    ldx #2
ssprite:
    lda lo,x
andvalue = *+1
    and #$ff
    sta (ptr2),y
    tya
    bne :+
    dec ptr2+1
:   dey
    dex
    bpl ssprite

    dec tmp1
    bpl setsprite

    pla
    tax
    dex
    bpl :+
    ldx #6 ;reset value for lineindex
    ;adjust spr ptr so that byte 63 of sprite is skipped
    dey
:
    stx lineindex
    sty ByteOfSpriteMatrixIdx

    lax bitTblIndex
    dec bitpos
    jpl gensprite
    axs #3
    stx bitTblIndex
    lda #3
    sta bitpos
    jcs gensprite
    rts
bitpos:
    .byte 3
.endproc

.SEGMENT "SETTINGS"
.export multicol = *
    .byte   $ff

.export bgcol = *
    .byte   $00

.export sprcol = *
    .byte   $0e

.export sprmcol1 = *
    .byte   $03

.export sprmcol2 = *
    .byte   $06

.DATA
raster:
    .byte %11111111
    .byte %11101110
    .byte %10111011
    .byte %10101010
    .byte %10011001
    .byte %01100110
    .byte %01010101
    .if 0

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
.endif

src:
    .byte 0
    .incbin "bitstream"
srclength = *-src
