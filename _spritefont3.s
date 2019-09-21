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

tmp = ptr1
lo = tmp2
hi = tmp3
bitpos = ptr3
;hi+1 = tmp3+1 pseudo code!

    .ifdef FILL
    jsr fillGarbage
    .endif

    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1
bitTblIndex = *+1
    ldx #srclength-1
gensprite0:
    lda #3
    sta bitpos
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
    ;sta tmp+1
ByteOfSpriteMatrixIdx = *+1
    ldy #<(sprites+(spriteCount-1)*64)+62

    ; set one line of a sprite
setsprite:

lineindex = *+1
    ldx #20;20
    lda raster,x
    sta tmp

; set each byte of a line of a sprite
    ldx #2
ssprite:
    lda lo,x
    and tmp
    sta (ptr2),y
    tya
    bne :+
    dec ptr2+1
:   dey
    dex
    bpl ssprite

    dec lineindex
    bpl :+
    ldx #20 ;reset value for lineindex
    ;adjust spr ptr so that byte 63 of sprite is skipped
    dey
    stx lineindex
:
    dec tmp1
    bpl setsprite

    sty ByteOfSpriteMatrixIdx

    lax bitTblIndex
    dec bitpos
    jpl gensprite
    axs #3
    stx bitTblIndex
    jcs gensprite0
    rts
.endproc

.SEGMENT "SETTINGS"
.export multicol = *
    .byte   $ff

.export bgcol = *
    .byte   $07

.export sprcol = *
    .byte   $0a

.export sprmcol1 = *
    .byte   $0f

.export sprmcol2 = *
    .byte   $04

.DATA
raster:
    .byte %11111111
    .byte %11101110
    .byte %10101010
    .byte %10011001
    .byte %01010101
    .byte %10011001
    .byte %10101010
src:
    .byte 0 ;padding byte so that index into table starts at srclength-1 and ends at 0 in steps of 3
    .incbin "bitstream"
srclength = *-src
